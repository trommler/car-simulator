/**
 * @file utils_test.h
 *
 */

#ifndef UTILSTEST_H
#define UTILSTEST_H

#include <cppunit/extensions/HelperMacros.h>

class UtilsTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(UtilsTest);

    CPPUNIT_TEST(testExistsFile);
    CPPUNIT_TEST(testExistsDirectory);

    CPPUNIT_TEST_SUITE_END();

public:
    UtilsTest();
    virtual ~UtilsTest();
    void setUp();
    void tearDown();

private:
    void testExistsFile();
    void testExistsDirectory();

};

#endif /* UTILSTEST_H */

