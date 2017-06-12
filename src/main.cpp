/**
 * @file main.cpp
 *
 * This file contains the `main()`-function with a simple Lua script test. See
 * https://github.com/jeremyong/Selene for usage instructions.
 */

#include "ecu_lua_script.h"
#include "ECU.h"
#include <string>

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

    EcuLuaScript script("PCM", PATH_TO_LUA);
    ECU motor(device, move(script));
    motor.initECU(device);
    return 0;
}
