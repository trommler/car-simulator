#include "CAN_UDS_Layer.h"

class UDS_ECU : public CAN_UDS_Layer
{
    public:
    UDS_ECU();
    void sendUDSmessage(unsigned char*, unsigned int);
    void receiveUDSmessage();
    void printUDSmessage();
    void init_send_id(int);
    void init_receive_id(int);
    void UDS_TesterPresent();

    // todo implement some UDS functions

};
