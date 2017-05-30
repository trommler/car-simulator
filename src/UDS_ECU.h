#include "isotp_socket.h"

class UDS_ECU
{
    protected:
    IsoTpSocket isotp_socket;

    public:
    UDS_ECU(unsigned int souce,
            unsigned int dest,
            const std::string &device);
    void receiveUDSmessage();
    void printUDSmessage(unsigned char *buffer, int size);
    void UDS_TesterPresent();
};
