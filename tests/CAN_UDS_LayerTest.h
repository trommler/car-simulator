/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   CAN_UDS_LayerTest.h
 * Author: liu
 *
 * Created on 30.05.2017, 16:24:11
 */

#ifndef CAN_UDS_LAYERTEST_H
#define CAN_UDS_LAYERTEST_H

#include "../CAN_UDS_Layer.h"
#include <algorithm>
#include <iterator>
#include <cppunit/extensions/HelperMacros.h>

class CAN_UDS_LayerTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(CAN_UDS_LayerTest);

    CPPUNIT_TEST(testsendCANmessage);
    

    CPPUNIT_TEST_SUITE_END();

public:
    CAN_UDS_LayerTest();
    virtual ~CAN_UDS_LayerTest();
    void setUp();
    void tearDown();
    void testsendCANmessage();
    
private:
    CAN_UDS_Layer *l1,*l2,*l3,*l4;
};

#endif /* CAN_UDS_LAYERTEST_H */

