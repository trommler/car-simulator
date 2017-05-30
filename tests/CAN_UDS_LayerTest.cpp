/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   CAN_UDS_LayerTest.cpp
 * Author: liu
 *
 * Created on 30.05.2017, 16:24:11
 */

#include "CAN_UDS_LayerTest.h"


CPPUNIT_TEST_SUITE_REGISTRATION(CAN_UDS_LayerTest);

CAN_UDS_LayerTest::CAN_UDS_LayerTest() {
}

CAN_UDS_LayerTest::~CAN_UDS_LayerTest() {
}

void CAN_UDS_LayerTest::setUp() {
    l1 = new CAN_UDS_Layer();
    l2 = new CAN_UDS_Layer();
    l3 = new CAN_UDS_Layer();
    l4 = new CAN_UDS_Layer();
}

void CAN_UDS_LayerTest::tearDown() {
    delete l1;
    delete l2;
    delete l3;
    delete l4;
}

void CAN_UDS_LayerTest::testsendCANmessage() {
    //does single frame on the server side, when it responses to Tester, really exist?
    //Because i havent found any evidence of this type responses from the documents

    //case 1:
    //single frame: byte0 + SID + 5 Data Bytes
    unsigned char send[7] = {0x07,0xF1,0x22,0x33,0x44,0x55,0x66};
    l1->sendCANmessage(send,7);
    CPPUNIT_ASSERT(l1->sendframe.can_id == 0x200);
    CPPUNIT_ASSERT(l1->sendframe.can_dlc == 7);
    CPPUNIT_ASSERT(std::equal(std::begin(send),std::end(send),std::begin(l1->sendframe.data)));
    
    //single frame: byte0 + SID + 6Data Bytes
    unsigned char send1[8] = {0x07,0xF1,0x22,0x33,0x44,0x55,0x66,0x77};
    l1->sendCANmessage(send,8);
    CPPUNIT_ASSERT(l1->sendframe.can_id == 0x200);
    CPPUNIT_ASSERT(l1->sendframe.can_dlc == 8);
    CPPUNIT_ASSERT(std::equal(std::begin(send),std::end(send),std::begin(l1->sendframe.data)));
    
    //case 2:
    //first frame is sent:
    unsigned char send_ff[8] = {0x18,0x10,0x08,0xC6,0x01,0x00,0x06,0x01};
    l2->sendCANmessage(send_ff,8);
    CPPUNIT_ASSERT(l2->sendframe.can_id == 0x200);
    CPPUNIT_ASSERT(l2->sendframe.can_dlc == 8);
    CPPUNIT_ASSERT(std::equal(std::begin(send_ff),std::end(send_ff),std::begin(l2->sendframe.data)));
    
    //consecutive frame is sent:
    unsigned char send_cf[4] = {0x23,0x19,0x01,0x01};
    l2->sendCANmessage(send_cf,4);
    CPPUNIT_ASSERT(l2->sendframe.can_id == 0x200);
    CPPUNIT_ASSERT(l2->sendframe.can_dlc == 4);
    CPPUNIT_ASSERT(std::equal(std::begin(send_cf),std::end(send_cf),std::begin(l2->sendframe.data)));

}




