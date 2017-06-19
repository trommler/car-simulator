/** 
 * @file ECU.h
 * 
 */

#ifndef ECU_H
#define ECU_H

#include "selene.h"
#include "uds_server.h"
#include "ecu_lua_script.h"
#include "config.h"
#include <string>
#include <thread>
#include <memory>

class ECU
{
public:
    ECU() = delete;
    ECU(const string& device, EcuLuaScript&& ecuScript);
    virtual ~ECU();
    void testECU(const std::string &config_file, const std::string &device);

private:
    BroadcastSkt broadcastSkt_;
    UdsServer udsServer_;
    std::thread broadcastServerThread_;
    std::thread udsServerThread_;
};

#endif /* ECU_H */
