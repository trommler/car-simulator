#include "UDS_TESTER.h"

    UDS_TESTER::UDS_TESTER()
    {
        CAN_UDS_Layer();
    }
    
    void UDS_TESTER::init_receive_id(int id)
    {
        CAN_UDS_Layer::set_CANreceiveID(id);
    }
    
    void UDS_TESTER::init_send_id(int id)
    {
        CAN_UDS_Layer::set_CANsendID(id);
    }
    
    void UDS_TESTER::sendUDSmessage(unsigned char* data, unsigned int data_size)
    {
        CAN_UDS_Layer::sendCANmessage(data, data_size);
    }
    
    void UDS_TESTER::receiveUDSmessage()
    {
        CAN_UDS_Layer::receiveCANmessage();
        if(CAN_UDS_Layer::get_recieved_data()[0] == 0x7E)
        {
            printf("Positive Received message:");
            printUDSmessage();
        }
        else if(CAN_UDS_Layer::get_recieved_data()[0] == 0xEF||CAN_UDS_Layer::get_recieved_data()[0] == 0x7E){
        	printf("Negative Received message:");
        	printUDSmessage();
        }
        else
        {
            printf("ECU_SERVER status can not be read");
        }
    }
    
    void UDS_TESTER::UDS_TesterPresent()
    {
        unsigned char temp[2];
        temp[0] = 0x7E;
        temp[1] = 0x00;
        CAN_UDS_Layer::sendCANmessage(temp, sizeof(temp));
    }
    
    void UDS_TESTER::printUDSmessage()
    {
        for(int i = 0; i < CAN_UDS_Layer::get_recieved_data_length(); i++)
        {
            if(i % 8 == 0)
                printf("\n");
            printf("%x ", CAN_UDS_Layer::get_recieved_data()[i]);            
        }
        printf("\n");
    }
    
