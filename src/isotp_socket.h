/** 
 * @file isotp_socket.h
 * @author Florian Bauer
 *
 */

#ifndef ISOTP_SOCKET_H
#define ISOTP_SOCKET_H

#include <cstddef>
#include <string>

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
    int recieveData(void *buffer, std::size_t size) noexcept;

private:
    unsigned int source_;
    unsigned int dest_;
    const std::string device_;
};

#endif /* ISOTP_SOCKET_H */

