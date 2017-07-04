/**
 * @file ecu_lua_script.cpp
 *
 * This file contains the class which represents the corresponding Lua script.
 */

#include "ecu_lua_script.h"
#include "utilities.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <unistd.h>
#include <climits>

using namespace std;

/// Look-up table for (uppercase) hexadecimal digits [0..F].
static constexpr char HEX_LUT[] = "0123456789ABCDEF";

/// Defines the maximum size of an UDS message in bytes.
static constexpr int MAX_UDS_SIZE = 4096;

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
        // inject the C++ functions into the Lua script
        lua_state_["ascii"] = &ascii;
        lua_state_["toByteResponse"] = &toByteResponse;
        lua_state_["sleep"] = &sleep;

        lua_state_.Load(luaScript);
        if (lua_state_[ecuIdent.c_str()].exists())
        {
            ecu_ident_ = ecuIdent;
            return;
        }
    }
    throw invalid_argument("No, or invalid Lua script!");
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
 * @return the identifier field on success, otherwise an empty string
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

/**
 * Overload with additional sessions handling.
 *
 * @param identifier: the identifier to access the field in the Lua table
 * @param session: the session as string (e.g. "Programming")
 * @return the identifier field on success, otherwise an empty string
 */
string EcuLuaScript::getDataByIdentifier(uint16_t identifier, const string& session) const
{
    auto val = lua_state_[ecu_ident_.c_str()][session][READ_DATA_BY_IDENTIFIER_TABLE][identifier];
    if (val.exists())
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
vector<uint8_t> EcuLuaScript::literalHexStrToBytes(const string& hexString) const
{
    // make a working copy
    string tmpStr = hexString;
    // remove white spaces from string
    tmpStr.erase(remove(tmpStr.begin(), tmpStr.end(), ' '), tmpStr.end());
    vector<uint8_t> data;
    // plus `% 2` just in case of a "odd" byte number
    data.reserve(tmpStr.length() / 2 + (tmpStr.length() % 2));
    string byteString;
    uint8_t byte;
    for (size_t i = 0; i < tmpStr.length(); i += 2)
    {
        byteString = tmpStr.substr(i, 2);
        byte = static_cast<uint8_t> (strtol(byteString.c_str(), NULL, 16));
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
 * @return a literal string of hex bytes (e.g. " 12 4A FF ") or an empty string
 * on error
 *
 * @note To allow a seamless string concatenation, the returned string always
 * begins and ends with an whitespace.
 */
string EcuLuaScript::ascii(const string& utf8_str) noexcept
{
    const size_t len = utf8_str.length();
    if (len == 0)
    {
        return "";
    }

    string output;
    // str length * (1 whitespace + 2 characters per byte) + last whitespace
    output.reserve(len * 3 + 1);
    unsigned char c;
    for (size_t i = 0; i < len; ++i)
    {
        c = utf8_str[i];
        output.push_back(' ');
        output.push_back(HEX_LUT[c >> 4]);
        output.push_back(HEX_LUT[c & 0x0F]);
    }
    output.push_back(' ');
    return output;
}

/**
 * Convert the given unsigned value into a hex byte string as used in requests
 * and responses. The parameter `len` [0..4096] gives the number of bytes that
 * gets returned. In case `len` equals 0 or a empty string is returned.
 *
 * Examples:
 *     `toByteResponse(13248, 2)` -> `"33 C0"`
 *     `toByteResponse(13248, 3)` -> `"00 33 C0"`
 *     `toByteResponse(13248, 1)` -> `"C0"`
 *     `toByteResponse(13248)` -> `"00 00 00 00 00 00 33 C0"`
 *
 * @param value: the numeric value to send (e.g. `123`, `0xff`)
 * @param len: the length in bytes [default = sizeof(unsigned long) -> 8 on x64]
 */
string EcuLuaScript::toByteResponse(unsigned long value,
                                    size_t len /* = sizeof(unsigned long) */) noexcept
{
    if (len > MAX_UDS_SIZE)
    {
        len = MAX_UDS_SIZE;
    }

    static constexpr int CHAR_SP = 3; // character space for 2 hex digits + 1 whitespace
    const int zeroFill = len - sizeof(value);

    if (zeroFill < 0)
    {
        // truncated value
        const size_t space = len * CHAR_SP;
        char str[space];
        for (size_t i = 0, j = (len * 2 - 1) * 4; i < space; i += CHAR_SP, j -= 8)
        {
            str[i] = HEX_LUT[(value >> j) & 0x0F];
            str[i + 1] = HEX_LUT[(value >> (j - 4)) & 0x0F];
            str[i + 2] = ' ';
        }
        str[space - 1] = '\0';
        return str;
    }
    else
    {
        // fill up wit zeros
        const size_t space = ((sizeof(value) + zeroFill) * CHAR_SP);
        const size_t valSpace = zeroFill * CHAR_SP;
        char str[space];
        for (size_t i = 0; i < valSpace; i += CHAR_SP)
        {
            str[i] = '0';
            str[i + 1] = '0';
            str[i + 2] = ' ';
        }

        // insert hex values
        for (size_t i = valSpace, j = (sizeof(value) * 2 - 1) * 4; i < space; i += CHAR_SP, j -= 8)
        {
            str[i] = HEX_LUT[(value >> j) & 0x0F];
            str[i + 1] = HEX_LUT[(value >> (j - 4)) & 0x0F];
            str[i + 2] = ' ';
        }
        str[space - 1] = '\0';
        return str;
    }
    return "";
}

/**
 * Sends the given response (string of hex bytes) immediately.
 *
 * @param response: the raw response message to send (e.g. "DE AD C0 DE")
 */
void EcuLuaScript::sendRaw(const string& response) const
{
    // TODO: implement
}

/**
 * Suspend the script for the given number of milliseconds.
 *
 * @param ms: time to sleep in milliseconds
 */
void EcuLuaScript::sleep(unsigned int ms) noexcept
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

/**
 * Checks if the identifier is in the Raw-section of the lua script.
 *
 * @param identStr: the identifier string for the entry in the Lua "Raw"-table
 * @return true if identifier is in the raw section, false otherwise
 */
bool EcuLuaScript::hasRaw(const string &identStr) const
{
    auto val = lua_state_[ecu_ident_.c_str()][RAW_TABLE][identStr.c_str()];
    return val.exists();
}

/**
 * Gets the raw data entries form the Lua "Raw"-Table. All entries, as well as
 * the identifiers of the corresponding entries, are literal hex byte strings
 * (e.g. "12 FF 00").
 *
 * @param identStr: the identifier string for the entry in the Lua "Raw"-table
 * @return the raw data as literal hex byte string or an empty string on error
 */
string EcuLuaScript::getRaw(const string& identStr) const
{
    auto val = lua_state_[ecu_ident_.c_str()][RAW_TABLE][identStr.c_str()];

    if (val.isFunction())
    {
        cout << "is a function" << endl;
        return lua_state_[ecu_ident_.c_str()][RAW_TABLE][identStr.c_str()]();
    }
    else
    {
        cout << "is not a function" << endl;
        return val; // will be cast into string
    }
}
