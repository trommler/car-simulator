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
: IsoTpReceiver(BROADCAST_ADDR, source, device)
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
        case DIAGNOSTIC_SESSION_CONTROL_REQ:
        {
                diagnosticSessionControl(buffer, num_bytes);
                break;
        }
        case 0x50:
        {
            constexpr array<uint8_t, 10> tp = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09};
            sender_.sendData(tp.data(), tp.size());
            break;
        }
        default:
        {
            cerr << "Invalid UDS broadcast request received!\n";
        }
    }
}

/**
 * Starts a session and sends back the corresponding response message.
 *
 * @param buffer: the buffer containing the UDS message
 * @param num_bytes: the length of the message in bytes
 */
void BroadcastReceiver::diagnosticSessionControl(const uint8_t* buffer, const size_t num_bytes)
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

