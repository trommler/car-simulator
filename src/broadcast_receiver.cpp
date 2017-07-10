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
                                     UdsReceiver* pUdsRec,
                                     SessionController* pSesCtrl)
: IsoTpReceiver(BROADCAST_ADDR, source, device)
, pUdsReceiver_(pUdsRec)
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
            pUdsReceiver_->sender_.sendData(tp.data(), tp.size());
            break;
        }
        case 0x50:
        {
            constexpr array<uint8_t, 10> tp = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
            pUdsReceiver_->sender_.sendData(tp.data(), tp.size());
            break;
        }
        default:
        {
            pUdsReceiver_->proceedReceivedData(buffer, num_bytes);
        }
    }
}