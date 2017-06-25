/** 
 * @file ecu_lua_script.cpp
 * 
 * This file contains the class which represents the corresponding Lua script.
 */

#include "ecu_lua_script.h"
#include "utilities.h"
#include <iostream>
#include <algorithm>
#include <exception>
#include <unistd.h>

using namespace std;

/**
 * Constructor. 
 * 
 * @param ecuIdent: the identifier name for the ECU (e.g. "PCM")
 * @param luaScript: the path to the Lua script
 */
EcuLuaScript::EcuLuaScript(const string& ecuIdent, const string& luaScript)
{
    if (utils::existsFile(luaScript))
    {
        lua_state_.Load(luaScript);
        if (lua_state_[ecuIdent.c_str()].exists())
        {
            ecu_ident_ = ecuIdent;
            return;
        }
        else
        {
            using sel::State;
            lua_state_.~State();
        }
    }
    throw exception();
}

/**
 * Gets the UDS request ID according to the loaded Lua script.
 * 
 * @return the request ID or 0 on error
 */
uint16_t EcuLuaScript::getRequestId() const
{
    return int(lua_state_[ecu_ident_.c_str()][REQ_ID_FIELD]);
}

/**
 * Gets the UDS response ID according to the loaded Lua script.
 * 
 * @return the response ID or 0 on error
 */
uint16_t EcuLuaScript::getResponseId() const
{
    return int(lua_state_[ecu_ident_.c_str()][RES_ID_FIELD]);
}

/**
 * Reads the data according to `ReadDataByIdentifier`-table in the Lua script.
 * 
 * @param identifier: the identifier to access the field in the Lua table
 * @return the identifier field on success, otherwise an empty std::string
 */
string EcuLuaScript::getDataByIdentifier(uint16_t identifier) const
{
    auto val = lua_state_[ecu_ident_.c_str()][READ_DATA_BY_IDENTIFIER_TABLE][identifier];
    if (val.exists())
    {
        return val;
    }
    return "";
}

string EcuLuaScript::Session_getDataByIdentifier(std::string session, std::uint16_t identifier) const
{
    auto val = lua_state_[ecu_ident_.c_str()][session][READ_DATA_BY_IDENTIFIER_TABLE][identifier];
    if(val.exists())
    {
        return val;
    }
    return "";
}

string EcuLuaScript::getSeed(uint8_t seed_level) const
{
    auto val = lua_state_[ecu_ident_.c_str()][READ_SEED][seed_level];
    if (val.exists())
    {
        return val;
    }
    return "";
}

/**
 * Converts a literal hex string into a value vector.
 * 
 * @param hexString: the literal hex string (e.g. "41 6f 54")
 * @return a vector with the byte values
 */
vector<uint8_t> EcuLuaScript::literalHexStrToBytes(string& hexString)
{
    // remove white spaces from string
    hexString.erase(remove(hexString.begin(), hexString.end(), ' '), hexString.end());
    vector<uint8_t> data;
    for (unsigned int i = 0; i < hexString.length(); i += 2)
    {
        string byteString = hexString.substr(i, 2);
        uint8_t byte = static_cast<uint8_t> (strtol(byteString.c_str(), NULL, 16));
        data.push_back(byte);
    }
    return data;
}

/**
 * Convert the given string into another string that represents the hex bytes of 
 * the input string. This is a convenience function to use ascii strings in 
 * responses.
 * 
 * Example: 
 *     `ascii("Hello")` -> `" 48 65 6C 6C 6F "`
 * 
 * @param utf8_str: the input string to convert
 */
std::string EcuLuaScript::ascii(const std::string& utf8_str) const noexcept
{
    static constexpr char lut[] = "0123456789ABCDEF";
    const size_t len = utf8_str.length();
    if (len == 0)
    {
        return "";
    }

    std::string output;
    output.reserve(len * 3 + 1); // str length * (1 whitespace + 2 characters per byte) + last whitespace
    unsigned char c;
    for (size_t i = 0; i < len; i++)
    {
        c = utf8_str[i];
        output.push_back(' ');
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    output.push_back(' ');
    return output;
}

/**
 * Convert the given integer value into a hex byte string as used in requests 
 * and responses. The parameter `len` gives the number of bytes that should be 
 * returned.
 * 
 * Examples:
 *     `toByteResponse(2, 13248)` -> `"33 C0"`
 *     `toByteResponse(3, 13248)` -> `"00 33 C0"`
 * 
 * @param len: the length in bytes
 * @param value: the numeric value to send (e.g. `123`, `0xff`)
 */
std::string EcuLuaScript::toByteResponse(std::size_t len, long value) const
{
    // TODO: implement
    return "";
}

/**
 * Send the given response (string of hex bytes) immediately.
 * 
 * @param response: the raw response message (e.g. "DE AD C0 DE")
 */
void EcuLuaScript::sendRaw(const std::string& response) const
{
    // TODO: implement
}

/**
 * Suspend the script for the given number of milliseconds.
 * 
 * @param ms: time to sleep in milliseconds
 */
void EcuLuaScript::sleep(unsigned int ms) const
{
    usleep(ms * 1000);
}

/**
 * Returns the currently active diagnostic session to be used in custom 
 * functions.
 */
int EcuLuaScript::getCurrentSession() const
{
    // TODO: implement
    return 0;
}

/**
 * Switch to the given (numeric) diagnostic session.
 * 
 * @param ses: the session ID (e.g. `0x01` = DEFAULT, `0x02` = PROGRAMMING)
 */
void EcuLuaScript::switchToSession(int ses)
{
    // TODO: implement
}