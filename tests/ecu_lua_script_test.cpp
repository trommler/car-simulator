/**
 * @file ecu_lua_script_test.cpp
 *
 * Unit tests for the class `EcuLuaScript`.
 */

#include "ecu_lua_script_test.h"
#include "ecu_lua_script.h"

const std::string ECU_IDENT = "PCM";
const std::string LUA_SCRIPT = "tests/testscript03.lua";

CPPUNIT_TEST_SUITE_REGISTRATION(EcuLuaScriptTest);

EcuLuaScriptTest::EcuLuaScriptTest()
{
}

EcuLuaScriptTest::~EcuLuaScriptTest()
{
}

void EcuLuaScriptTest::setUp()
{
}

void EcuLuaScriptTest::tearDown()
{
}

void EcuLuaScriptTest::testEcuLuaScript()
{
    const std::string ecuIdent = ECU_IDENT;
    const std::string luaScript = LUA_SCRIPT;

    CPPUNIT_ASSERT_NO_THROW(EcuLuaScript ecuLuaScript(ecuIdent, luaScript));

    // these tests are supposed to fail
    CPPUNIT_ASSERT_THROW(EcuLuaScript ecuLuaScript(ecuIdent, "adsf"), std::exception);
    CPPUNIT_ASSERT_THROW(EcuLuaScript ecuLuaScript("hzlpfrt", luaScript), std::exception);
}

void EcuLuaScriptTest::testGetRequestId()
{
    EcuLuaScript ecuLuaScript(ECU_IDENT, LUA_SCRIPT);
    std::uint16_t result = ecuLuaScript.getRequestId();
    std::uint16_t expect = 0x100;
    CPPUNIT_ASSERT_EQUAL(expect, result);
}

void EcuLuaScriptTest::testGetResponseId()
{
    EcuLuaScript ecuLuaScript(ECU_IDENT, LUA_SCRIPT);
    std::uint16_t result = ecuLuaScript.getResponseId();
    std::uint16_t expect = 0x200;
    CPPUNIT_ASSERT_EQUAL(expect, result);
}

void EcuLuaScriptTest::testGetDataByIdentifier()
{
    EcuLuaScript ecuLuaScript(ECU_IDENT, LUA_SCRIPT);
    std::uint16_t identifier = 0xf190;
    std::string expect = "SALGA2EV9HA298784";
    std::string result = ecuLuaScript.getDataByIdentifier(identifier);
    CPPUNIT_ASSERT_EQUAL(expect, result);

    identifier = 0xf124;
    expect = "HPLA-12345-AB";
    result = ecuLuaScript.getDataByIdentifier(identifier);
    CPPUNIT_ASSERT_EQUAL(expect, result);

    identifier = 0x1e23;
    expect = "231132";
    result = ecuLuaScript.getDataByIdentifier(identifier);
    CPPUNIT_ASSERT_EQUAL(expect, result);

    // this is supposed to fail
    identifier = 0xf123;
    expect = "";
    result = ecuLuaScript.getDataByIdentifier(identifier);
    CPPUNIT_ASSERT_EQUAL(expect, result);
}

void EcuLuaScriptTest::testGetSeed()
{
    EcuLuaScript ecuLuaScript(ECU_IDENT, LUA_SCRIPT);
    std::uint8_t identifier = 0x01;
    std::string expect = "0x4455";
    std::string result = ecuLuaScript.getSeed(identifier);
    CPPUNIT_ASSERT_EQUAL(expect, result);

    identifier = 0x03;
    expect = "0x6677";
    result = ecuLuaScript.getSeed(identifier);
    CPPUNIT_ASSERT_EQUAL(expect, result);

    // this is supposed to fail
    identifier = 0x02;
    expect = "";
    result = ecuLuaScript.getSeed(identifier);
    CPPUNIT_ASSERT_EQUAL(expect, result);
}

void EcuLuaScriptTest::testLiteralHexStrToBytes()
{
    EcuLuaScript ecuLuaScript(ECU_IDENT, LUA_SCRIPT);
    std::string hexString = "48 65 6c 6c 6f"; // "Hello"
    std::array<std::uint8_t, 5> expect = {0x48, 0x65, 0x6c, 0x6c, 0x6f};
    std::vector<std::uint8_t> result = ecuLuaScript.literalHexStrToBytes(hexString);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Size mismatch!", expect.size(), result.size());

    for (unsigned i = 0; i < result.size(); i++)
    {
        CPPUNIT_ASSERT_EQUAL(expect.at(i), result.at(i));
    }

    hexString = " 48 65 6c 6c 6"; // odd byte number
    expect = {0x48, 0x65, 0x6c, 0x6c, 0x6};
    result = ecuLuaScript.literalHexStrToBytes(hexString);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Size mismatch!", expect.size(), result.size());

    for (unsigned i = 0; i < result.size(); i++)
    {
        CPPUNIT_ASSERT_EQUAL(expect.at(i), result.at(i));
    }

    hexString = " 48 6h 6c gg 6f "; // not hex -> character gets ignored
    expect = {0x48, 0x6, 0x6c, 0x0, 0x6f};
    result = ecuLuaScript.literalHexStrToBytes(hexString);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Size mismatch!", expect.size(), result.size());

    for (unsigned i = 0; i < result.size(); i++)
    {
        CPPUNIT_ASSERT_EQUAL(expect.at(i), result.at(i));
    }
}

void EcuLuaScriptTest::testAscii()
{
    EcuLuaScript ecuLuaScript(ECU_IDENT, LUA_SCRIPT);
    std::string inputStr = "Hello";
    std::string expect = " 48 65 6C 6C 6F ";
    std::string result;

    result = ecuLuaScript.ascii(inputStr);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Size mismatch!", expect.size(), result.size());

    for (unsigned i = 0; i < result.size(); i++)
    {
        CPPUNIT_ASSERT_EQUAL(expect.at(i), result.at(i));
    }

    inputStr = "";
    expect = "";
    result = ecuLuaScript.ascii(inputStr);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Size mismatch!", expect.size(), result.size());

    for (unsigned i = 0; i < result.size(); i++)
    {
        CPPUNIT_ASSERT_EQUAL(expect.at(i), result.at(i));
    }

    inputStr = "Möp™®"; // stressing the function with Unicode chars
    expect = " 4D C3 B6 70 E2 84 A2 C2 AE ";
    result = ecuLuaScript.ascii(inputStr);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Size mismatch!", expect.size(), result.size());

    for (unsigned i = 0; i < result.size(); i++)
    {
        CPPUNIT_ASSERT_EQUAL(expect.at(i), result.at(i));
    }
}

