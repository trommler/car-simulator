/** 
 * @file electronic_control_unit.h
 * 
 */

#ifndef ELECTRONIC_CONTROL_UNIT_H
#define ELECTRONIC_CONTROL_UNIT_H

#include "selene.h"
#include "broadcast_server.h"
#include "uds_server.h"
#include "ecu_lua_script.h"
#include "config.h"
#include "session_controller.h"
#include <string>
#include <thread>
#include <memory>

class ElectronicControlUnit
{
public:
    ElectronicControlUnit() = delete;
    ElectronicControlUnit(const std::string& device, EcuLuaScript&& ecuScript);
    virtual ~ElectronicControlUnit();
    void testECU(const std::string &config_file, const std::string &device);

private:
    SessionController sessionControl_;
    BroadcastServer broadcastServer_;
    UdsServer udsServer_;
    std::thread broadcastServerThread_;
    std::thread udsServerThread_;
};

#endif /* ELECTRONIC_CONTROL_UNIT_H */
