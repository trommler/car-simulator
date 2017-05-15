/**
 * @file LuaScriptTest.cpp
 * @author Florian Bauer 
 * 
 * UnitTests for the Lua script wrapper class.
 */

#include "LuaScriptTest.h"
#include "LuaScript.h"
#include <cstring>
#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION(LuaScriptTest);

static const std::string TEST_SCRIPT = "tests/testscript.lua";

LuaScriptTest::LuaScriptTest() { }

LuaScriptTest::~LuaScriptTest() { }

void LuaScriptTest::setUp() { }

void LuaScriptTest::tearDown() { }

void LuaScriptTest::testImport_script()
{
    const std::string& scriptFile = TEST_SCRIPT;
    // since the c'tor is private, we have to use this awkward workaround for testing
    LuaScript* empty = static_cast<LuaScript*> (malloc(sizeof(LuaScript)));
    memset(empty, 0, sizeof(LuaScript));

    LuaScript* result = static_cast<LuaScript*> (malloc(sizeof(LuaScript)));
    memset(result, 0, sizeof(LuaScript));

    LuaScript tmp = LuaScript::import_script(scriptFile);
    memcpy(result, &tmp, sizeof(LuaScript));

    if (memcmp(empty, result, sizeof(LuaScript)) == 0)
    {
        CPPUNIT_ASSERT_MESSAGE("Constructed object is empty!", false);
    }

    free(empty);
    free(result);
}

void LuaScriptTest::testExecuteScript()
{
    LuaScript script = LuaScript::import_script(TEST_SCRIPT);
    int result = script.executeScript();
    CPPUNIT_ASSERT(result == 0);
}

void LuaScriptTest::testGetGlobalInt()
{
    const std::string varName = "theAnswer";
    LuaScript script = LuaScript::import_script(TEST_SCRIPT);
    script.executeScript();
    int result = script.getGlobalInt(varName);
    CPPUNIT_ASSERT(result == 42);
}

