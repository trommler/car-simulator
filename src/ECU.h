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
    std::unique_ptr<std::thread> p_server_thread_;
    UdsServer uds_server_;
};

#endif /* ECU_H */
