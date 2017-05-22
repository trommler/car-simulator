## Installing CAN on Linux

1. Install SocketCAN a.k.a. the 'can-utils' package

This can simply be done under a Debian/Ubuntu/Raspian based distribution with:

    sudo apt install can-utils

OR install the package manually by following the steps as described here:
[eLinux Wiki](http://elinux.org/Can-utils)


2. Load the Kernel modules

Since you probably have no real hardware CAN device, we use the virtual CAN interface for this guide. Therefore we have to load the Kernel module and set up the connection with:

    sudo modprobe vcan
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


## Installing ISO-TP

1. Clone the following repository with: `git clone https://github.com/linux-can/can-utils.git`

2. Enter the directory via bash and run the following commands:

```sh
./autogen.sh
./configure
make
sudo make install
```

For further details and troubleshooting consider the [can-isotp README](https://github.com/hartkopp/can-isotp).


## Testing the ISO-TP connection

1. Set up the CAN connection with the following commands, like described in the section before:

    sudo modprobe vcan
    sudo ip link add dev vcan0 type vcan
    sudo ip link set up vcan0

2. Open the receiver with `isotprecv -s 123 -d 321 -l vcan0`.

3. Open a new terminal window and transmit some test data thorough the sender via `echo 11 22 33 44 55 66 DE AD BE EF | isotpsend -s 321 -d 123 vcan0`.

4. Now, you should see how the first terminal window is receiving the data.

For further instructions and troubleshooting take a look into the [can-isotp README](https://github.com/hartkopp/can-isotp).


## Installing Lua on Linux

Install the `lua5.2` and `liblua5.2-0` packages with:

    sudo apt install lua5.2 liblua5.2-0

Include Lua support in the C++ file with

```cpp
#include <lua.hpp>
```

The access from the C++ code to the Lua scripts is done via Selene. See the [Git Page](https://github.com/jeremyong/Selene) for a simple introduction.


## Install Unit-Tests

Install the following CppUnitTest packages with:

    sudo apt install libcppunit-1.* libcppunit-dev libcppuint-doc`

The build target according to the Makefile is `make test`.


## Set-Up an IDE

You can choose the easy way with the [NetBeans Set-Up](resources/NetBeansSetUp.md) or the harder one with the [Eclipse Set-Up](resources/EclipseSetUp.md)

_(Author's note: NetBeans is more comfortable in any way)_


