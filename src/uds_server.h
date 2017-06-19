/**
 * @file uds_server.h
 *
 */

#ifndef UDSSERVER_H
#define UDSSERVER_H

#include "isotp_socket.h"
#include "ecu_lua_script.h"
#include "ecuTimer.h"

// forward decl
class UdsServer;


constexpr canid_t BROADCAST_ADDR = 0x7E0;


class BroadcastSkt : public IsoTpSocket
{
public:
    BroadcastSkt() = delete;

    BroadcastSkt(canid_t dest, const std::string& device, UdsServer *uds_server);

    virtual ~BroadcastSkt();

    virtual void proceedReceivedData(const std::uint8_t* buffer,
                                     const std::size_t num_bytes) noexcept override;
private:
    UdsServer *uds_server_;
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

    void test_callback(int foo);

private:
    const EcuLuaScript script_;
    uint8_t securityAccessType = 0x00;
    uint8_t response_data_[4095];
    uint8_t response_data_size_ = 0;

    // TODO: replace this proxy with useful timers
    SomePurposeTimer23_42 test_timer_;


    void copyLuaScriptResponse(std::string);
    void readDataByIdentifier(const std::uint8_t* buffer, const std::size_t num_bytes) noexcept;
};

#endif /* UDSSERVER_H */
