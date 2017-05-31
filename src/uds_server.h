/** 
 * @file: uds_server.h
 *
 */

#ifndef UDSSERVER_H
#define UDSSERVER_H

#include "isotp_socket.h"

class UdsServer : public IsoTpSocket
{
public:
    UdsServer() = delete;

    UdsServer(canid_t source,
              canid_t dest,
              const std::string& device)
    : IsoTpSocket(source, dest, device)
    {
    }
    virtual ~UdsServer() = default;
    void proceedReceivedData(const std::uint8_t* buffer,
                             std::size_t num_bytes) noexcept override;

private:

};

#endif /* UDSSERVER_H */

