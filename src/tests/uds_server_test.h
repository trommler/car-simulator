/**
 * @file uds_server_test.h
 *
 */

#ifndef UDS_SERVER_TEST_H
#define UDS_SERVER_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "isotp_socket.h"

class TestReceiver : public IsoTpSocket
{
public:
    TestReceiver() = delete;

    TestReceiver(canid_t source,
                 canid_t dest,
                 const std::string& device)
    : IsoTpSocket(source, dest, device)
    {
    }
    virtual ~TestReceiver() = default;

    void setExpectedUdsRespData(const uint8_t* expBufferData,
                                const size_t expNumBytes) noexcept;

private:
    uint8_t m_expBufferData[4096];
    size_t m_expNumBytes;

    void proceedReceivedData(const uint8_t* buffer,
                             const size_t num_bytes) noexcept override;

};

class UdsServerTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(UdsServerTest);

    CPPUNIT_TEST(testUdsReadDataByIdentifier);

    CPPUNIT_TEST_SUITE_END();

public:
    UdsServerTest();
    virtual ~UdsServerTest();
    void setUp();
    void tearDown();

private:
    void testUdsServer();
    void testUdsReadDataByIdentifier();

};

#endif /* UDS_SERVER_TEST_H */

