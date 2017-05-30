/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   SocketCANTest.cpp
 * Author: liu
 *
 * Created on 30.05.2017, 16:08:47
 */

#include "SocketCANTest.h"


CPPUNIT_TEST_SUITE_REGISTRATION(SocketCANTest);


SocketCANTest::SocketCANTest() {
}

SocketCANTest::~SocketCANTest() {
}

void SocketCANTest::setUp() {
    s1 = new SocketCAN();
    s2 = new SocketCAN();
    s3 = new SocketCAN();
    s4 = new SocketCAN();
    s5 = new SocketCAN();
}

void SocketCANTest::tearDown() {
    delete s1;
    delete s2;
    delete s3;
    delete s4;
    delete s5;
}

void SocketCANTest::testinitSocketCAN_driver() {
    //examine the interface which has not been set up
    CPPUNIT_ASSERT_THROW(s1->initSocketCAN_driver("vcan2"),std::invalid_argument);
    //examine the interface which has alreayd been set up
    CPPUNIT_ASSERT_NO_THROW(s2->initSocketCAN_driver("vcan0"));
}

void SocketCANTest::testset_CANsendID(){
    s2->set_CANsendID(0x7FF);
    CPPUNIT_ASSERT(s2->send_id == 0x7FF);
    
    CPPUNIT_ASSERT_THROW(s2->set_CANsendID(0x8FF),std::out_of_range);
    CPPUNIT_ASSERT_THROW(s2->set_CANsendID(0xFFF),std::out_of_range);
}

void SocketCANTest::testreceiveallCANframe(){
    struct can_frame testframe;
    __u8 data[8] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
    testframe = {0x123,8,0,0,0,data[8]};
    write(s3->skt,&testframe,sizeof(testframe));
    s3->receiveallCANframe();
    CPPUNIT_ASSERT(std::equal(std::begin(data),std::end(data),std::begin(s3->can_data)));
    CPPUNIT_ASSERT(testframe.can_id == s3->receiveframe.can_id);
}

