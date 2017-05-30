#include "UDS_ECU.h"
#include <iostream>

using namespace std;

UDS_ECU::UDS_ECU(unsigned int source, unsigned int dest, const string &device)
    : isotp_socket(source, dest, device.c_str())
{
}

void UDS_ECU::receiveUDSmessage()
{
    unsigned char *buffer = new unsigned char[4096];

    // TODO(chris): Process recieved messages here or in receiveData?
    //              Right now it's done in receiveData.
    //int received_bytes = isotp_socket.receiveData(buffer, 4096);
    int received_bytes = 0;

    if (buffer[0] == 0x3E)
    {
        cout << "Received message: ";
        printUDSmessage(buffer, received_bytes);

        printf("UDS - TesterPresent sending: \n%x %x \n", 0x7E, 0x00);
        UDS_TesterPresent();
    }
    else
    {
        cout << "Received unkown message: ";
        printUDSmessage(buffer, received_bytes);

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
