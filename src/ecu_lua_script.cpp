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
        request_id_ = int(lua_state_[REQ_ID_FIELD]);
        response_id_ = int(lua_state_[RES_ID_FIELD]);
    }
    else
    {
        throw;
    }
}

/**
 * Gets the UDS request ID according to the loaded Lua script.
 * 
 * @return the request ID or 0 on error
 */
uint16_t EcuLuaScript::getRequestId() const
{
    return request_id_;
}

/**
 * Gets the UDS response ID according to the loaded Lua script.
 * 
 * @return the response ID or 0 on error
 */
uint16_t EcuLuaScript::getResponseId() const
{
    return response_id_;
}

/**
 * Reads the data according to `ReadDataByIdentifier`-table in the Lua script.
 * 
 * @param identifier: the identifier to access the field in the Lua table
 * @return the identifier field on success, otherwise an empty `std::string`
 */
const string EcuLuaScript::getDataByIdentifier(uint16_t identifier) const
{
    auto val = lua_state_[READ_DATA_BY_IDENTIFIER_FIELD][identifier];
    if (val.exists())
    {
        return val;
    }
    return "";
}
