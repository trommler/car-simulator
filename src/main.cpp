/**
 * @file main.cpp
 * @author Florian Bauer
 *
 * This file contains the `main()`-function with a simple Lua script test. See
 * https://github.com/jeremyong/Selene for usage instructions.
 */

#include "selene.h"
#include "utilities.h"
#include "isotp_socket.h"
#include "UDS_ECU.h"

#include <lua.hpp>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <libgen.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <array>
#include <cstdint>
#include <iostream>
#include <cerrno>

using namespace std;

void luaTest()
{
    const string test_script = "tests/testscript01.lua";

    if (utils::existsFile(test_script))
    {
        sel::State state;
        state.Load(test_script);

        cout << "The Answer: " << int(state["theAnswer"]) << '\n';
        const string cmd = state["bashCommand"];
        cout << "The Bash command everyone should try out: \"" << cmd << "\"\n";
    }
}

void rawCanTest()
{
    int skt = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    // Locate the interface you wish to use
    struct ifreq ifr;
    strcpy(ifr.ifr_name, "vcan0");
    // ifr.ifr_ifindex gets filled with that device's index
    ioctl(skt, SIOCGIFINDEX, &ifr);

    // Select that CAN interface, and bind the socket to it.
    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(skt, (struct sockaddr*) &addr, sizeof(addr));

    // Send a message to the CAN bus
    struct can_frame frame; // can_frame payload is limited to 8 Byte.
    frame.can_id = 0x100;
    constexpr unsigned char buf[] = {
        0x11, 0x22, 0x33, 0x44, 0x55,
        0x66, 0x77, 0x88, 0x99, 0xaa
    };
    memcpy(frame.data, buf, 8);
    frame.can_dlc = 8;
    auto bytes_sent = write(skt, &frame, sizeof(frame));

    cerr << __func__ << "() sent " << bytes_sent << " Bytes\n";
    close(skt);
}

void isotpCanTest()
{
    struct sockaddr_can addr;

    addr.can_addr.tp.tx_id = 0x123; // sender
    addr.can_addr.tp.rx_id = 0x321; // receiver

    int skt = socket(PF_CAN, SOCK_DGRAM, CAN_ISOTP);
    if (skt < 0)
    {
        cerr << __func__ << "() socket: " << strerror(errno) << '\n';
        exit(1);
    }

    // static struct can_isotp_options opts;
    // setsockopt(skt, SOL_CAN_ISOTP, CAN_ISOTP_OPTS, &opts, sizeof(opts));

    addr.can_family = AF_CAN;

    struct ifreq ifr;
    constexpr char dev[] = "vcan0";
    strncpy(ifr.ifr_name, dev, sizeof(dev));
    ioctl(skt, SIOCGIFINDEX, &ifr);
    addr.can_ifindex = ifr.ifr_ifindex;

    auto bind_res = bind(skt,
                         reinterpret_cast<struct sockaddr*>(&addr),
                         sizeof(addr));
    if (bind_res < 0)
    {
        cerr << __func__ << "() bind: " << strerror(errno) << '\n';
        close(skt);
        exit(1);
    }

    constexpr array<uint8_t, 16> buf = {
        0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
    };

    auto bytes_sent = write(skt, buf.data(), buf.size());
    if (bytes_sent < 0)
    {
        cerr << __func__ << "() write: " << strerror(errno) << '\n';
        return;
    }

    cerr << __func__ << "() sent " << bytes_sent << " Bytes\n";
    close(skt);
}

/**
 * The main application only for testing purposes.
 *
 * @param argc: the number of arguments
 * @param argv: the argument list
 * @return 0 on success, otherwise a negative value
 */
int main(int argc, char** argv)
{
    luaTest();
    rawCanTest();
    isotpCanTest();

    constexpr array<uint8_t, 32> payload = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
    };

    // listen with `isotprecv -s 321 -d 123 -l vcan0`
    IsoTpSocket my_sender(0x123, 0x321, "vcan0");
    my_sender.sendData(payload.data(), payload.size());


    UDS_ECU *testserver = new UDS_ECU(0x123, 0x321, "vcan0");
    cout << "start testserver" << endl;
    while(1)
    {
        testserver->receiveUDSmessage();
    }

    return 0;
}
