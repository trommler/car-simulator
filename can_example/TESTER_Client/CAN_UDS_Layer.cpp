#include "CAN_UDS_Layer.h"

    CAN_UDS_Layer::CAN_UDS_Layer()
    {
        // default value
        SocketCAN();
        SocketCAN::initSocketCAN_driver("vcan0");
        SocketCAN::set_CANsendID(0x200);
        SocketCAN::set_CANreceiveID(0x400);
        block_size = 0;
    }
    
    void CAN_UDS_Layer::sendCANmessage(unsigned char* data, int data_size)
    {
        // unsigned char temp[8];
        int count = 0;
        
        if(data_size < 8)
        {
            // single frame
            send_data[0] = data_size;
            for(int i = 1; i < data_size+1;i++)
            {
                send_data[i] = data[i-1];
            }
            SocketCAN::sendCANframe(send_data, data_size+1);
        }
        else
        {
            // multiply CAN frame
            
            // send first frame
            send_data[0] = 0x10 + data_size / 256;
            send_data[1] = data_size % 256;
            for(int i = 0; i < 6; i++)
            {
                send_data[i+2] = data[count++];
            }
            
            SocketCAN::sendCANframe(send_data, CAN_MAX_DLEN);
            
            // flow control frame
            SocketCAN::receiveCANframe();
            if(SocketCAN::get_can_data()[0] == 0x30 && SocketCAN::get_can_data()[1] == 0x00)
            {                
                // send consecutive frame
                sn = 0;
                int split_frame = 0;
                while(count != data_size)
                {
                    if(split_frame == 0)
                    {
                        send_data[split_frame++] = 0x20 + sn++;
                        if(sn % 16 == 0)
                            sn = 0;
                    }
                    else if(split_frame < 8 && split_frame != 0)
                    {
                        send_data[split_frame++] = data[count++];
                    }
                    else
                    {
                        SocketCAN::sendCANframe(send_data, CAN_MAX_DLEN);
                        split_frame = 0;
                    }
                }
                SocketCAN::sendCANframe(send_data, split_frame);
                sn = 0;
            }
            else
            {
                unsigned char bs = SocketCAN::get_can_data()[1];
         
                // send consecutive frame
                sn = 0;
                int split_frame = 0;
                while(count != data_size)
                {
                    if(split_frame == 0)
                    {
                        send_data[split_frame++] = 0x20 + sn++;
                        if(sn % 16 == 0)
                            sn = 0;
                    }
                    else if(split_frame < 8 && split_frame != 0)
                    {
                        send_data[split_frame++] = data[count++];
                    }
                    else
                    {
                        SocketCAN::sendCANframe(send_data, split_frame);
                        split_frame = 0;
                        bs--;
                    }
                    
                    if(bs == 0)
                    {
                        SocketCAN::receiveCANframe();
                        bs = SocketCAN::get_can_data()[1];
                    }
                }
                
                // send last consectuive frame
                SocketCAN::sendCANframe(send_data, split_frame);
                sn = 0;
            }            
        }
    }
    
    void CAN_UDS_Layer::receiveCANmessage()
    {
        SocketCAN::receiveCANframe();
        unsigned char frame_type = SocketCAN::get_can_data()[0];
        
        if((frame_type >> 4) == 0x00)
        {
            // printf("Single Frame \n");
            received_data_size = SocketCAN::get_can_data()[0];
            for(int i = 0; i < received_data_size; i++)
            {
                received_data[i] = SocketCAN::get_can_data()[i+1];
            }
            
        }
        else if((frame_type >> 4) == 0x01)
        {
            // printf("First Frame \n");
            unsigned int count = 0;
            frame_type = (frame_type << 4);
            frame_type = frame_type >> 4;
            received_data_size = (frame_type << 8) + SocketCAN::get_can_data()[1];
            for(int i = 0; i < 6; i++)
            {
                received_data[count++] = SocketCAN::get_can_data()[i+2];
            }
            
            // flow control frame
            sendflowcontrolframe();
            
            if(block_size == 0x00)
            {
                while(count != received_data_size)
                {
                    SocketCAN::receiveCANframe();
                    for(int i = 1; i < SocketCAN::get_can_data_length(); i++)
                    {
                        received_data[count++] = SocketCAN::get_can_data()[i];
                        if(count == received_data_size)
                            break;
                    }
                }
            }
            else
            {
                // implement block size consecutive frames
                
            }
        }
        else
        {
            received_data_size = SocketCAN::get_can_data_length();
            for(int i = 0; i < SocketCAN::get_can_data_length(); i++)
            {
                received_data[i] = SocketCAN::get_can_data()[i];
            }
        }
    }
    
    void CAN_UDS_Layer::sendflowcontrolframe()
    {
        send_data[0] = 0x30;
        send_data[1] = block_size;
        send_data[2] = 0x00;
        send_data_size = 3;
        SocketCAN::sendCANframe(send_data,send_data_size);
    }
    
    unsigned char* CAN_UDS_Layer::get_recieved_data()
    {
        return received_data;
    }
    
    unsigned int CAN_UDS_Layer::get_recieved_data_length()
    {
        return received_data_size;
    }
    
    void CAN_UDS_Layer::printreceiveddata()
    {
        for(int i = 0; i < received_data_size; i++)
        {
            printf("%X \n" , received_data[i]);
        }
    }
