#include "SocketCAN.h"

using namespace std;

    SocketCAN::SocketCAN()
    {
        can_data_size = 0;
    }

    void SocketCAN::initSocketCAN_driver(char* device)
    {
        skt = socket(PF_CAN, SOCK_RAW,CAN_RAW);
        strcpy(ifr.ifr_name, device);
        ioctl(skt, SIOCGIFINDEX, &ifr);

        // CAN interface
        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;
        bind(skt, (struct sockaddr*) &addr, sizeof(addr));
    }

    void SocketCAN::set_CANsendID(signed int can_id)
    {
        send_id = can_id;
    }

    void SocketCAN::set_CANreceiveID(signed int can_id)
    {
        receive_id = can_id;
    }

    void SocketCAN::sendCANframe(unsigned char* data, int data_size)
    {
        // send can message
        for(int i = 0; i < data_size; i++)
        {
            sendframe.data[i] = data[i];
        }
        sendframe.can_id = send_id;
        sendframe.can_dlc = data_size;
        write(skt, &sendframe, sizeof(sendframe));
    }

    void SocketCAN::receiveallCANframe()
    {
        read(skt, &receiveframe, sizeof(receiveframe));
    }

    void SocketCAN::receiveCANframe()
    {
        while(receiveframe.can_id != receive_id)
        {
            receiveallCANframe();
        }
        can_data_size = receiveframe.can_dlc;
        for(int i = 0; i < receiveframe.can_dlc; i++)
        {
            can_data[i] = receiveframe.data[i];
        }
        receiveframe.can_id = 0x000;
    }

    unsigned char* SocketCAN::get_can_data()
    {
        return can_data;
    }

    unsigned char SocketCAN::get_can_data_length()
    {
        return can_data_size;
    }
