/**
 * @file electronic_control_unit_test.h
 *
 */

#ifndef ELECTRONIC_CONTROL_UNIT_TEST_H
#define ELECTRONIC_CONTROL_UNIT_TEST_H

#include <cppunit/extensions/HelperMacros.h>

class electronic_control_unit_test : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(electronic_control_unit_test);

    CPPUNIT_TEST(testElectronicControlUnit);

    CPPUNIT_TEST_SUITE_END();

public:
    electronic_control_unit_test();
    virtual ~electronic_control_unit_test();
    void setUp();
    void tearDown();

private:
    void testElectronicControlUnit();

};

#endif /* ELECTRONIC_CONTROL_UNIT_TEST_H */

