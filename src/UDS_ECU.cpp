#include "UDS_ECU.h"
#include <iostream>

using namespace std;

UDS_ECU::UDS_ECU(unsigned int source, unsigned int dest, const string &device)
    : isotp_socket(source, dest, device.c_str())
{
    CAN_UDS_Layer();
}

//void UDS_ECU::init_receive_id(int id)
//{
    //CAN_UDS_Layer::set_CANreceiveID(id);
//}

//void UDS_ECU::init_send_id(int id)
//{
    //CAN_UDS_Layer::set_CANsendID(id);
//}

//void UDS_ECU::sendUDSmessage(unsigned char* data, unsigned int data_size)
//{
    //CAN_UDS_Layer::sendCANmessage(data, data_size);
//}

void UDS_ECU::receiveUDSmessage()
{
    unsigned char *buffer = new unsigned char[4096];
    int recieved_bytes = isotp_socket.recieveData(buffer, 4096);

    if (buffer[0] == 0x3E)
    {
        cout << "Received message: ";
        printUDSmessage(buffer, recieved_bytes);

        printf("UDS - TesterPresent sending: \n%x %x \n", 0x7E, 0x00);
        UDS_TesterPresent();
    }
    else
    {
        cout << "Received unkown message: ";
        printUDSmessage(buffer, recieved_bytes);

        unsigned char temp[1] = {0xEF};
        cout << "sending error - code: ";
        printUDSmessage(temp, 1);
        isotp_socket.sendData(temp, 1);
    }
}

void UDS_ECU::UDS_TesterPresent()
{
    unsigned char temp[2];
    temp[0] = 0x7E;
    temp[1] = 0x00;
    isotp_socket.sendData(temp, 2);
}

void UDS_ECU::printUDSmessage(unsigned char *buffer, int size)
{
    for (int i = 0; i < size; ++i)
    {
        cout << hex << static_cast<int>(buffer[i]) << " ";
    }
    cout << endl;
}
