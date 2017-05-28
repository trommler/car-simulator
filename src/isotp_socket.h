/** 
 * @file isotp_socket.h
 *
 */

#ifndef ISOTP_SOCKET_H
#define ISOTP_SOCKET_H

#include <cstddef>
#include <string>
#include <thread>

class IsoTpSocket
{
public:
    IsoTpSocket() = delete;

    IsoTpSocket(unsigned int source,
                unsigned int dest,
                const std::string& device)
    : source_(source)
    , dest_(dest)
    , device_(device) { }
    virtual ~IsoTpSocket() = default;
    int sendData(const void* buffer, std::size_t size) noexcept;
    void receiveData();
    void closeReceive();

private:
    unsigned int source_;
    unsigned int dest_;
    const std::string device_;
    std::thread receiver_thread_;

    static int receiveDataImpl(unsigned int source,
                               unsigned int dest,
                               const std::string& device) noexcept;
};

#endif /* ISOTP_SOCKET_H */

