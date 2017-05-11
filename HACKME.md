## Installing CAN in an Linux enviroment

1. Install SocketCAN aka the 'can-utils' package  

This can simply be done under a Debian/Ubuntu/Raspian based distribution with:  

    sudo apt install can-utils

OR install the package manually by following the steps as described here:
[eLinux Wiki](http://elinux.org/Can-utils)


2. Load the Kernel modules

Since you probably have no real hardware CAN interface, we use the virtualCAN device. Therefore we
have to load the Kernel module with `sudo modprobe vcan` and set up the connection with:  

    sudo ip link add dev vcan0 type vcan
    sudo ip link set up vcan0


3. Test the connection

* Open a Terminal window and typ `candump vcan0`
* Open another new Terminal window and send an arbitrary CAN message with 
  `cansend vcan0 123#1122334455667788`

The first Terminal window should now show something like:

    user@linux ~ $ candump vcan0
      vcan0  123   [8]  11 22 33 44 55 66 77 88

For further informations and troubleshooting see:

[Using can-utils](https://discuss.cantact.io/t/using-can-utils/24)
[Bringing CAN interface up](http://elinux.org/Bringing_CAN_interface_up)
[SocketCAN](https://en.wikipedia.org/wiki/SocketCAN)


## Installing Lua on Linux

Install the following packages with `apt get install ...`:  

    lua5.2
    lua5.2-dev
    lua5.2-doc

Include Lua support in the C++ file with 

```cpp
#include <lua.hpp>
```


## Install Unit-Tests

Install the following packages with `apt get install ...`:  

    libcppunit-1.13
    libcppunit-dev
    libcppuint-doc

Build targets according to the Makefile are `build-tests` and `tests`.


## Set-Up an IDE

You can choose the easy way with the [NetBeans Set-Up](resources/NetBeansSetUp.md) or the harder one with the [Eclipse Set-Up](resources/EclipseSetUp.md)

_(Author's note: NetBeans is more comfortable anyway)_


