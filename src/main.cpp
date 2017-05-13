/** 
 * @file main.cpp
 * @author Florian Bauer
 *
 * This file contains the `main()`-function with a simple Lua script test and a CAN test set-up.
 */
#include "LuaScript.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <cstring>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>

using namespace std;

/**
 * The main application only for testing purposes. 
 * 
 * @param argc: the number of arguments
 * @param argv: the argument list
 * @return 0 on success, otherwise a negative value
 */
int main(int argc, char** argv)
{
    // some Lua script tests...
    LuaScript myScript = LuaScript::import_script("testscript.lua");
    myScript.executeScript();
    
    // some virtualCAN tests...
    int so = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (so < 0)
    {
        cerr << "Error while opening socket\n";
        return -1;
    }

    const string ifname = "vcan0"; // TODO: change to real CAN device
    struct ifreq ifr;
    strncpy(ifr.ifr_name, ifname.c_str(), ifname.length());
    ioctl(so, SIOCGIFINDEX, &ifr);

    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    cout << ifname << " at index " << ifr.ifr_ifindex << '\n';

    if (bind(so, (struct sockaddr*) &addr, sizeof(addr)) < 0)
    {
        cerr << "Error in socket bind\n";
        return -2;
    }

    struct can_frame frame;
    frame.can_id = 0x123;
    frame.can_dlc = 2;
    frame.data[0] = 0x11;
    frame.data[1] = 0x22;

    auto nbytes = write(so, &frame, sizeof(struct can_frame));
    cout << "Wrote " << nbytes << " bytes\n";

    return 0;
}
