#include "CAN_UDS_Layer.h"
#include "isotp_socket.h"

class UDS_ECU : public CAN_UDS_Layer
{
    protected:
    IsoTpSocket isotp_socket;

    public:
    UDS_ECU(unsigned int souce,
            unsigned int dest,
            const std::string &device);
    //void sendUDSmessage(unsigned char*, unsigned int);
    void receiveUDSmessage();
    void printUDSmessage(unsigned char *buffer, int size);
    //void init_send_id(int);
    //void init_receive_id(int);
    void UDS_TesterPresent();

    // todo implement some UDS functions

};
