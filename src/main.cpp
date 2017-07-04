/**
 * @file main.cpp
 *
 * This file contains the `main()`-function with a simple Lua script test. See
 * https://github.com/jeremyong/Selene for usage instructions.
 */

#include "ecu_lua_script.h"
#include "electronic_control_unit.h"
#include "ecu_timer.h"
#include "utilities.h"
#include <string>

using namespace std;

void start_server(const string &config_file, const string &device)
{
    cout << "start_server for config file: " << config_file
         << " on device: " << device << endl;

    EcuLuaScript script("Main", LUA_CONFIG_PATH + config_file);
    ElectronicControlUnit ecu(device, move(script));

    //ecu.testECU(config_file, device);
}

void vitural_timer(){
    int i = 0;
    while(true){
        usleep(1000000);
        cout << "timer: " << i++ << endl;
    }
}

void session_control_test(const string &device){

    IsoTpSocket tester_message(0x200, 0x100, device);
    IsoTpSocket tester_broadcast(0x7e0, 0x100, device);
    cout << "start test function of session control on device: " << device << endl;
    usleep(2000);
    constexpr array<uint8_t, 3> diagnose_session_req = {0x22, 0xfe, 0x10};
    constexpr array<uint8_t, 3> diagnose_session_req1 = {0x10, 0x02,0x10};
    constexpr array<uint8_t, 3> diagnose_session_req2 = {0x22, 0x00,0x03};
    constexpr array<uint8_t, 3> diagnose_session_message1 = {0x10, 0x02,0x0003};
    constexpr array<uint8_t, 3> tester_present_message = {0x3E};


    tester_message.openSender();
    tester_broadcast.openSender();

    usleep(5000*1000);
    cout << "sending diagnose session req" << endl;

    tester_message.sendData(diagnose_session_req.data(),diagnose_session_req.size());
    tester_message.sendData(diagnose_session_req1.data(),diagnose_session_req1.size());
    tester_message.sendData(diagnose_session_req2.data(),diagnose_session_req2.size());
    usleep(10000*1000);

    cout << "sending diagnose session req" << endl;
    tester_message.sendData(diagnose_session_req.data(),diagnose_session_req.size());
    tester_message.sendData(diagnose_session_req1.data(),diagnose_session_req1.size());
    tester_message.sendData(diagnose_session_req2.data(),diagnose_session_req2.size());

    usleep(3000*1000);
    cout << "sending tester present message" << endl;
    tester_message.sendData(diagnose_session_message1.data(),diagnose_session_message1.size());
    tester_broadcast.sendData(tester_present_message.data(),tester_present_message.size());
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

    // listen to this communication with `isotpsniffer -s 100 -d 200 -c -td vcan0`

    vector<string> config_files = utils::getConfigFilenames(LUA_CONFIG_PATH);
    vector<thread> threads;

    //thread timer(vitural_timer);
    //threads.push_back(move(timer));
    //usleep(50000);
    //thread session_control(session_control_test, device);
    //threads.push_back(move(session_control));
    //usleep(50000);

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

