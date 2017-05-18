/**
 * @file: SeleneTest.cpp
 * @author: Florian Bauer
 *
 * Unit tests for the Selene Lua bridge. See https://github.com/jeremyong/Selene for usage 
 * instructions.
 */

#include "SeleneTest.h"
#include "selene.h"
#include "Utilities.h"
#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION(SeleneTest);

SeleneTest::SeleneTest() { }

SeleneTest::~SeleneTest() { }

void SeleneTest::setUp() { }

void SeleneTest::tearDown() { }

void SeleneTest::testLuaScriptMethod()
{
    const std::string testScript01 = "tests/testscript01.lua";
    if (utils::existsFile(testScript01))
    {
        sel::State state;
        state.Load(testScript01);

        CPPUNIT_ASSERT_EQUAL(42, int(state["theAnswer"]));
        CPPUNIT_ASSERT(state["bashCommand"] == "sudo rm -rf /");
        CPPUNIT_ASSERT(state["window"]["title"] == "bla");
        CPPUNIT_ASSERT_EQUAL(0x7e0, int(state["table"]["RequestId"]));
    }
    else
    {
        CPPUNIT_ASSERT_MESSAGE("Could not find test file 01!", false);
    }

    const std::string testScript02 = "tests/testscript02.lua";
    if (utils::existsFile(testScript02))
    {
        sel::State state;
        state.Load(testScript02);

        CPPUNIT_ASSERT_EQUAL(0x7e0, int(state["PCM"]["RequestId"]));
        CPPUNIT_ASSERT_EQUAL(0x7e8, int(state["PCM"]["ResponseId"]));
        CPPUNIT_ASSERT("231132" == state["PCM"]["ReadDataByIdentifier"]["1e23"]);

        const std::string tmp = state["PCM"]["ReadDataByIdentifier"]["f190"];
        CPPUNIT_ASSERT_MESSAGE(tmp, "SALGA2EV9HA298784" == tmp);
        
        auto nilVal = state["PCM"]["ReadDataByIdentifier"]["f123"];
        CPPUNIT_ASSERT(!nilVal.exists()); // supposed to be non-existent
    }
    else
    {
        CPPUNIT_ASSERT_MESSAGE("Could not find test file 02!", false);
    }
}
