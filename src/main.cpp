/**
 * @file main.cpp
 *
 * This file contains the `main()`-function with a simple Lua script test. See
 * https://github.com/jeremyong/Selene for usage instructions.
 */

#include "selene.h"
#include "utilities.h"
#include "isotp_socket.h"
#include "uds_server.h"
#include "ecu_lua_script.h"
#include "ECU.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <libgen.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <array>
#include <cstdint>
#include <iostream>
#include <cerrno>
#include <thread>

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

void isotpCanTest(string device)
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

    addr.can_family = AF_CAN;

    struct ifreq ifr;
    strncpy(ifr.ifr_name, device.c_str(), device.size());
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

    cerr << __func__ << "() sent " << bytes_sent << " bytes\n";
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
    string device = "vcan0";
    if (argc > 1)
    {
        device = argv[1];
    }

    ECU motor;
    motor.initECU(device);
    return 0;
}
