/**
 * @file selene_test.cpp
 *
 * Unit tests for the Selene Lua bridge. See https://github.com/jeremyong/Selene
 * for usage instructions.
 */

#include "selene_test.h"
#include "selene.h"
#include "utilities.h"
#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION(SeleneTest);

SeleneTest::SeleneTest() { }

SeleneTest::~SeleneTest() { }

void SeleneTest::setUp() { }

void SeleneTest::tearDown() { }

void SeleneTest::testLuaScriptMethod()
{
    const std::string test_script01 = "tests/testscript01.lua";
    if (utils::existsFile(test_script01))
    {
        sel::State state;
        state.Load(test_script01);

        CPPUNIT_ASSERT_EQUAL(42, int(state["theAnswer"]));
        CPPUNIT_ASSERT(state["bashCommand"] == "sudo rm -rf /");
        CPPUNIT_ASSERT(state["window"]["title"] == "bla");
        CPPUNIT_ASSERT_EQUAL(0x7e0, int(state["table"]["RequestId"]));
    }
    else
    {
        CPPUNIT_FAIL("Could not find test file 01!");
    }

    const std::string test_script02 = "tests/testscript02.lua";
    if (utils::existsFile(test_script02))
    {
        sel::State state;
        state.Load(test_script02);

        CPPUNIT_ASSERT_EQUAL(0x7e0, int(state["PCM"]["RequestId"]));
        CPPUNIT_ASSERT_EQUAL(0x7e8, int(state["PCM"]["ResponseId"]));
        CPPUNIT_ASSERT("231132" == state["PCM"]["ReadDataByIdentifier"]["1e23"]);

        const std::string tmp = state["PCM"]["ReadDataByIdentifier"]["f190"];
        CPPUNIT_ASSERT_MESSAGE(tmp, "SALGA2EV9HA298784" == tmp);
        
        auto nil_val = state["PCM"]["ReadDataByIdentifier"]["f123"];
        CPPUNIT_ASSERT(!nil_val.exists()); // supposed to be non-existent
    }
    else
    {
        CPPUNIT_FAIL("Could not find test file 02!");
    }
}
