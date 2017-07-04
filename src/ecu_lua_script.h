/** 
 * @file: ecu_lua_script.h
 *
 */

#ifndef ECU_LUA_SCRIPT_H
#define ECU_LUA_SCRIPT_H

#include "selene.h"
#include "session_controller.h"
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

    std::uint16_t getRequestId() const;
    std::uint16_t getResponseId() const;
    std::string getSeed(std::uint8_t identifier) const;
    std::string getDataByIdentifier(std::uint16_t identifier) const;
    std::string getDataByIdentifier(std::uint16_t identifier, const std::string& session) const;
    std::string getRaw(const std::string& identStr) const;
    bool hasRaw(const std::string &identStr) const;
    std::vector<std::uint8_t> literalHexStrToBytes(const std::string& hexString) const;

    static std::string ascii(const std::string& utf8_str) noexcept;
    static std::string toByteResponse(unsigned int value, unsigned int len = sizeof(unsigned long)) noexcept;
    static void sleep(unsigned int ms) noexcept;
    void sendRaw(const std::string& response) const;
    int getCurrentSession() const;
    void switchToSession(int ses);

    void setSessionController(SessionController *sc) noexcept {pSessionCtrl_ = sc;}

private:
    sel::State lua_state_;
    std::string ecu_ident_;
    SessionController* pSessionCtrl_;

};

#endif /* ECU_LUA_SCRIPT_H */
