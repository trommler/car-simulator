/** 
 * @file uds_server.h
 *
 */

#ifndef UDSSERVER_H
#define UDSSERVER_H

#include "isotp_socket.h"
#include "ecu_lua_script.h"

class UdsServer : public IsoTpSocket
{
public:
    UdsServer() = delete;

    UdsServer(canid_t source,
              canid_t dest,
              const std::string& device,
              const std::string& luaFile)
    : IsoTpSocket(source, dest, device)
    , script_(EcuLuaScript(luaFile))
    {
    }

    UdsServer(canid_t source,
              canid_t dest,
              const std::string& device,
              EcuLuaScript&& ecuScript)
    : IsoTpSocket(source, dest, device)
    , script_(std::move(ecuScript))
    {
    }

    virtual ~UdsServer() = default;

    void proceedReceivedData(const std::uint8_t* buffer,
                             std::size_t num_bytes) noexcept override;

private:
    const EcuLuaScript script_;

};

#endif /* UDSSERVER_H */
