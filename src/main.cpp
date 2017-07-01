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
    cout << "start_server for config file: " << config_file << endl;

    EcuLuaScript script("PCM", LUA_CONFIG_PATH + config_file);
    ElectronicControlUnit ecu(device, move(script));

    ecu.testECU(config_file, device);
}

void vitural_timer(){
    int i = 0;
    while(true){
        usleep(1000000);
        printf("timer:  %d\n",i++);      
    }
}

void session_control_test(){
    
    IsoTpSocket tester_message(0x200,0x100,"vcan0");
    IsoTpSocket tester_broadcast(0x7e0,0x100,"vcan0");
    printf("start test function of session control\n");
    usleep(2000);
    constexpr array<uint8_t, 3> diagnose_session_req = {0x22, 0xfe, 0x10};
    constexpr array<uint8_t, 3> diagnose_session_req1 = {0x10, 0x02,0x10};
    constexpr array<uint8_t, 3> diagnose_session_req2 = {0x22, 0x00,0x03};
    constexpr array<uint8_t, 3> diagnose_session_message1 = {0x10, 0x02,0x0003};
    constexpr array<uint8_t, 3> tester_present_message = {0x3E};
    
    
    tester_message.openSender();
    tester_broadcast.openSender();
    
    usleep(5000*1000);
    printf("sending diagnose session req  \n");
    tester_message.sendData(diagnose_session_req.data(),diagnose_session_req.size());
    tester_message.sendData(diagnose_session_req1.data(),diagnose_session_req1.size());
    tester_message.sendData(diagnose_session_req2.data(),diagnose_session_req2.size());
    usleep(10000*1000);
    
    printf("sending diagnose session req  \n");
    tester_message.sendData(diagnose_session_req.data(),diagnose_session_req.size());
    tester_message.sendData(diagnose_session_req1.data(),diagnose_session_req1.size());
    tester_message.sendData(diagnose_session_req2.data(),diagnose_session_req2.size());
    
    usleep(3000*1000);
    printf("sending tester present message  \n");
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
    //timer_test(3);

    string device = "vcan0";
    if (argc > 1)
    {
        device = argv[1];
    }

    // listen to this communication with `isotpsniffer -s 100 -d 200 -c -td vcan0`

    vector<string> config_files = utils::getConfigFilenames(LUA_CONFIG_PATH);
    vector<thread> threads;

    thread timer(vitural_timer);
    threads.push_back(move(timer));
    
    thread session_control(session_control_test);
    threads.push_back(move(session_control));
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

