/** 
 * @file isotp_sender.cpp
 * @author Florian Bauer
 * 
 * This file holds a basic sender class to transmit data over CAN using the 
 * ISO-TP protocol. 
 */

#include "isotp_socket.h"
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <array>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cerrno>

using namespace std;

/**
 * Writes the given data into the ISO-TP socket. 
 * 
 * @param buffer: the pointer to the buffer
 * @param size: the size to be written in the socket
 * @return the number of sent bytes or a negative value on failure
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
    strncpy(ifr.ifr_name, device_.c_str(), device_.length() + 1);
    ioctl(skt, SIOCGIFINDEX, &ifr);
    addr.can_ifindex = ifr.ifr_ifindex;

    auto bind_res = bind(skt,
                         reinterpret_cast<struct sockaddr*>(&addr),
                         sizeof(addr));
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

    cout << __func__ << "() sent " << bytes_sent << " Bytes\n";
    close(skt);

    return bytes_sent;
}
