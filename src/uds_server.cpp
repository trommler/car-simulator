/**
 * @file uds_server.cpp
 *
 * Implementation of an UDS server, which receives requests and sends response
 * messages via ISO_TP.
 */

#include "uds_server.h"
#include "service_identifier.h"
#include "session_controller.h"
#include <iostream>
#include <cstdint>
#include <string>
#include <exception>
#include <array>
#include <vector>

using namespace std;

/**
 * Constructor. Opens the sockets for sending and receiving.
 *
 * @param device: the name of the transmitting device (e. g. "vcan0")
 * @param ecuScript: the `EcuLuaScipt` as rvalue
 */
UdsServer::UdsServer(canid_t source,
                     canid_t dest,
                     const string& device,
                     SessionController* pSesCtrl,
                     EcuLuaScript&& ecuScript)
: IsoTpSocket(source, dest, device)
, pSessionCtrl_(pSesCtrl)
, script_(move(ecuScript))
{
    int err = openReceiver();
    err |= openSender();

    if (err != 0)
    {
        throw exception();
    }
}

/**
 * Destructor. Closes the sockets for sending and receiving.
 */
UdsServer::~UdsServer()
{
    closeSender();
    closeReceiver();
}

/**
 * Handles the received UDS messages and sends back the response like defined in
 * the according Lua script.
 *
 * @param buffer: the buffer containing the received data
 * @param num_bytes: the number of received bytes.
 * @see IsoTpSocket::sendData()
 * @see EcuLuaScript::getRequestId()
 * @see EcuLuaScript::getResponseId
 */
void UdsServer::proceedReceivedData(const uint8_t* buffer, const size_t num_bytes) noexcept
{
    const uint8_t udsServiceIdentifier = buffer[0];
    switch (udsServiceIdentifier)
    {
        case READ_DATA_BY_IDENTIFIER_REQ:
            readDataByIdentifier(buffer, num_bytes);
            break;
        case DIAGNOSTIC_SESSION_CONTROL_REQ:
            diagnosticSessionControl(buffer, num_bytes);
            break;
        case SECURITY_ACCESS_REQ:
            securityAccess(buffer, num_bytes);
            break;
            // TODO: implement all other requests ...
        default:
            cerr << "Invalid UDS request received!\n";
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
void UdsServer::readDataByIdentifier(const uint8_t* buffer, const size_t num_bytes) noexcept
{
    const uint16_t dataIdentifier = (buffer[1] << 8) + buffer[2];
    const string data = script_.getDataByIdentifier(dataIdentifier);
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
        sendData(resp.data(), resp.size());
    }
    else // send out of range
    {
        constexpr array<uint8_t, 2> nrc = {
            ERROR,
            REQUEST_OUT_OF_RANGE
        };
        sendData(nrc.data(), nrc.size());
    }
}

/**
 * Starts a session and sends back the corresponding response message.
 * 
 * @param buffer: the buffer containing the UDS message
 * @param num_bytes: the length of the message in bytes
 */
void UdsServer::diagnosticSessionControl(const uint8_t* buffer, const size_t num_bytes)
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
    sendData(resp.data(), resp.size());
}

/**
 * 
 * @param buffer: the buffer containing the UDS message
 * @param num_bytes: the length of the message in bytes
 */
void UdsServer::securityAccess(const uint8_t* buffer, const size_t num_bytes) noexcept
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
        sendData(resp.data(), resp.size());
        securityAccessType_ = seedId + 0x01;
    }
    else
    {
        if (securityAccessType_ == seedId)
        {
            // second request
            constexpr array<uint8_t, 1> resp = {SECURITY_ACCESS_RES};
            // Lua seed function
            sendData(resp.data(), resp.size());
            securityAccessType_ = 0x00;
        }
        else
        {
            constexpr array<uint8_t, 2> resp = {
                ERROR,
                SUBFUNCTION_NOT_SUPPORTED
            };
            sendData(resp.data(), resp.size());
        }
    }
}
