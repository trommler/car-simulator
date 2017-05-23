#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#ifndef PF_CAN
#define PF_CAN 29
#endif

#ifndef AF_CAN
#define AF_CAN PF_CAN
#endif

using namespace std;

class SocketCAN
{
    protected:
    int skt;
    int send_id;
    int receive_id;
    struct ifreq ifr;
    struct sockaddr_can addr;
    struct can_frame sendframe;
    struct can_frame receiveframe;
    unsigned char can_data[8];
    unsigned char can_data_size;

    public:
    SocketCAN();

    void initSocketCAN_driver(char*);
    void set_CANsendID(int);
    void set_CANreceiveID(int);

    void sendCANframe(unsigned char*, int);
    void receiveallCANframe();
    void receiveCANframe();

    unsigned char* get_can_data();
    unsigned char get_can_data_length();
};
