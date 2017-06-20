/** 
 * @file broadcast_server.h
 *
 */

#ifndef BROADCAST_SERVER_H
#define BROADCAST_SERVER_H

#include "isotp_socket.h"
#include "uds_server.h"
#include "ecuTimer.h"

constexpr canid_t BROADCAST_ADDR = 0x7E0; ///< CAN address for broadcast messages like `TesterPresent`

class BroadcastServer : public IsoTpSocket
{
public:
    BroadcastServer() = delete;
    BroadcastServer(canid_t dest, const std::string& device, UdsServer* uds_server);
    virtual ~BroadcastServer();
    virtual void proceedReceivedData(const std::uint8_t* buffer,
                                     const std::size_t num_bytes) noexcept override;
private:
    UdsServer* uds_server_;

};

#endif /* BROADCAST_SERVER_H */
