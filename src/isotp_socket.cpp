/** 
 * @file isotp_sender.cpp
 * 
 * This file holds a basic sender and receiver class to transmit data over CAN
 * using the ISO-TP protocol.
 * 
 * @attention The thread using receiver function is not supposed to get shipped 
 * in the final version! This is only for testing purposes of the system, to 
 * allow sending* and receiving data frames within one single application! 
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
#include <cstdint>
#include <iomanip>

using namespace std;

/**
 * Writes the given data into the ISO-TP socket. 
 * 
 * @param buffer: the pointer to the buffer
 * @param size: the number of bytes to write in the socket
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
    strncpy(ifr.ifr_name, device_.c_str(), device_.length());
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

    cout << __func__ << "() sent " << bytes_sent << " bytes\n";
    close(skt);

    return bytes_sent;
}

/**
 * Starts a thread which listens on the given socket addresses.
 * 
 * @deprecated The whole thread implementation is only for testing the sender 
 * and receiver system in an asynchronous way within one single application. It 
 * is not supposed to have this mechanism in the final app!
 */
void IsoTpSocket::receiveData()
{
    receiver_thread_ = thread(IsoTpSocket::receiveDataImpl, source_, dest_, device_);
}

/**
 * Closes the receiver thread and waits until the thread is finished.
 * 
 * @deprecated The whole thread implementation is only for testing the sender 
 * and receiver system in an asynchronous way within one single application. It 
 * is not supposed to have this mechanism in the final app!
 */
void IsoTpSocket::closeReceive()
{
    receiver_thread_.join();
}

/**
 * A static function which receives data from the given source address.
 * 
 * @param source: the source socket address
 * @param dest: the destination socket address (for response messages)
 * @param device: the device (e.g. "vcan0")
 * @return the number of received bytes or a negative value on failure
 */
int IsoTpSocket::receiveDataImpl(unsigned int source,
                                 unsigned int dest,
                                 const std::string& device) noexcept
{
    struct sockaddr_can addr;
    addr.can_addr.tp.tx_id = source;
    addr.can_addr.tp.rx_id = dest;
    addr.can_family = AF_CAN;

    int skt = socket(PF_CAN, SOCK_DGRAM, CAN_ISOTP);
    if (skt < 0)
    {
        cerr << __func__ << "() socket: " << strerror(errno) << '\n';
        return -1;
    }

    struct ifreq ifr;
    strncpy(ifr.ifr_name, device.c_str(), device.length());
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

    constexpr size_t BUFSIZE = 5000; // > 4096 bytes
    uint8_t msg[BUFSIZE];

    int num_bytes = read(skt, msg, BUFSIZE);
    cerr << __func__ << "() received " << num_bytes << " bytes!\n";
    if (num_bytes > 0)
    {
        cerr << __func__ << "() received data:";
        for (int i = 0; i < num_bytes; i++)
        {
            /* TODO: (Florian 28.05.17) This output is only for demonstration
             * and test purposes. A proper implementation which evaluates the 
             * received data, has to be done here.
             */
            cerr << " 0x"
                 << setw(2)
                 << setfill('0')
                 << hex
                 << static_cast<int>(msg[i]);
        }
    }
    close(skt);
    return num_bytes;
}