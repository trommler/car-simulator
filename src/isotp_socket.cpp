/** 
 * @file isotp_sender.cpp
 * 
 * This file holds a basic sender and receiver class to transmit data over CAN
 * using the ISO-TP protocol.
 * 
 */

#include "isotp_socket.h"
#include "service_identifier.h"
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <array>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <cstdint>
#include <iomanip>

using namespace std;

constexpr size_t MAX_BUFSIZE = 5000; ///< >= 4096 bytes

/**
 * Writes the given data into the ISO-TP socket. 
 * 
 * @param buffer: the pointer to the buffer
 * @param size: the number of bytes to write in the socket
 * @return the number of sent bytes or a negative value on error
 */
int IsoTpSocket::sendData(const void* buffer, std::size_t size) noexcept
{
    struct sockaddr_can addr;
    addr.can_addr.tp.tx_id = source_; // sender
    addr.can_addr.tp.rx_id = dest_; // receiver
    addr.can_family = AF_CAN;

    int skt = socket(PF_CAN, SOCK_DGRAM, CAN_ISOTP);
    if (skt < 0)
    {
        cerr << __func__ << "() socket: " << strerror(errno) << '\n';
        return -1;
    }

    struct ifreq ifr;
    strncpy(ifr.ifr_name, device_.c_str(), device_.length());
    ioctl(skt, SIOCGIFINDEX, &ifr);
    addr.can_ifindex = ifr.ifr_ifindex;

    auto bind_res = bind(skt,
                         reinterpret_cast<struct sockaddr*> (&addr),
                         sizeof (addr));
    if (bind_res < 0)
    {
        cerr << __func__ << "() bind: " << strerror(errno) << '\n';
        close(skt);
        return -2;
    }

    auto bytes_sent = write(skt, buffer, size);
    if (bytes_sent < 0)
    {
        cerr << __func__ << "() write: " << strerror(errno) << '\n';
        return -3;
    }

    cout << __func__ << "() sent " << dec << bytes_sent << " bytes\n";
    close(skt);

    return bytes_sent;
}

/**
 * Receives the via ISO-TP transmitted data.
 * 
 * @return 0 on success, otherwise a negative value
 */
int IsoTpSocket::receiveData()
{
    isOnExit_ = false;
    struct sockaddr_can addr;
    addr.can_addr.tp.tx_id = source_;
    addr.can_addr.tp.rx_id = dest_;
    addr.can_family = AF_CAN;

    int skt = socket(PF_CAN, SOCK_DGRAM, CAN_ISOTP);
    if (skt < 0)
    {
        cerr << __func__ << "() socket: " << strerror(errno) << '\n';
        return -1;
    }

    struct ifreq ifr;
    strncpy(ifr.ifr_name, device_.c_str(), device_.length());
    ioctl(skt, SIOCGIFINDEX, &ifr);
    addr.can_ifindex = ifr.ifr_ifindex;

    auto bind_res = bind(skt,
                         reinterpret_cast<struct sockaddr*> (&addr),
                         sizeof (addr));
    if (bind_res < 0)
    {
        cerr << __func__ << "() bind: " << strerror(errno) << '\n';
        close(skt);
        return -2;
    }

    uint8_t msg[MAX_BUFSIZE];
    size_t num_bytes;
    do
    {
        num_bytes = read(skt, msg, MAX_BUFSIZE);
        if (num_bytes > 0 && num_bytes < MAX_BUFSIZE)
        {
            cout << __func__ << "() received " << dec << num_bytes << " bytes!\n";
            for (unsigned i = 0; i < num_bytes; i++)
            {
                /* TODO: (Florian 30.05.17) This output is only for 
                 * demonstration and test purposes. A proper implementation 
                 * which evaluates the received data, has to be done here.
                 */
                cout << " 0x"
                        << hex
                        << setw(2)
                        << setfill('0')
                        << static_cast<int> (msg[i]);
            }
            cout << '\n';
        }
    }
    while (!isOnExit_);

    close(skt);
    return 0;
}

void IsoTpSocket::exitReceive()
{
    isOnExit_ = true;
}
