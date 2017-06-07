/**
 * @file main.cpp
 *
 * This file contains the `main()`-function with a simple Lua script test. See
 * https://github.com/jeremyong/Selene for usage instructions.
 */

#include "selene.h"
#include "utilities.h"
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
