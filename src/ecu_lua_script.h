/** 
 * @file: ecu_lua_script.h
 *
 */

#ifndef ECU_LUA_SCRIPT_H
#define ECU_LUA_SCRIPT_H

#include "selene.h"
#include "lua.hpp"
#include <string>
#include <cstdint>

constexpr char REQ_ID_FIELD[] = "RequestId";
constexpr char RES_ID_FIELD[] = "ResponseId";
constexpr char READ_DATA_BY_IDENTIFIER_FIELD[] = "ReadDataByIdentifier";
constexpr char READ_SEED[] = "Seed";


class EcuLuaScript
{
    
public:
    EcuLuaScript() = delete;
    EcuLuaScript(const EcuLuaScript& origin) = default;
    EcuLuaScript& operator =(const EcuLuaScript& origin) = default;
    EcuLuaScript(EcuLuaScript&& origin) = default;
    EcuLuaScript& operator =(EcuLuaScript&& origin) = default;
    EcuLuaScript(const std::string& luaScript);
    virtual ~EcuLuaScript();

    std::uint16_t getRequestId() const;
    std::uint16_t getResponseId() const;
    const std::string getSeed(std::uint8_t identifier) const;
    const std::string getDataByIdentifier(std::uint16_t identifier) const;

private:
    sel::State* lua_state_ = nullptr;
    std::uint16_t request_id_ = 0;
    std::uint16_t response_id_ = 0;

};

#endif /* ECU_LUA_SCRIPT_H */
