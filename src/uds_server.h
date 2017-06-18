/** 
 * @file uds_server.h
 *
 */

#ifndef UDSSERVER_H
#define UDSSERVER_H

#include "isotp_socket.h"
#include "ecu_lua_script.h"

constexpr canid_t BROADCAST_ADDR = 0x7e0; //TODO: set the address according to the spec. 

class BroadcastSkt : public IsoTpSocket
{
public:
    BroadcastSkt() = delete;

    BroadcastSkt(canid_t dest, const std::string& device);

    virtual ~BroadcastSkt();

    virtual void proceedReceivedData(const std::uint8_t* buffer,
                                     const std::size_t num_bytes) noexcept override;
private:
    std::unique_ptr<std::thread> p_server_thread_;    
};

class UdsServer : public IsoTpSocket
{
public:
    UdsServer() = delete;

    UdsServer(canid_t source,
              canid_t dest,
              const std::string& device,
              EcuLuaScript&& ecuScript);

    virtual ~UdsServer();

    virtual void proceedReceivedData(const std::uint8_t* buffer,
                                     const std::size_t num_bytes) noexcept override;

private:
    const EcuLuaScript script_;
    uint8_t securityAccessType = 0x00;
    uint8_t response_data_[4095];
    uint8_t response_data_size_ = 0;
    const BroadcastSkt broadcastSkt_;

    void copyLuaScriptResponse(std::string);
    void readDataByIdentifier(const std::uint8_t* buffer, const std::size_t num_bytes) noexcept;
};

#endif /* UDSSERVER_H */
