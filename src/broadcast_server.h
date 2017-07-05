/** 
 * @file broadcast_server.h
 *
 */

#ifndef BROADCAST_SERVER_H
#define BROADCAST_SERVER_H

#include "isotp_socket.h"
#include "session_controller.h"

/// CAN address for broadcast messages like `TesterPresent`
constexpr canid_t BROADCAST_ADDR = 0x7DF;

class BroadcastServer : public IsoTpSocket
{
public:
    BroadcastServer() = delete;
    BroadcastServer(canid_t dest, const std::string& device, SessionController* pSesCtrl);
    virtual ~BroadcastServer();
    virtual void proceedReceivedData(const std::uint8_t* buffer,
                                     const std::size_t num_bytes) noexcept override;
private:
    SessionController* pSessionCtrl_;
};

#endif /* BROADCAST_SERVER_H */
