#include "SocketCAN.h"

class CAN_UDS_Layer : public SocketCAN
{
    protected:
    int sn;
    unsigned char block_size;
    
    unsigned char send_data[8];
    unsigned char send_data_size;
    
    unsigned int received_data_size;
    unsigned char received_data[4095];
    
    public:
    CAN_UDS_Layer();
    
    void sendCANmessage(unsigned char*, int);
    void sendflowcontrolframe();
    void receiveCANmessage();
    
    unsigned char* get_recieved_data();
    unsigned int get_recieved_data_length();
    
    void printreceiveddata();
};
