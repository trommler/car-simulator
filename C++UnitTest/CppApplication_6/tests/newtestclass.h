/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   newtestclass.h
 * Author: liu
 *
 * Created on 16.05.2017, 13:52:27
 */

#ifndef NEWTESTCLASS_H
#define NEWTESTCLASS_H

#include <cppunit/extensions/HelperMacros.h>
#include "/home/liu/NetBeansProjects/CppApplication_6/Complex.h"

class newtestclass : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(newtestclass);
    
    //TestSuite: which you can run your tests at one time
    CPPUNIT_TEST(testEqual);
    CPPUNIT_TEST(testAddition);
    //CPPUNIT_TEST_EXCEPTION(testDividedByZeroThrows,std::overflow_error);

    CPPUNIT_TEST_SUITE_END();

public:
    newtestclass();
    virtual ~newtestclass();
    void setUp();
    void tearDown();

private:
    Complex *m1, *m2, *m3, *m4;
    void testEqual();
    void testAddition();
    //void testDividedByZeroThrows();
};

#endif /* NEWTESTCLASS_H */

