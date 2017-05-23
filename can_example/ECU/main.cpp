#include "SocketCAN.h""

int main(int argc, char **argv)
{
    SocketCAN* test = new SocketCAN();
    char send[] = {0x20,0x20};
    test->setupSocketCAN_interface("vcan0");

    while(true)
    {
        test->receiveCANframe();
        test->printreceiveCANframe();
        test->sendCANframe(send, sizeof(send));
    }

	printf("hello world\n");
	return 0;
}
