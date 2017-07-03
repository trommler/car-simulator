/**
 * @file uds_server.h
 *
 */

#ifndef UDS_SERVER_H
#define UDS_SERVER_H

#include "isotp_socket.h"
#include "ecu_lua_script.h"
#include "session_controller.h"

class UdsServer : public IsoTpSocket
{
public:
    UdsServer() = delete;

    UdsServer(canid_t source,
              canid_t dest,
              const std::string& device,
              SessionController* pSesCtrl,
              EcuLuaScript&& ecuScript);

    virtual ~UdsServer();

    virtual void proceedReceivedData(const std::uint8_t* buffer,
                                     const std::size_t num_bytes) noexcept override;

private:
    SessionController* pSessionCtrl_;
    const EcuLuaScript script_;
    uint8_t securityAccessType_ = 0x00;

    void readDataByIdentifier(const std::uint8_t* buffer, const std::size_t num_bytes) noexcept;
    void diagnosticSessionControl(const std::uint8_t* buffer, const std::size_t num_bytes);
    void securityAccess(const std::uint8_t* buffer, const std::size_t num_bytes) noexcept;
    std::string intToHexString(const uint8_t* buffer, const std::size_t num_bytes);
};

#endif /* UDS_SERVER_H */
