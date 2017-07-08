/** 
 * @file broadcast_receiver.cpp
 * 
 * This file contains the broadcast receiver which handles messages like 
 * `TesterPresent`.
 */

#include "broadcast_receiver.h"
#include "service_identifier.h"
#include <iostream>

using namespace std;

BroadcastReceiver::BroadcastReceiver(canid_t source,
                                     const string& device,
                                     const IsoTpSender& sender,
                                     SessionController* pSesCtrl)
: IsoTpReceiver(source, BROADCAST_ADDR, device)
, sender_(sender)
, pSessionCtrl_(pSesCtrl)
{
}

/**
 * Handles the broadcast messages (e.g. `TesterPresent`).
 * 
 * @param buffer: the buffer of the UDS message
 * @param num_bytes: the number of transmitted data in bytes
 */
void BroadcastReceiver::proceedReceivedData(const uint8_t* buffer,
                                            const size_t num_bytes) noexcept
{
    switch (buffer[0])
    {
        case TESTER_PRESENT_REQ:
        {
            pSessionCtrl_->reset();
            constexpr array<uint8_t, 1> tp = {TESTER_PRESENT_RES};
            sender_.sendData(tp.data(), tp.size());
            break;
        }
        default:
        {
            cerr << "Invalid UDS broadcast request received!\n";
        }
    }
}

