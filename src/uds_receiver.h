/** 
 * @file uds_receiver.h
 *
 */

#ifndef UDS_RECEIVER_H
#define UDS_RECEIVER_H

#include "isotp_receiver.h"
#include "isotp_sender.h"
#include "ecu_lua_script.h"
#include "session_controller.h"

class UdsReceiver : public IsoTpReceiver
{
    friend class BroadcastReceiver;

public:
    UdsReceiver() = delete;
    UdsReceiver(canid_t source,
                canid_t dest,
                const std::string& device,
                EcuLuaScript&& ecuScript,
                const IsoTpSender& sender,
                SessionController* pSesCtrl);
    UdsReceiver(const UdsReceiver& orig) = default;
    UdsReceiver& operator =(const UdsReceiver& orig) = default;
    UdsReceiver(UdsReceiver&& orig) = default;
    UdsReceiver& operator =(UdsReceiver&& orig) = default;
    virtual ~UdsReceiver() = default;

    virtual void proceedReceivedData(const uint8_t* buffer, const size_t num_bytes) noexcept override;

private:
    EcuLuaScript script_;
    const IsoTpSender& sender_;
    SessionController* pSessionCtrl_;
    uint8_t securityAccessType_ = 0x00;

    void readDataByIdentifier(const std::uint8_t* buffer, const std::size_t num_bytes) noexcept;
    void diagnosticSessionControl(const std::uint8_t* buffer, const std::size_t num_bytes);
    void securityAccess(const std::uint8_t* buffer, const std::size_t num_bytes) noexcept;

    std::string intToHexString(const uint8_t* buffer, const std::size_t num_bytes);
};

#endif /* UDS_RECEIVER_H */

