#include <SocketCAN.h>

int main(int argc, char **argv)
{
    SocketCAN* test = new SocketCAN();
    char send[] = {0x10};
    test->setupSocketCAN_interface("vcan0");
    
    while(true)
    {
        test->sendCANframe(send, sizeof(send));
        test->receiveCANframe();
        test->printreceiveCANframe();
    }
    
	printf("hello world\n");
	return 0;
}
