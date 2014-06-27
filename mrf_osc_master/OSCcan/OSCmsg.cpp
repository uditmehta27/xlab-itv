#include "mbed.h"
#include "OSCmsg.h"
#include "MRF24J40.h"

MRF24J40 mrf(p11,p12,p13,p14,p21);

//The function to send the data over zigbee using the  MRF24J40 modules
void rf_send(char *data, uint8_t len)
{   printf("sending\r\n");

    //We need to prepend the message with a valid ZigBee header
    uint8_t header[8]= {1, 8, 0, 0xA1, 0xB2, 0xC3, 0xD4, 0x00};
    uint8_t *send_buf = (uint8_t *) malloc( sizeof(uint8_t) * (len+8) );

    for(uint8_t i = 0; i < len+8; i++) {
        //prepend the 8-byte header
        send_buf[i] = (i<8) ? header[i] : data[i-8];
    }
    
    mrf.Send(send_buf, len+8);
    free(send_buf);
}

//Constructor initialzing the OSC message to defaults
OSCmsg::OSCmsg(){
    type=0;
    msg=0;
    length=0;

}

//Method to set the address field in the message
int OSCmsg::setAddr(char* addr){

        strncpy(address,addr,5);
        address[5]=0;
        return 1;
        
 }
 
 //Getter for the address field of the message
 char* OSCmsg::getAddr(){
        return address;
 }
 
 //Method to set the type of the message.
 //type can be 'time','int','float','string'
 int OSCmsg::setType(char mtype){
        type = mtype;
         return 1;
  }
//Getter for the message type
  char OSCmsg::getType(){
        return type;
  }
  
  //Method to send any additional arguments in the message
  int OSCmsg::setArgs(char msgs){ 
  
        msg = msgs;
        return 1;
    
  }
  //Getter for the additional arguments
  char OSCmsg::getArgs(){
        return msg;
  }
  

  /****************functions for OSC class***********************/

  
OSCclass::OSCclass(){}
 //MEthod to unpack the OSC message and send it using the zigbee modules. It retrieves the values in all the fields of the OSC object and creates an
 //appropriate OSC message.
 void OSCclass::sendOSCmsg(OSCmsg *sendmsg){
            
        
        if(sendmsg->getType() == 0)
            ttype = 'n';
         else ttype = sendmsg->getType();
         
         strncpy(addr,sendmsg->getAddr(),5);
        
        strncpy(packet,sendmsg->getAddr(),5);
        packet[5] = ',';
        packet[6]= ttype;
        packet[7] = sendmsg->getArgs();
        packet[8] = 0;

        printf("OSc mesage %s\r\n",packet);
        //Send this message on the zigbee

        rf_send(packet,8);
        printf("sent %s %d \r\n",packet,strlen(packet));
        
  }
  //This method retrieves the values from the received OSC message. 
  //It creates a new OSC object and sets the appropriate fields in the object using the information in the message.
  OSCmsg* OSCclass::getOSCmsg(unsigned char *recv){

                
                temp = new OSCmsg;
                addr[0] = recv[0];
                addr[1] = recv[1];
                addr[2] = recv[2];
                addr[3] = recv[3];
                addr[4] = recv[4];
                
                temp->setAddr(addr);
                temp->setType(recv[6]);
                temp->setArgs(recv[7]);

        
                return temp;
        
        
  
  
  }
        
  
  
  
  
  