/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   SocketCANTest.h
 * Author: liu
 *
 * Created on 30.05.2017, 16:08:47
 */

#ifndef SOCKETCANTEST_H
#define SOCKETCANTEST_H

#include "../SocketCAN.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <cppunit/extensions/HelperMacros.h>

class SocketCANTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(SocketCANTest);

    CPPUNIT_TEST(testinitSocketCAN_driver);
    CPPUNIT_TEST(testset_CANsendID);
    CPPUNIT_TEST(testreceiveallCANframe);

    CPPUNIT_TEST_SUITE_END();

public:
    SocketCANTest();
    virtual ~SocketCANTest();
    void setUp();
    void tearDown();
    void testinitSocketCAN_driver();
    void testset_CANsendID();
    void testreceiveallCANframe();
    
    
private:
    SocketCAN *s1,*s2,*s3,*s4,*s5;
};

#endif /* SOCKETCANTEST_H */

