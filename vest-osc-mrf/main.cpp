#include "mbed.h"
#include "OSCmsg.h"
#include "MRF24J40.h"


#include <string>

// Timing delays for each of the vibrating motors
Timeout to1;
Timeout to2;
Timeout to3;
Timeout to4;

int m1,m2,m3,m4;

//Configuring the PWM pins
PwmOut motor1(p24);  //LB
PwmOut motor2(p22);
PwmOut motor3(p25); //LT
PwmOut motor4(p26);  //back top
                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
DigitalOut led1(LED1);

int motorStatus;
int intensity;
int rxval;

// Serial port for showing RX data.
Serial pc(USBTX,USBRX);

//Configuring the MRF24J240 Transceiver
MRF24J40 mrf(p11,p12,p13,p14,p21);

/**
* Receive data from the MRF24J40.
*
* @param data A pointer to a char array to hold the data
* @param maxLength The max amount of data to read.
*/
int rf_receive(char *data, uint8_t maxLength)
{
    uint8_t len = mrf.Receive((uint8_t *)data, maxLength);
    uint8_t header[8]= {1, 8, 0, 0xA1, 0xB2, 0xC3, 0xD4, 0x00};
    if(len > 10) {
        //Remove the header and footer of the message
        for(uint8_t i = 0; i < len-2; i++) {
            if(i<8) {
                //Make sure our header is valid first
                if(data[i] != header[i])
                    return 0;
            } else {
                data[i-8] = data[i];
            }
        }

        //pc.printf("Received: %s length:%d\r\n", data, ((int)len)-10);
    }
    return ((int)len)-10;
}


//Haptics vest: H <top-left> <top-right> <bot-left> <bot-right> <back-left> <back-right>

char txBuffer[128];
char rxBuffer[128];
int rxLen;


// Timeout for vibration
void shot1() {

     motor1  = 0;
     motor2 = 0;
     m1 = 0;
     m2 = 0;
       
}

void shot2() {
/*     motor2 = 0;
     m2 = 0;*/
}

void shot3() {
     motor3 = 0;
     m3 = 0;
}
void shot4() {
     motor4 = 0;
     m4  = 0;
}



// Call the functions to update the status of the motors (i.e vibrate the motors)
void updateMotors() {
    
    if(m1) to1.attach(&shot1,0.2);
    if(m2) to2.attach(&shot2,.2);
    if(m3) to3.attach(&shot3,.2);
    if(m4) to4.attach(&shot4,.2);
}

// From the received message find out which motors need to vibrate
int find_status(char *add){
    int motorstatus; 
    if(strncmp(add,"/B/LT",5)==0){
            motorstatus = 1000;
    }
    else if(strncmp(add,"/B/LB",5)==0){
        motorstatus = 100;
    }
    else if(strncmp(add,"/B/FR",5)==0){
        motorstatus = 1110;
    }
     else if(strncmp(add,"/B/HS",5)==0){
        motorstatus = 1111;
     }
    else if(strncmp(add,"/B/BA",5)==0){
         motorstatus = 1;
     }
    else motorstatus = 0; 
    
    
   return motorstatus;
                   
        
        
}
/********Motor Mappings*****************/
/*
    motor1 : LT
    motor2 : LB
    motor3 : RT
    motor4 : RB
    motor5 : BACK
    motor6 : BACK

*/
int main() {

    OSCclass *c=new OSCclass;
    OSCmsg *recv;

    
    uint8_t channel = 7;
    // Set the Channel. 0 is the default, 15 is max
    //mrf.SetChannel(channel);
    char add[5];
    char type[2];
    char *command;
    
    pc.printf("Start----- Haptic Vest!\r\n");
    
    // Set the default intensity of the motors to 9 (Scale 0 to 10)
    intensity = 9;
    
    while(1) {

        rxLen = rf_receive(rxBuffer, 128);
       // pc.printf("RxLen %d", rxLen);
        if(rxLen > 0) {
                    
            recv= c->getOSCmsg(rxBuffer);

            printf("Address is %s with type %c and msg %c \r\n",recv->getAddr(),recv->getType(),recv->getArgs());
            
            // Parse the received message to determine which motors have to vibrate
            strncpy(add,recv->getAddr(),5);
           
            if(add[1] == 'B'){
            
                    
                        motorStatus = find_status(add);
        
                        //pc.printf("motor status: %d\r\n", motorStatus);
        
                        //motor6 = motorStatus%10;//BACK
                        m4 = motorStatus%10;//BACK
                        if(m4 == 1){
                             motor4 = intensity/10.0;
                             pc.printf("Vibrate back\r\n");
                        }
                        m3 = (motorStatus / 1000) % 10;//LT
                        if(m3 == 1) motor3 = intensity/10.0;
                        
                        m1 = (motorStatus / 100) % 10;//LB
                        if(m1 == 1){ 
                            motor1 = intensity/10.0;
                            motor2 = intensity/10.0;
                        }    
                        
                        updateMotors();
                        
                        if(recv->getType() == 'I'){
                            
                            rxval = recv->getArgs();
                            
                            if(rxval == 'L')
                            {
                                pc.printf("LOW\r\n");
                                intensity = 0;
                                pc.printf("internsity changed to %d",intensity);
                            }
                            
                            if(rxval == 'M')
                            {
                                pc.printf("MEDIUM\r\n");
                                intensity = 5;
                                pc.printf("internsity changed to %d",intensity);
                            }
                            
                            if(rxval == 'H')
                            {
                                pc.printf("HIGH\r\n");
                                intensity = 9;
                                pc.printf("internsity changed to %d",intensity);
                            }
                            
                            if(rxval == 'A')
                            {
                                pc.printf("A: Back Left\r\n");
                                m4 = 1;
                                motor4 = intensity/10.0;
                                updateMotors();
                                
                            }
                            
                            if(rxval == 'C')
                            {
                                pc.printf("C: left top\r\n");
                                motor3 = intensity/10.0;
                                m3 = 1;
                                updateMotors();
                            }
                            

                            if(rxval == 'E')
                            {
                                motor1 = intensity/10.0;
                                m1 = 1;
                                pc.printf("E: Left bottom\r\n");
                                updateMotors();
                            }
                            
                            
                            
                            if(rxval == 'G')
                            {
                                motor4 = intensity/10.0;
                                motor3 = intensity/10.0;
                                motor1 = intensity/10.0;
                                m4 = 1;
                                m3 = 1;
                                m1 = 1;
                                pc.printf("G:ALL \r\n");
                                updateMotors();
                            }
                            
                            if(rxval == 'H')
                            {
                                motor4 = intensity/10.0;
                                m4 =1 ;
                                pc.printf("H: ALL back \r\n");
                                updateMotors();
                            }
                            
                            if(rxval == 'I')
                            {
                                motor3 = intensity/10.0;
                                m3 = 1;
                                pc.printf("I: Swipe \r\n");
                                updateMotors();
                            }
                            
                        }

                }
                rxBuffer[0]=0;
        }

    }
}
