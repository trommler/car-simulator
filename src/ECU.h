/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ECU.h
 * 
 */

#ifndef ECU_H
#define ECU_H

#include "selene.h"
#include "utilities.h"
#include "isotp_socket.h"
#include "uds_server.h"
#include "ecu_lua_script.h"
#include "config.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <libgen.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <array>
#include <cstdint>
#include <iostream>
#include <cerrno>
#include <thread>

using namespace std;

class ECU
{
private:    
    unsigned int server_size;
    thread* t;
    EcuLuaScript* script_;
    UdsServer* uds_server[MAX_ECU];
public:
    ECU()
    {
        server_size = 0;
        script_ = new EcuLuaScript(PATH_TO_LUA);
    }
    ~ECU()
    {
        for(int i = 0; i < server_size; i++)
        {
            uds_server[i]->closeSender();
            uds_server[i]->closeReceiver();
        }
        t->~thread();
    }
    void initECU();
    void testECU();
};


#endif /* ECU_H */

