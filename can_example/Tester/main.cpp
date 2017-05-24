#include "SocketCAN.h"

int main(int argc, char **argv)
{
   SocketCAN* test = new SocketCAN();
    char send[] = {0x10};
    test->setupSocketCAN_interface("vcan0");
    printf("hello world\n");
    while(true)
    {
        test->sendCANframe(send, sizeof(send));
        test->receiveCANframe();
        test->printreceiveCANframe();
    }


	return 0;
}
