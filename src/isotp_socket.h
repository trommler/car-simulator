/**
 * @file isotp_socket.h
 *
 */

#ifndef ISOTP_SOCKET_H
#define ISOTP_SOCKET_H

#include <cstddef>
#include <string>
#include <thread>
#include <linux/can.h>

class IsoTpSocket
{
public:
    IsoTpSocket() = delete;

    IsoTpSocket(canid_t source,
                canid_t dest,
                const std::string& device)
    : source_(source)
    , dest_(dest)
    , device_(device)
    {
    }
    virtual ~IsoTpSocket() = default;
    int sendData(const void* buffer, std::size_t size) noexcept;
    int receiveData() noexcept;
    void exitReceive();

private:
    canid_t source_;
    canid_t dest_;
    const std::string device_;
    bool isOnExit_ = false;
    int basicUdsService(std::uint8_t* buffer, std::size_t size);

};

#endif /* ISOTP_SOCKET_H */
