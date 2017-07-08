/** 
 * @file broadcast_receiver.h
 *
 */

#ifndef BROADCAST_RECEIVER_H
#define BROADCAST_RECEIVER_H

#include "isotp_receiver.h"
#include "isotp_sender.h"
#include "session_controller.h"
#include <string>

/// CAN address for broadcast messages like `TesterPresent`
static constexpr canid_t BROADCAST_ADDR = 0x7DF;

class BroadcastReceiver : public IsoTpReceiver
{
public:

    BroadcastReceiver() = delete;
    BroadcastReceiver(canid_t source,
                      const std::string& device,
                      const IsoTpSender& sender,
                      SessionController* pSesCtrl);
    BroadcastReceiver(const BroadcastReceiver& orig) = default;
    BroadcastReceiver& operator =(const BroadcastReceiver& orig) = default;
    BroadcastReceiver(BroadcastReceiver&& orig) = default;
    BroadcastReceiver& operator =(BroadcastReceiver&& orig) = default;
    virtual ~BroadcastReceiver() = default;

    virtual void proceedReceivedData(const std::uint8_t* buffer,
                                     const std::size_t num_bytes) noexcept override;

private:
    const IsoTpSender& sender_;
    SessionController* pSessionCtrl_;

};

#endif /* BROADCAST_RECEIVER_H */

