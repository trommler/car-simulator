/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   newtestclass.cpp
 * Author: liu
 *
 * Created on 16.05.2017, 13:52:28
 */

#include "newtestclass.h"
//#include "/home/liu/NetBeansProjects/CppApplication_6/Complex.h"

CPPUNIT_TEST_SUITE_REGISTRATION(newtestclass);

Complex *m1, *m2, *m3, *m4;
newtestclass::newtestclass() {
}

newtestclass::~newtestclass() {
}
//SetUp(), tearDown() make the test individual from the others
void newtestclass::setUp() {
    m1 = new Complex(10,1);
    m2 = new Complex(0,0);
    m3 = new Complex(3,1);
    m4 = new Complex(13,2);
}

void newtestclass::tearDown() {
    delete m1;
    delete m2;
    delete m3;
    delete m4;
}

void newtestclass::testEqual() {
    CPPUNIT_ASSERT(!(*m1 == *m2));
    CPPUNIT_ASSERT(!(*m2 == *m3));
}

void newtestclass::testAddition(){
    CPPUNIT_ASSERT(*m1 + *m3 == *m4);
}

/*void newtestclass::testDividedByZeroThrows(){
    *m1/(*m2);
}*/


