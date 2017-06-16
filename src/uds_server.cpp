/**
 * @file uds_server.cpp
 * 
 * Implementation of an UDS server, which receives requests and sends response
 * messages via ISO_TP.
 */

#include "uds_server.h"
#include "service_identifier.h"
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
                     EcuLuaScript&& ecuScript)
: IsoTpSocket(source, dest, device)
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
    response_data_size_ = 0;
    switch (buffer[0])
    {
        case READ_DATA_BY_IDENTIFIER_REQ:
            readDataByIdentifier(buffer, num_bytes);
            break;
        case TESTER_PRESENT_REQ:
        {
            // 0x7E0
            break;
        }
        case DIAGNOSTIC_SESSION_CONTROL_REQ:
        {
            response_data_[response_data_size_++] = DIAGNOSTIC_SESSION_CONTROL_RES;
            response_data_[response_data_size_++] = buffer[1];

            // lua session function
            sendData(response_data_, response_data_size_);

            break;
        }
        case SECURITY_ACCESS_REQ:
        {
            if (!script_.getSeed((buffer[1])).empty())
            {
                // send seed
                response_data_[response_data_size_++] = SECURITY_ACCESS_REQ;
                response_data_[response_data_size_++] = buffer[1];

                // get lua file seed
                securityAccessType = buffer[1] + 0x01;
                copyLuaScriptResponse(script_.getSeed(buffer[1]));
                sendData(response_data_, response_data_size_);
            }
            else
            {
                if (securityAccessType == buffer[1])
                {
                    // second request 
                    response_data_[response_data_size_++] = SECURITY_ACCESS_RES;

                    // lua seed function

                    sendData(response_data_, response_data_size_);
                    securityAccessType = 0x00;
                }
                else
                {
                    response_data_[response_data_size_++] = ERROR;
                    response_data_[response_data_size_++] = SUBFUNCTION_NOT_SUPPORTED;
                    sendData(response_data_, response_data_size_);
                }
            }
            break;
        }
            // TODO: implement all other requests ...
        default:
            cerr << "Invalid UDS request received!\n";
    }
}

void UdsServer::copyLuaScriptResponse(string lua_response)
{
    for (unsigned int i = 0; i < lua_response.length(); i++)
    {
        response_data_[response_data_size_++] = lua_response[i];
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
    uint16_t dataIdentifier = (buffer[1] << 8) + buffer[2];
    const string data = script_.getDataByIdentifier(dataIdentifier);
    if (!data.empty())
    {
        // send positive response
        vector<uint8_t> resp(data.length() + 3);
        resp = {
            READ_DATA_BY_IDENTIFIER_RES,
            buffer[1],
            buffer[2]
        };
        resp.insert(resp.cend(), data.cbegin(), data.cend()); // insert payload
        sendData(resp.data(), resp.size());
    }
    else
    {
        // send out of range
        constexpr array<uint8_t, 2> nrc = {
            ERROR,
            REQUEST_OUT_OF_RANGE
        };
        sendData(nrc.data(), nrc.size());
    }
}
