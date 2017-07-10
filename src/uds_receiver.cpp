/** 
 * @file uds_receiver.cpp
 * 
 * Implementation of an UDS server, which receives requests and sends response
 * messages via ISO_TP.
 */

#include "uds_receiver.h"
#include "service_identifier.h"
#include <vector>
#include <array>
#include <iostream>

using namespace std;

UdsReceiver::UdsReceiver(canid_t source,
                         canid_t dest,
                         const string& device,
                         EcuLuaScript&& ecuScript,
                         const IsoTpSender& sender,
                         SessionController* pSesCtrl)
: IsoTpReceiver(source, dest, device)
, script_(move(ecuScript))
, sender_(sender)
, pSessionCtrl_(pSesCtrl)
{
}

/**
 * Handles the received UDS messages and sends back the response like defined in
 * the according Lua script.
 *
 * @param buffer: the buffer containing the received data
 * @param num_bytes: the number of received bytes.
 * @see IsoTpSender::sendData()
 * @see EcuLuaScript::getRequestId()
 * @see EcuLuaScript::getResponseId()
 */
void UdsReceiver::proceedReceivedData(const uint8_t* buffer, const size_t num_bytes) noexcept
{
    const uint8_t udsServiceIdentifier = buffer[0];
    const bool isRaw = script_.hasRaw(intToHexString(buffer, num_bytes));

    if (isRaw)
    {
        vector<unsigned char> raw = script_.literalHexStrToBytes(script_.getRaw(intToHexString(buffer, num_bytes)));
        sender_.sendData(raw.data(), raw.size());
    }
    else
    {
        switch (udsServiceIdentifier)
        {
            case READ_DATA_BY_IDENTIFIER_REQ:
            {
                readDataByIdentifier(buffer, num_bytes);
                pSessionCtrl_->reset();
                break;
            }
            case DIAGNOSTIC_SESSION_CONTROL_REQ:
                diagnosticSessionControl(buffer, num_bytes);
                break;
            case SECURITY_ACCESS_REQ:
                //                securityAccess(buffer, num_bytes);
                break;
                // TODO: implement all other requests ...
            default:
                cerr << "Invalid UDS request received!\n";
        }
    }
}

/**
 * Handles the UDS `readDataByIdentifier` request. The ISO-TP layer already
 * ensures the min. length of 3 bytes by filling up the request with zero bytes
 * if necessary.
 *
 * @param buffer: the buffer containing the UDS message
 * @param num_bytes: the length of the message in bytes (min. 3 bytes)
 */
void UdsReceiver::readDataByIdentifier(const uint8_t* buffer, const size_t num_bytes) noexcept
{
    const uint16_t dataIdentifier = (buffer[1] << 8) + buffer[2];
    string data;

    if (pSessionCtrl_->getCurretnUdsSession() == UdsSession::PROGRAMMING)
    {
        data = script_.getDataByIdentifier(EcuLuaScript::toByteResponse(dataIdentifier, sizeof(dataIdentifier)), "Programming");
    }
    else if (pSessionCtrl_->getCurretnUdsSession() == UdsSession::EXTENDED)
    {
        data = script_.getDataByIdentifier(EcuLuaScript::toByteResponse(dataIdentifier, sizeof(dataIdentifier)), "Extended");
    }
    else // default session
    {
        const string resp = EcuLuaScript::toByteResponse(dataIdentifier, sizeof(dataIdentifier));
        data = script_.getDataByIdentifier(resp);
    }


    if (!data.empty())
    {
        // send positive response
        vector<uint8_t> resp(data.length() + 3); // data + UDS header
        resp = {
            READ_DATA_BY_IDENTIFIER_RES,
            buffer[1],
            buffer[2]
        };
        resp.insert(resp.cend(), data.cbegin(), data.cend()); // insert payload
        sender_.sendData(resp.data(), resp.size());
    }
    else // send out of range
    {
        constexpr array<uint8_t, 2> nrc = {
            ERROR,
            REQUEST_OUT_OF_RANGE
        };
        sender_.sendData(nrc.data(), nrc.size());
    }
}

