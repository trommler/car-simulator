/**
 * @file uds_server_test.cpp
 *
 * These test cases are used to ensure a proper UDS response from the 
 * `UdsServer`. Since this task is done via ISO-TP, ensure the kernel module for
 * virtual can is enabled (`sudo modprobe vcan`), the vcan anddress is added 
 * (`sudo ip link add dev vcan0 type vcan`) and set up (`sudo ip link set up 
 * vcan0`) before running these tests. 
 */

#include "uds_server_test.h"
#include "uds_server.h"
#include "service_identifier.h"
#include <unistd.h>
#include <cstring>

const std::string DEVICE = "vcan0";
const std::string ECU_IDENT = "PCM";
const std::string LUA_SCRIPT = "tests/testscript03.lua";

CPPUNIT_TEST_SUITE_REGISTRATION(UdsServerTest);

UdsServerTest::UdsServerTest()
{
}

UdsServerTest::~UdsServerTest()
{
}

void UdsServerTest::setUp()
{
}

void UdsServerTest::tearDown()
{
}

void UdsServerTest::testUdsReadDataByIdentifier()
{
    constexpr std::array<uint8_t, 3> readDataById01 = {0x22, 0xf1, 0x90};
    constexpr std::array<uint8_t, 3> readDataById02 = {0x22, 0xf1, 0x24};
    constexpr std::array<uint8_t, 3> readDataById03 = {0x22, 0xf1, 0x23};
    constexpr std::array<uint8_t, 3> readDataById04 = {0x22, 0x1e, 0x23};

    constexpr std::array<uint8_t, 20> expAnswer01 = {
        READ_DATA_BY_IDENTIFIER_RES, 0xf1, 0x90,
        'S', 'A', 'L', 'G', 'A', '2', 'E', 'V', '9', 'H', 'A', '2', '9', '8', '7', '8', '4'
    };

    constexpr std::array<uint8_t, 16> expAnswer02 = {
        READ_DATA_BY_IDENTIFIER_RES, 0xf1, 0x24,
        'H', 'P', 'L', 'A', '-', '1', '2', '3', '4', '5', '-', 'A', 'B'
    };

    constexpr std::array<uint8_t, 2> expAnswer03 = {
        ERROR, REQUEST_OUT_OF_RANGE
    };

    constexpr std::array<uint8_t, 9> expAnswer04 = {
        READ_DATA_BY_IDENTIFIER_RES, 0x1e, 0x23,
        '2', '3', '1', '1', '3', '2'
    };

    std::uint8_t* buffer;
    std::size_t num_bytes;
    EcuLuaScript script(ECU_IDENT, LUA_SCRIPT);

    TestReceiver testReceiver(script.getRequestId(), script.getResponseId(), DEVICE);
    testReceiver.openReceiver();

    UdsServer udsServer(script.getResponseId(), script.getRequestId(), DEVICE, std::move(script));
    std::thread testThread(&IsoTpSocket::readData, &testReceiver); // run async in thread
    usleep(4000); // wait some time to ensure the thread is set up and running

    testReceiver.setExpectedUdsRespData(expAnswer01.data(), expAnswer01.size());
    usleep(1000);
    buffer = (uint8_t*) readDataById01.data();
    num_bytes = readDataById01.size();
    udsServer.proceedReceivedData(buffer, num_bytes);

    usleep(2000);
    testReceiver.setExpectedUdsRespData(expAnswer02.data(), expAnswer02.size());
    usleep(1000);
    buffer = (uint8_t*) readDataById02.data();
    num_bytes = readDataById02.size();
    udsServer.proceedReceivedData(buffer, num_bytes);

    usleep(2000);
    testReceiver.setExpectedUdsRespData(expAnswer03.data(), expAnswer03.size());
    usleep(1000);
    buffer = (uint8_t*) readDataById03.data();
    num_bytes = readDataById03.size();
    udsServer.proceedReceivedData(buffer, num_bytes);

    usleep(2000);
    testReceiver.setExpectedUdsRespData(expAnswer04.data(), expAnswer04.size());
    usleep(1000);
    buffer = (uint8_t*) readDataById04.data();
    num_bytes = readDataById04.size();
    udsServer.proceedReceivedData(buffer, num_bytes);

    testReceiver.closeReceiver();
    udsServer.proceedReceivedData(buffer, num_bytes); // send some garbage to close the receiver
    testThread.join();
}

/**
 * Compares the incoming response data from the corresponding `UdsServer` with
 * the expected internal data set. This is done by the 
 * `setExpectedUdsRespData()`-function, so invoke this routine first and wait
 * some time, to ensure the comparing data is fully copied.
 * 
 * @param buffer: the buffer with the incoming data
 * @param num_bytes: the size of the incoming data in bytes
 * @see TestReceiver::setExpectedUdsRespData()
 */
void TestReceiver::proceedReceivedData(const uint8_t* buffer,
                                       const size_t num_bytes) noexcept
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Message size don't match!", m_expNumBytes, num_bytes);

    for (size_t i = 0; i < num_bytes; i++)
    {
        CPPUNIT_ASSERT_EQUAL(m_expBufferData[i], buffer[i]);
    }
}

/**
 * Copies the expected UDS response message into the internal member fields.
 * After that, the overridden `proceedReceiveData()`-function will compare all
 * receiving results form the `UdsServer`-instance with the provided data set.
 * Use this function before calling `sendData()` or `proceedReceivedData()`
 * of the `UdsServer`-instance and wait some time, since this is a async task.
 * 
 * @param expBufferData: the expected UDS message response from the `UdsServer`
 * @param expNumBytes: the expected UDS message length form the `UdsServer`
 * @see TestReceiver::proceedReceivedData()
 */
void TestReceiver::setExpectedUdsRespData(const uint8_t* expBufferData,
                                          const size_t expNumBytes) noexcept
{
    m_expNumBytes = expNumBytes;
    memcpy(m_expBufferData, expBufferData, expNumBytes);
}
