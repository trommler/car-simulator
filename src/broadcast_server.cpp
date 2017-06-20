/** 
 * @file broadcast_server.cpp
 * 
 * This file contains the broadcast server which handles messages like 
 * `TesterPrsent`.
 */

#include "broadcast_server.h"
#include "service_identifier.h"

using namespace std;

/**
 * Constructor. Opens the ISO-TP sockets for sending and receiving.
 * 
 * @param dest: CAN destination address
 * @param device: the hardware device (e.g "vcan0")
 * @param uds_server: only for testing purposes
 */
BroadcastServer::BroadcastServer(canid_t dest, const string& device, UdsServer* uds_server)
: IsoTpSocket(dest, BROADCAST_ADDR, device)
, uds_server_(uds_server)
{
    int err = openReceiver();
    err |= openSender();
    if (err != 0)
    {
        throw exception();
    }
}

/**
 * Destructor. Closes the ISO-TP sockets for sending and receiving.
 */
BroadcastServer::~BroadcastServer()
{
    closeSender();
    closeReceiver();
}

/**
 * Handles the broadcast messages (e.g. `TesterPresent`).
 * 
 * @param buffer: the buffer of the UDS message
 * @param num_bytes: the number of transmitted data in bytes
 */
void BroadcastServer::proceedReceivedData(const uint8_t* buffer,
                                          const size_t num_bytes) noexcept
{
    switch (buffer[0])
    {
        case TESTER_PRESENT_REQ:
        {
            uds_server_->test_callback(42);

            // TODO: implement handling for TesterPresent
            // TODO: reset timer
            constexpr array<uint8_t, 1> tp = {TESTER_PRESENT_RES};
            sendData(tp.data(), tp.size());
            break;
        }
        default:
        {
            cerr << "Invalid UDS broadcast request received!\n";
        }
    }
}