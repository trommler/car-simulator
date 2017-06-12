/**
 * @file ECU.cpp
 *
 */

#include "ECU.h"
#include "isotp_socket.h"
#include <array>
#include <unistd.h>

using namespace std;

ECU::ECU(const string& device, EcuLuaScript&& ecuScript)
: uds_server_(ecuScript.getRequestId(), ecuScript.getResponseId(), device, move(ecuScript))
{
    thread* t = new thread(&IsoTpSocket::readData, &uds_server_);
    p_server_thread_ = std::make_unique<thread>(move(*t));
}

ECU::~ECU()
{
    if (p_server_thread_ != nullptr)
    {
        p_server_thread_->join();
    }
}

void ECU::initECU(string device)
{
    // todo implement more than 0 ecu
    if (p_server_thread_ == nullptr)
    {
        thread* t = new thread(&IsoTpSocket::readData, &uds_server_);
        p_server_thread_ = make_unique<thread>(move(*t));
    }
    usleep(5000);

    // test ecu
    /*
    UdsServer tester(0x200,0x100, device.c_str(), PATH_TO_LUA);
    
    constexpr array<uint8_t, 3> ReadDataByIdentifier01 = { 0x22, 0xf1, 0x90 };
    constexpr array<uint8_t, 3> ReadDataByIdentifier02 = { 0x22, 0xf1, 0x24 };
    constexpr array<uint8_t, 3> ReadDataByIdentifier03 = { 0x22, 0x1e, 0x23 };
    constexpr array<uint8_t, 3> ReadDataByIdentifier04 = { 0x22, 0x00, 0x00 };
    constexpr array<uint8_t, 2> security_access01 = { 0x27, 0x01 };
    constexpr array<uint8_t, 4> security_access02 = { 0x27, 0x02, 0x00, 0x00 };
    constexpr array<uint8_t, 2> diagnostic_session01 = { 0x10, 0x01 };
    
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
    
     */

    // ECU::testECU();
    //    p_server_thread_->join();
}

void ECU::testECU(string device)
{
    // test ecu
    EcuLuaScript script("PCM", PATH_TO_LUA);
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


