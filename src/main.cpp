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

/**
 * The main application only for testing purposes.
 *
 * @param argc: the number of arguments
 * @param argv: the argument list
 * @return 0 on success, otherwise a negative value
 */
int main(int argc, char** argv)
{
    constexpr array<uint8_t, 12> payload01 = {
        0x10, 0x59, 0x65, 0x73, 0x20, 0x77, 0x65, 0x20, 0x43, 0x41, 0x4E, 0x21
    };

    // listen to this communication with `isotpsniffer -s 123 -d 321 -c -td vcan0`

    IsoTpSocket my_sender(0x123, 0x321, "vcan0");
    my_sender.openSender();

    EcuLuaScript my_script("tests/testscript03.lua");
    UdsServer my_receiver(0x321, 0x123, "vcan0", move(my_script));

    usleep(5000);
    my_receiver.openReceiver();
    my_receiver.openSender(); // open also sender port for response
    thread t1(&IsoTpSocket::readData, &my_receiver); // run async in thread
    usleep(2000); // wait some time to ensure the thread is set up and running

    my_sender.sendData(payload01.data(), payload01.size()); // finally send the data

    constexpr array<uint8_t, 4> payload02 = {
        0x11, 0x00, 0x00, 0x00,
    };

    my_sender.sendData(payload02.data(), payload02.size()); // send some more data
    usleep(2000);
    my_sender.closeSender();

    my_receiver.closeSender();
    my_receiver.closeReceiver();

    t1.join(); // exit program with [Ctrl + C]

    return 0;
}
