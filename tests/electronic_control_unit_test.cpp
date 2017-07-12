/**
 * @file electronic_control_unit_test.cpp
 *
 */

#include "electronic_control_unit_test.h"
#include "electronic_control_unit.h"
#include <array>
#include <cstdint>
#include <iostream>

const std::string ECU_IDENT = "PCM";
const std::string LUA_SCRIPT = "tests/test_config_dir/testscript06.lua";

CPPUNIT_TEST_SUITE_REGISTRATION(electronic_control_unit_test);

electronic_control_unit_test::electronic_control_unit_test()
{
}

electronic_control_unit_test::~electronic_control_unit_test()
{
}

void electronic_control_unit_test::setUp()
{
}

void electronic_control_unit_test::tearDown()
{
}

void electronic_control_unit_test::testElectronicControlUnit()
{
// TODO: create a passing test case

//    const std::string device = "vcan0";
//    EcuLuaScript ecuScript(ECU_IDENT, LUA_SCRIPT);
//    std::uint16_t requId = ecuScript.getRequestId();
//    std::uint16_t respId = ecuScript.getResponseId();
//    
//    std::cout << "requId: " << requId << ", respId: " << respId << std::endl;
//    IsoTpSender sender(respId, requId, device);
//
//    IsoTpReceiver receiver(respId, requId, device);
//    std::thread testThread(&IsoTpReceiver::readData, &receiver); // run async in thread
//    usleep(4000); // wait some time to ensure the thread is set up and running
//
//    std::array<std::uint8_t, 2> testData = {0x10, 0x02};
//
//    { 
//        CPPUNIT_ASSERT_NO_THROW(ElectronicControlUnit electronicControlUnit(device, std::move(ecuScript)));
//        sender.sendData(testData.data(), testData.size());
//    } // local scoped to check the destructor
//
//    usleep(4000);
//    receiver.closeReceiver();
//    testThread.join();
//    constexpr std::array<uint8_t, 1> eof = {0x00};
//    sender.sendData(eof.data(), eof.size());
}

