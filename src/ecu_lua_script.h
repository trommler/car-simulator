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
#include <vector>

constexpr char REQ_ID_FIELD[] = "RequestId";
constexpr char RES_ID_FIELD[] = "ResponseId";
constexpr char READ_DATA_BY_IDENTIFIER_TABLE[] = "ReadDataByIdentifier";
constexpr char READ_SEED[] = "Seed";
constexpr char RAW_TABLE[] = "Raw";

class EcuLuaScript
{
public:
    EcuLuaScript() = delete;
    EcuLuaScript(EcuLuaScript&& orig) = default;
    EcuLuaScript& operator =(EcuLuaScript&& orig) = default;
    EcuLuaScript(const std::string& ecuIdent, const std::string& luaScript);
    virtual ~EcuLuaScript() = default;

    virtual std::uint16_t getRequestId() const;
    virtual std::uint16_t getResponseId() const;
    virtual std::string getSeed(std::uint8_t identifier) const;
    virtual std::string getDataByIdentifier(std::uint16_t identifier) const;
    std::vector<std::uint8_t> literalHexStrToBytes(std::string& hexString);

    std::string ascii(const std::string& utf8_str) const noexcept;
    std::string toByteResponse(std::size_t len, long value) const;
    void sendRaw(const std::string& response) const;
    void sleep(unsigned int ms) const;
    int getCurrentSession() const;
    void switchToSession(int ses);

private:
    sel::State lua_state_;
    std::string ecu_ident_ = "";

};

#endif /* ECU_LUA_SCRIPT_H */
