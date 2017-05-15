/**
 * @file LuaScriptTest.h
 * @author Florian Bauer
 *
 * Created on 15.05.2017, 16:30:27
 */

#ifndef LUASCRIPTTEST_H
#define LUASCRIPTTEST_H

#include <cppunit/extensions/HelperMacros.h>

class LuaScriptTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(LuaScriptTest);

    CPPUNIT_TEST(testImport_script);
    CPPUNIT_TEST(testExecuteScript);
    CPPUNIT_TEST(testGetGlobalInt);

    CPPUNIT_TEST_SUITE_END();

public:
    LuaScriptTest();
    virtual ~LuaScriptTest();
    void setUp();
    void tearDown();

private:
    void testImport_script();
    void testExecuteScript();
    void testGetGlobalInt();

};

#endif /* LUASCRIPTTEST_H */

