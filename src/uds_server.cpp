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

/**
 * Handles the received UDS messages and send response back the according to the
 * Lua script.
 * 
 * @param buffer: the buffer containing the received data 
 * @param num_bytes: the number of received bytes.
 * @see IsoTpSocket::sendData()
 * @see EcuLuaScript::getRequestId()
 * @see EcuLuaScript::getResponseId
 */
void UdsServer::proceedReceivedData(const uint8_t* buffer, size_t num_bytes) noexcept
{
    switch (buffer[0])
    {
        // TODO: Enhance the functionality. This are only a basic response test. 
        case DIAGNOSTIC_SESSION_CONTROL_REQ:
        {
            // gets the response ID from the Lua script
            std::uint16_t tmpRespId = script_.getResponseId();
            if (tmpRespId != 0)
            {
                // FIXME: this is not the proper response, since this is only a simple test!
                sendData(&tmpRespId, sizeof(tmpRespId));
            }
            break;
        }
        case ECU_RESET_REQ:
        {
            const std::string testStr = script_.getDataByIdentifier(0xf124);
            if (!testStr.empty())
            {
                // FIXME: this is not the proper response, since this is only a simple test!
                sendData(testStr.c_str(), testStr.length());
            }
            break;
        }
        // TODO: implement all other requests ...
        default:
            std::cerr << "Invalid UDS request received!\n";
    }

}

