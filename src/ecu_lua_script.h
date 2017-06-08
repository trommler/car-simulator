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
constexpr char READ_DATA_BY_IDENTIFIER_TABLE[] = "ReadDataByIdentifier";
constexpr char READ_SEED[] = "Seed";
constexpr char RAW_TABLE[] = "Raw";

class EcuLuaScript
{
public:
    EcuLuaScript() = delete;
    EcuLuaScript(EcuLuaScript&& origin) = default;
    EcuLuaScript& operator =(EcuLuaScript&& origin) = default;
    EcuLuaScript(const std::string& ecuIdent, const std::string& luaScript);
    virtual ~EcuLuaScript() = default;

    std::uint16_t getRequestId() const;
    std::uint16_t getResponseId() const;
    std::string getSeed(std::uint8_t identifier) const;
    std::string getDataByIdentifier(std::uint16_t identifier) const;

private:
    sel::State lua_state_;
    std::string ecu_ident_ = "";
    std::uint16_t request_id_ = 0;
    std::uint16_t response_id_ = 0;

};

#endif /* ECU_LUA_SCRIPT_H */
