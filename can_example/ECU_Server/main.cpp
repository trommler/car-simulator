#include "UDS_ECU.h"

int main(int argc, char **argv)
{
    UDS_ECU* test = new UDS_ECU();
    printf("ECU Starts \n");
        
    while(true)
    {
        test->receiveUDSmessage();
    }
    
    
	return 0;
}
