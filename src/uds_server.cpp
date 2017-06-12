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
void UdsServer::proceedReceivedData(const uint8_t* buffer, size_t num_bytes) noexcept
{
    response_data_size_ = 0;
    switch (buffer[0])
    {
            // TODO: Enhance the functionality. This are only a basic response test. 
        case ECU_RESET_REQ:
        {
            if (!script_.getDataByIdentifier(0xf124).empty())
            {
                // FIXME: this is not the proper response, since this is only a simple test!
                string tmpData = script_.getDataByIdentifier(0xf124);
                sendData(tmpData.c_str(), tmpData.length());
            }
            break;
        }
        case READ_DATA_BY_IDENTIFIER_REQ:
        {
            uint16_t temp = (buffer[1] << 8) + buffer[2];
            // printf("READ_DATA_BY_IDENTIFIER_REQ %x \n", temp);
            if (!script_.getDataByIdentifier(temp).empty())
            {
                //send positive response
                response_data_[response_data_size_++] = READ_DATA_BY_IDENTIFIER_RES;
                response_data_[response_data_size_++] = buffer[1];
                response_data_[response_data_size_++] = buffer[2];
                copyLuaScriptResponse(script_.getDataByIdentifier(temp));
                sendData(response_data_, response_data_size_);
            }
            else
            {
                // send out of range
                response_data_[response_data_size_++] = ERROR;
                response_data_[response_data_size_++] = OUT_OF_RANGE;
                sendData(response_data_, response_data_size_);
            }
            break;
        }
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
            std::cerr << "Invalid UDS request received!\n";
    }
}

void UdsServer::copyLuaScriptResponse(std::string lua_response)
{
    for (unsigned int i = 0; i < lua_response.length(); i++)
    {
        response_data_[response_data_size_++] = lua_response[i];
    }
}
