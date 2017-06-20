/**
 * @file ECU.cpp
 * 
 * This file contains a class which constitutes a Electronic Control Unit (ECU). 
 */

#include "ECU.h"
#include "isotp_socket.h"
#include <array>
#include <unistd.h>

using namespace std;

ECU::ECU(const string& device, EcuLuaScript&& ecuScript)
: broadcastServer_(ecuScript.getRequestId(), device, &udsServer_)
, udsServer_(ecuScript.getRequestId(), ecuScript.getResponseId(), device, move(ecuScript))
, broadcastServerThread_(&IsoTpSocket::readData, &broadcastServer_)
, udsServerThread_(&IsoTpSocket::readData, &udsServer_)
{
}

ECU::~ECU()
{
    udsServerThread_.join();
    broadcastServerThread_.join();
}

void ECU::testECU(const string &config_file, const string &device)
{
    // test ecu
    EcuLuaScript script("PCM", LUA_CONFIG_PATH + config_file);
    UdsServer tester(script.getResponseId(), script.getRequestId(), device, move(script));

    constexpr array<uint8_t, 3> ReadDataByIdentifier01 = {0x22, 0xf1, 0x90};
    constexpr array<uint8_t, 3> ReadDataByIdentifier02 = {0x22, 0xf1, 0x24};
    constexpr array<uint8_t, 3> ReadDataByIdentifier03 = {0x22, 0x1e, 0x23};
    constexpr array<uint8_t, 3> ReadDataByIdentifier04 = {0x22, 0x00, 0x00};
    constexpr array<uint8_t, 2> security_access01 = {0x27, 0x01};
    constexpr array<uint8_t, 4> security_access02 = {0x27, 0x02, 0x00, 0x00};
    constexpr array<uint8_t, 2> diagnostic_session01 = {0x10, 0x01};

    tester.sendData(ReadDataByIdentifier01.data(), ReadDataByIdentifier01.size());
    usleep(2000);
    tester.sendData(ReadDataByIdentifier02.data(), ReadDataByIdentifier02.size());
    usleep(2000);
    tester.sendData(ReadDataByIdentifier03.data(), ReadDataByIdentifier03.size());
    usleep(2000);
    tester.sendData(ReadDataByIdentifier04.data(), ReadDataByIdentifier04.size());
    usleep(2000);
    tester.sendData(security_access01.data(), security_access01.size());
    usleep(2000);
    tester.sendData(security_access02.data(), security_access01.size());
    usleep(2000);
    tester.sendData(diagnostic_session01.data(), diagnostic_session01.size());
    usleep(2000);
}


