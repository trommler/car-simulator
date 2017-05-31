/** 
 * @file: uds_server.cpp
 * 
 * Implementation of an UDS server, which receives messages via ISO_TP.
 */

#include "uds_server.h"
#include "service_identifier.h"
#include <iostream>

/**
 * Handles the received UDS messages and send response back the according 
 * response.
 * 
 * @param buffer: the buffer containing the received data 
 * @param num_bytes: the number of received bytes.
 */
void UdsServer::proceedReceivedData(const uint8_t* buffer, size_t num_bytes) noexcept
{
    switch (buffer[0])
    {
    case DIAGNOSTIC_SESSION_CONTROL_REQ:
        // TODO: enhance functionality (this is only a basic response test). 
        sendData(&DIAGNOSTIC_SESSION_CONTROL_RES, 1);
        break;
    case ECU_RESET_REQ:
        break;
        // TODO: implement all other requests
    default:
        std::cerr << "Invalid UDS request received!\n";
    }

}

