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

    void test_callback(int foo);

private:
    SessionController* pSessionCtrl_;
    const EcuLuaScript script_;
    uint8_t securityAccessType = 0x00;
    uint8_t response_data_[4095];
    uint8_t response_data_size_ = 0;

    void copyLuaScriptResponse(std::string);
    void readDataByIdentifier(const std::uint8_t* buffer, const std::size_t num_bytes) noexcept;
};

#endif /* UDS_SERVER_H */
