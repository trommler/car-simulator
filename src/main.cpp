/**
 * @file main.cpp
 *
 * This file contains the `main()`-function with a simple Lua script test. See
 * https://github.com/jeremyong/Selene for usage instructions.
 */

#include "ecu_lua_script.h"
#include "ECU.h"
#include "ecu_timer.h"
#include "utilities.h"
#include <string>

using namespace std;

void start_server(const string &config_file, const string &device)
{
    cout << "start_server for config file: " << config_file << endl;

    EcuLuaScript script("PCM", LUA_CONFIG_PATH + config_file);
    ECU ecu(device, move(script));

    ecu.testECU(config_file, device);
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
    //timer_test(3);

    string device = "vcan0";
    if (argc > 1)
    {
        device = argv[1];
    }

    // listen to this communication with `isotpsniffer -s 100 -d 200 -c -td vcan0`

    vector<string> config_files = utils::getConfigFilenames(LUA_CONFIG_PATH);
    vector<thread> threads;

    for (const string &config_file : config_files)
    {
        thread t(start_server, config_file, device);
        threads.push_back(move(t));
        usleep(50000);
    }

    for (unsigned int i = 0; i < threads.size(); ++i)
    {
        threads[i].join();
    }

    return 0;
}
