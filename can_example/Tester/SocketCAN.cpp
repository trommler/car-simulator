#include "SocketCAN.h"

using namespace std;

    SocketCAN::SocketCAN()
    {

    }

    void SocketCAN::setupSocketCAN_interface(char* device)
    {
        skt = socket(PF_CAN, SOCK_RAW,CAN_RAW);
        strcpy(ifr.ifr_name, device);
        ioctl(skt, SIOCGIFINDEX, &ifr);

        // CAN interface
        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;
        bind(skt, (struct sockaddr*) &addr, sizeof(addr));


    }

    void SocketCAN::sendCANframe(char* data, int data_size)
    {
        // send can message
        frame.can_id = 0x100;
        for(int i = 0; i < data_size; i++)
        {
            frame.data[i] = data[i];
        }
        frame.can_dlc = data_size;
        write(skt, &frame, sizeof(frame));
    }

    void SocketCAN::receiveCANframe()
    {
        read(skt, &frame, sizeof(frame));
    }

    void SocketCAN::printreceiveCANframe()
    {
        for(int i = 0; i < frame.can_dlc; i++)
        {
            printf("%x ", frame.data[i]);
        }
        printf("\n");
    }
