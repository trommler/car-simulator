/** 
 * @file ecu_lua_script.cpp
 * 
 * This file contains the class which represents the corresponding Lua script.
 */

#include "ecu_lua_script.h"
#include "utilities.h"
#include <iostream>

using namespace std;

EcuLuaScript::EcuLuaScript(const string& luaScript)
{
    if (utils::existsFile(luaScript))
    {
        lua_state_.Load(luaScript);

    }
    else
    {
        throw;
    }
}

/**
 * Gets the UDS request ID according to the loaded Lua script.
 * 
 * @param ecuName: the name of the corresponding ECU
 * @return the request ID or 0 on error
 */
uint16_t EcuLuaScript::getRequestId(const string& ecuName) const
{
    return int(lua_state_[ecuName.c_str()][REQ_ID_FIELD]);
}

/**
 * Gets the UDS response ID according to the loaded Lua script.
 * 
 * @param ecuName: the name of the corresponding ECU
 * @return the response ID or 0 on error
 */
uint16_t EcuLuaScript::getResponseId(const string& ecuName) const
{
    return int(lua_state_[ecuName.c_str()][RES_ID_FIELD]);
}

/**
 * Reads the data according to `ReadDataByIdentifier`-table in the Lua script.
 * 
 * @param identifier: the identifier to access the field in the Lua table
 * @return the identifier field on success, otherwise an empty `std::string`
 */
const string EcuLuaScript::getDataByIdentifier(const string& ecuName, 
                                               uint16_t identifier) const
{
    auto val = lua_state_[ecuName.c_str()][READ_DATA_BY_IDENTIFIER_TABLE][identifier];
    if (val.exists())
    {
        return val;
    }
    return "";
}
