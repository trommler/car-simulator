#include "UDS_TESTER.h"

int main(int argc, char **argv)
{
	printf("SocketCAN Tester started, creating connection...\n");
    UDS_TESTER* tester = new UDS_TESTER();
    printf("Connection established, sending message:0x20 \n");
    unsigned char Message[] = {0x10,0x20,0x30,0x40,0x50};

       while(true)
       {
           tester->sendUDSmessage(Message,sizeof(Message));
           printf("Message 0x10 sended,waiting for reply...\n");
           tester->receiveUDSmessage();
       }


   	return 0;
}