/**
 * Starts a session and sends back the corresponding response message.
 *
 * @param buffer: the buffer containing the UDS message
 * @param num_bytes: the length of the message in bytes
 */
void UdsReceiver::diagnosticSessionControl(const uint8_t* buffer, const size_t num_bytes)
{
    const uint8_t sessionId = buffer[1];
    switch (sessionId)
    {
        case 0x01: // UdsSession::DEFAULT
            pSessionCtrl_->setCurrentUdsSession(UdsSession::DEFAULT);
            break;
        case 0x02: // UdsSession::PROGRAMMING
            pSessionCtrl_->setCurrentUdsSession(UdsSession::PROGRAMMING);
            pSessionCtrl_->start(SESSION_TIME);
            break;
        case 0x03: // UdsSession::EXTENDED
            pSessionCtrl_->setCurrentUdsSession(UdsSession::EXTENDED);
            pSessionCtrl_->start(SESSION_TIME);
            break;
        default:
            cerr << "Invalid session ID!\n";
            break;
    }

    const array<uint8_t, 2> resp = {
        DIAGNOSTIC_SESSION_CONTROL_RES,
        sessionId
    };
    sender_.sendData(resp.data(), resp.size());
}

/**
 *
 * @param buffer: the buffer containing the UDS message
 * @param num_bytes: the length of the message in bytes
 */
void UdsReceiver::securityAccess(const uint8_t* buffer, const size_t num_bytes) noexcept
{
    const uint8_t seedId = buffer[1];
    const string seed = script_.getSeed(seedId);
    if (!seed.empty())
    {
        // send seed
        vector<uint8_t> resp(seed.length() + 2); // data + UDS header
        resp = {
            SECURITY_ACCESS_REQ,
            seedId
        };
        resp.insert(resp.cend(), seed.cbegin(), seed.cend() - 1); // insert payload - nullbyte
        sender_.sendData(resp.data(), resp.size());
        securityAccessType_ = seedId + 0x01;
    }
    else
    {
        if (securityAccessType_ == seedId)
        {
            // second request
            constexpr array<uint8_t, 1> resp = {SECURITY_ACCESS_RES};
            // Lua seed function
            sender_.sendData(resp.data(), resp.size());
            securityAccessType_ = 0x00;
        }
        else
        {
            constexpr array<uint8_t, 2> resp = {
                ERROR,
                SUBFUNCTION_NOT_SUPPORTED
            };
            sender_.sendData(resp.data(), resp.size());
        }
    }
}

string UdsReceiver::intToHexString(const uint8_t* buffer, const size_t num_bytes)
{
    string a = "";

    for (unsigned int i = 0; i < num_bytes; i++)
    {
        if ((buffer[i] / 16) > 9)
        {
            if ((buffer[i] / 16) == 0x0a)
                a.append("A");
            else if ((buffer[i] / 16) == 0x0b)
                a.append("B");
            else if ((buffer[i] / 16) == 0x0c)
                a.append("C");
            else if ((buffer[i] / 16) == 0x0d)
                a.append("D");
            else if ((buffer[i] / 16) == 0x0e)
                a.append("E");
            else if ((buffer[i] / 16) == 0x0f)
                a.append("F");
        }
        else
        {
            a.append(to_string(buffer[i] / 16));
        }
        if ((buffer[i] % 16) > 9)
        {
            if ((buffer[i] % 16) == 0x0a)
                a.append("A");
            else if ((buffer[i] % 16) == 0x0b)
                a.append("B");
            else if ((buffer[i] % 16) == 0x0c)
                a.append("C");
            else if ((buffer[i] % 16) == 0x0d)
                a.append("D");
            else if ((buffer[i] % 16) == 0x0e)
                a.append("E");
            else if ((buffer[i] % 16) == 0x0f)
                a.append("F");
        }
        else
        {
            a.append(to_string(buffer[i] % 16));
        }
        if (!(i == num_bytes - 1))
        {
            a.append(" ");
        }
    }
    return a;
}