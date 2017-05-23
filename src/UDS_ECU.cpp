#include "UDS_ECU.h"

    UDS_ECU::UDS_ECU()
    {
        CAN_UDS_Layer();
    }

    void UDS_ECU::init_receive_id(int id)
    {
        CAN_UDS_Layer::set_CANreceiveID(id);
    }

    void UDS_ECU::init_send_id(int id)
    {
        CAN_UDS_Layer::set_CANsendID(id);
    }

    void UDS_ECU::sendUDSmessage(unsigned char* data, unsigned int data_size)
    {
        CAN_UDS_Layer::sendCANmessage(data, data_size);
    }

    void UDS_ECU::receiveUDSmessage()
    {
        CAN_UDS_Layer::receiveCANmessage();
        if(CAN_UDS_Layer::get_recieved_data()[0] == 0x3E)
        {
            printf("Received message:");
            printUDSmessage();
            printf("UDS - TesterPresent sending: \n%x %x \n", 0x7E, 0x00);
            UDS_TesterPresent();
        }
        else
        {
            unsigned char temp[1];
            temp[0] = 0xEF;
            printf("Received unkown message:");
            printUDSmessage();
            printf("sending error - code:\n");
            sendUDSmessage(temp, 1);
            printf("%x \n", temp[0]);
        }
    }

    void UDS_ECU::UDS_TesterPresent()
    {
        unsigned char temp[2];
        temp[0] = 0x7E;
        temp[1] = 0x00;
        CAN_UDS_Layer::sendCANmessage(temp, sizeof(temp));
    }

    void UDS_ECU::printUDSmessage()
    {
        for(unsigned int i = 0; i < CAN_UDS_Layer::get_recieved_data_length(); i++)
        {
            if(i % 8 == 0)
                printf("\n");
            printf("%x ", CAN_UDS_Layer::get_recieved_data()[i]);
        }
        printf("\n");
    }

