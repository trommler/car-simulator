/** 
 * @file ecu_lua_script.h
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
constexpr char BROADCAST_ID_FIELD[] = "BroadcastId";
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
    std::uint16_t getBroadcastId() const;
    std::string getSeed(std::uint8_t identifier) const;
    std::string getDataByIdentifier(const std::string& identifier) const;
    std::string getDataByIdentifier(const std::string& identifier, const std::string& session) const;
    std::string getRaw(const std::string& identStr) const;
    bool hasRaw(const std::string& identStr) const;
    std::vector<std::uint8_t> literalHexStrToBytes(const std::string& hexString) const;

    static std::string ascii(const std::string& utf8_str) noexcept;
    static std::string toByteResponse(std::uint32_t value, std::uint32_t len = sizeof(std::uint32_t)) noexcept;
    static void sleep(unsigned int ms) noexcept;
    void sendRaw(const std::string& response) const;
    int getCurrentSession() const;
    void switchToSession(int ses);

    void setSessionController(SessionController* pSesCtrl) noexcept;

private:
    sel::State lua_state_;
    std::string ecu_ident_;
    SessionController* pSessionCtrl_;
    std::uint16_t requestId_;
    std::uint16_t responseId_;

};

#endif /* ECU_LUA_SCRIPT_H */
