/** 
 * @file broadcast_server.cpp
 * 
 * This file contains the broadcast server which handles messages like 
 * `TesterPresent`.
 */

#include "broadcast_server.h"
#include "service_identifier.h"
#include <iostream>

using namespace std;

/**
 * Constructor. Opens the ISO-TP sockets for sending and receiving.
 * 
 * @param dest: CAN destination address
 * @param device: the hardware device (e.g "vcan0")
 * @param pSesCtrl: pointer to access the `SessionController`
 */
BroadcastServer::BroadcastServer(canid_t dest, const string& device, SessionController* pSesCtrl)
: IsoTpSocket(dest, BROADCAST_ADDR, device)
, pSessionCtrl_(pSesCtrl)
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
            pSessionCtrl_->reset();
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
