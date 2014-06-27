#include "mbed.h"
#include "OSCmsg.h"
#include "MRF24J40.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>

char   rcvBuff[8]; 
Timeout to1;
Timeout to2;
Timeout to3;
int   buflength;

//Configuring pin 25 and 26 as PWM for vibrating the couch motors
PwmOut motor1(p25);
PwmOut motor2(p26);

int m1,m2,m3;
int num1, num2;
int num;
char number[10];
int n;
//int motor2;


// Serial port for showing RX data. Will be changed to Input Stream from USB
Serial pc(USBTX,USBRX);

DigitalOut led1(LED1);
DigitalOut song(p5);
PwmOut red(p23);
PwmOut green(p24);
PwmOut blue(p25);

// Used for sending
char inputBuffer[128];
char previousInput[128];

// Sports Data
short sportsflag = 0; //sports data needs updating
char *team1=0;
char *prev_team1=0;
char *team2 =0;
char *prev_team2 =0;
int score1=0;
int prev_score1=0;
int score2=0;
int prev_score2=0;
char *event=0;
char *prev_event=0;

//Action data
char *movie = 0;
char *mood = 0;
char *impact = 0;
char *shot = 0;

//RGB data
short rgbflag = 0;    //rgb data needs updating

// Timeout for vibration
void shot1() {

     motor1  = 0;
     m1 = 0;
       
}
void shot2() {
     motor2 = 0;
     m2 = 0;
}

void shot3() {

     motor1  = 0;
     motor2 = 0;
     m3 = 0;
      
}

void updateMotors() {
    
    if(m1) to1.attach(&shot1,0.2);
    if(m2) to2.attach(&shot2,.2);
    if(m3) to3.attach(&shot3,.2);
    
}

int main()
{
    OSCmsg m;

    OSCclass *c=new OSCclass;
    
    
    uint8_t channel = 7;

    pc.baud(115200);

    pc.printf("Start----- MASTER!\r\n");

    char *inputToken;
    motor2=1;
    wait_ms(5000);
    motor2 = 0;
    motor1 = 1;
    wait_ms(5000);
    motor1 = 0;
 
    while(1) {

        char buffer[30] = "V I 60\r\n";

        if (pc.readable()) {
            
            pc.gets(inputBuffer,128);
            pc.printf("received from serial %s\r\n",inputBuffer);

          //  if (strcmp(previousInput,inputBuffer) != 0) {

                sprintf(previousInput,"%s",inputBuffer);

                // parse the input received from the USB port, create corresponding OSC messages and send them over using ZigBee
                inputToken = strtok(inputBuffer, " ,");

                // Differentiate between different game modes: sports, action, etc....
                // Sports:
                if(strncmp(inputToken, "sports",6) == 0) {
                    team1 = strtok(NULL," ,");
                    team2 = strtok(NULL," ,");
                    score1 = atoi(strtok(NULL," ,"));
                    score2 = atoi(strtok(NULL," ,"));
                    event = strtok(NULL," \n");

                    char buffer[14];
                    sprintf(buffer, "S %d\r\n", (score1 * 10 + score2));

                    
                    printf("%s\r\n",event);

                    // Updates the event
                    if(strcmp(event, team1) == 0) {
                            m.setAddr("/A/WA");
                            m.setType('s');
                            m.setArgs('1');
                            c->sendOSCmsg(&m);
                            red = 1.0;
                            blue = 0.0;
                            green = 0.0f;
                            

                    } else if(strcmp(event, team2) == 0) {
                            m.setAddr("/A/WA");
                            m.setType('s');
                            m.setArgs('3');
                            c->sendOSCmsg(&m);
                            red = 0.0;
                            blue = 1.0;
                            green = 0.0f;

                    } else if(strncmp(event, "red",5) == 0) {
                            m.setAddr("/A/ST");
                            m.setType('s');
                            m.setArgs('1');
                            c->sendOSCmsg(&m);
                            

                    } else if(strncmp(event, "end",3) == 0) {
                            m.setAddr("/A/WA");
                            m.setType('s');
                            m.setArgs('2');
                            c->sendOSCmsg(&m);
                        
                    } 
                    else if(strncmp(event, "opening",7) == 0) {
                            m.setAddr("/A/RA");
                            m.setType('t');
                            m.setArgs('1');
                            c->sendOSCmsg(&m);
                            //sound reactive leds
                            song = 1;
                            red=0.0f;
                            blue=0.0f;
                            green=1.0f;
                            
                    }
                    else if(strncmp(event, "fight",5) == 0) {
                            m.setAddr("/A/ST");
                            m.setType('t');
                            m.setArgs('2');
                            c->sendOSCmsg(&m);

                    }
                    
                    else if(strncmp(event, "shootout",8) == 0) {
                            m.setAddr("/A/WA");
                            m.setType('s');
                            m.setArgs('2');
                            c->sendOSCmsg(&m);
                    }
                    
                    else if(strncmp(event, "miss",4) == 0) {
                            m.setAddr("/A/ST");
                            m.setType('s');
                            m.setArgs('4');
                            c->sendOSCmsg(&m);

                    }
                    else if(strncmp(event, "nil",3) == 0) {
                            m.setAddr("/A/NI");
                            m.setType('t');
                            m.setArgs('1');
                            c->sendOSCmsg(&m);
                            red = 0.0f;
                            blue=0.0f;
                            green=0.5f;
                            song = 0;

                    }
                    else if(strncmp(event,"vibrate",7)==0){
                            //send to the jacket mbed
                            m.setAddr("/B/HS");
                            m.setType('t');
                            m.setArgs('1');
                            c->sendOSCmsg(&m);
                    }
                    else if(strncmp(event,"music",7)==0){
                            //sound reactive led
                            song = 1;
                    }
                    else if(strncmp(event,"ends",7)==0){
                            //switch off sound reactive led
                            song = 0;
                    }
                } 
                if(strncmp(inputToken, "action",6) == 0) {
                    //action BoondockSaints <mood> <impact> <shot>
                    //moods: bright, indoor, dark, action
                    //impact: vibrators
                    //shot: right-top, right-bot, left-top, left-bot, nil
                    
                    movie = strtok(NULL, " ");
                    mood = strtok(NULL, " ");
                    impact = strtok(NULL, " ");
                    shot = strtok(NULL, " ");
                    
                    
                    if (strncmp(mood,"spider",6)==0){

                             m.setAddr("/A/PA");
                             m.setType('s');
                             m.setArgs('8');
                             c->sendOSCmsg(&m);
                             
                         
                        }
                     if (strncmp(mood,"fall",4)==0){

                             m.setAddr("/A/PA");
                             m.setType('s');
                             m.setArgs('9');
                             c->sendOSCmsg(&m);
                             
                         
                        }
                     if (strncmp(mood,"super1",6)==0){

                             m.setAddr("/A/PA");
                             m.setType('s');
                             m.setArgs('4');
                             c->sendOSCmsg(&m);
                             
                         
                        }

                    
                    if (strncmp(mood,"clear",5)==0){

                            m.setAddr("/A/PA");
                            m.setType('I');
                            m.setArgs('0');
                            c->sendOSCmsg(&m); 
                             
                         
                        }
                      if (strncmp(mood,"rocket",5)==0){

                             m.setAddr("/A/PA");
                             m.setType('s');
                             m.setArgs('2');
                             c->sendOSCmsg(&m);
                             
                         
                        }

                       if (strncmp(mood,"stop",4)==0){
                            
                             
                         
                        } 
                     if (strncmp(mood,"blue",4)==0){

                             m.setAddr("/A/PA");
                             m.setType('s');
                             m.setArgs('7');
                             c->sendOSCmsg(&m);
                             
                         
                        }
                     if (strncmp(mood,"nemo",4)==0){

                             m.setAddr("/A/PA");
                             m.setType('s');
                             m.setArgs('1');
                             c->sendOSCmsg(&m);
                             
                         
                        }
                     if (strncmp(mood,"beyo1",5)==0){

                             m.setAddr("/A/PA");
                             m.setType('s');
                             m.setArgs('3');
                             c->sendOSCmsg(&m);
                             
                         
                        }
                     if (strncmp(mood,"beyo2",5)==0){

                             m.setAddr("/A/PA");
                             m.setType('s');
                             m.setArgs('6');
                             c->sendOSCmsg(&m);
                             
                         
                        }
                     if (strncmp(mood,"beyo3",5)==0){

                             m.setAddr("/A/PA");
                             m.setType('s');
                             m.setArgs('5');
                             c->sendOSCmsg(&m);
                             
                         
                        }

                        else if(strncmp(mood,"indoor",6)==0){//TODO

                            m.setAddr("/A/ON");
                            m.setType('s');
                            m.setArgs('5');
                            c->sendOSCmsg(&m);
                         }
                         else if(strncmp(mood,"music",5)==0){//TODO

                            song=1;
                         }
                         else if(strncmp(mood,"dark",4)==0){

                            m.setAddr("/A/NI");
                            m.setType('t');
                            m.setArgs('7');
                            c->sendOSCmsg(&m); 
                             red = 0.0f;
                             green=0.0f;
                             blue=0.0f;
                        } 
                        else if(strncmp(mood,"action",6)==0){

                            m.setAddr("/A/ST");
                            m.setType('s');
                            m.setArgs('1');
                            c->sendOSCmsg(&m);
                        } 
                        else if(strncmp(mood,"strobe",6)==0){

                            m.setAddr("/A/ST");
                            m.setType('s');
                            m.setArgs('4');
                            c->sendOSCmsg(&m);
                        } 
                         else if(strncmp(mood,"done",6)==0){

                            m.setAddr("/A/NI");
                            m.setType('s');
                            m.setArgs('4');
                            c->sendOSCmsg(&m);
                            red = 1.0f;
                             green=1.0f;
                             blue=1.0f;
                        } 
                     
                        else if(strncmp(mood,"nil",3)==0){
                       // printf("mood %s\r\n",mood);
                             //rf_send("Nil\r\n",6);
                        }
                        
                        if(strncmp(impact,"int1",4)==0){
                            pc.printf("vest mbed 1 OSC\r\n");
                            m.setAddr("/B/IN");
                            m.setType('I');
                            m.setArgs(shot[0]);
                            c->sendOSCmsg(&m);
                    
                        }
                        
                        
                        if(strncmp(impact,"int2",4)==0){
                            pc.printf("vest mbed 2 OSC\r\n");
                            m.setAddr("/C/IN");
                            m.setType('I');
                            m.setArgs(shot[0]);
                            c->sendOSCmsg(&m);
                    
                        }
                         else if(strncmp(impact,"pat",3)==0){
                            pc.printf("Intensity OSC\r\n");
                            m.setAddr("/A/PA");
                            m.setType('I');
                            m.setArgs(shot[0]);
                            c->sendOSCmsg(&m);        
                        }
                        else if(strncmp(impact,"vib",3)==0){
                            pc.printf("couch OSC\r\n");
                            if(shot[0] == 'L'){
                                printf("you set me to low\r\n");
                                motor1 = 0.5;
                                motor2 = 0.5;
                            }  
                            else if(shot[0] == 'M'){
                                      printf("you set me to med\r\n");
                                motor1 = 0.75;
                                motor2 = 0.75;
                            }
                            
                             else if(shot[0] == 'H'){
                                      printf("you set me to high\r\n");
                                motor1 = 1;
                                motor2 = 1;
                            }
                             else if(shot[0] == 'J'){
                                      printf("vin back\r\n");
                                m1 = 1;
                                 updateMotors();
                            }
                             else if(shot[0] == 'K'){
                                 printf("vin front\r\n");
                                 m1 = 2;
                                 updateMotors();
                            }
                                  
                        }
                        
                       if(strncmp(impact,"cback",5)==0){
                             m1 = 1;
                            if(strncmp(shot,"low",3)==0) {
                                pc.printf("couch-back-low\r\n");
                                 motor1 = 0.5;
                            }
                             else if(strncmp(shot,"med",3)==0) {
                                 motor1 = 0.75;
                                 pc.printf("couch-back-medium\r\n");
                            }
                             else if(strncmp(shot,"high",4)==0) {
                                 motor1 = 1;
                                 pc.printf("couch-back-high\r\n");
                            }
                            updateMotors();
         
                           
                         }
                         
                         if(strncmp(impact,"cboth",5)==0){
                             m3 = 1;
                            if(strncmp(shot,"low",3)==0) {
                                
                                 motor1 = 0.5;
                                  motor2 = 0.5;
                                  pc.printf("couch-both-low\r\n");
                            }
                             else if(strncmp(shot,"med",3)==0) {
                                 motor1 = 0.75;
                                 motor2 = 0.75;
                                 pc.printf("couch-both-medium\r\n");
                            }
                             else if(strncmp(shot,"high",4)==0) {
                                 motor1 = 1;
                                  motor2 = 1;
                                  pc.printf("couch-both-high\r\n");
                            }
                            updateMotors();
         
                           
                         }
                         
                        else if(strncmp(impact,"cfront",6)==0){
                            m2 =1 ;
                            
                            if(strncmp(shot,"low",3)==0) {
                                 motor2 = 0.5;
                                 pc.printf("couch-front-low\r\n");
                            }
                             else if(strncmp(shot,"med",6)==0) {
                                 motor2 = 0.75;
                                 pc.printf("couch-front-medium\r\n");
                            }
                             else if(strncmp(shot,"high",4)==0) {
                                 motor2 = 1;
                                 pc.printf("couch-front-high\r\n");
                            }
                            updateMotors();
                          
                        }
                        
                        
                        else if(strncmp(shot,"left-top",8)==0){

                            m.setAddr("/B/LT");
                            m.setType('t');
                            m.setArgs('1');
                            c->sendOSCmsg(&m);
                        }
                        else if(strncmp(shot,"left-bot",8)==0){

                            m.setAddr("/B/LB");
                            m.setType('t');
                            m.setArgs('1');
                            c->sendOSCmsg(&m);
                        }
                        else if(strncmp(shot,"back",4)==0){
                            pc.printf("In back\r\n");
                            m.setAddr("/B/BA");
                            m.setType('t');
                            m.setArgs('1');
                            c->sendOSCmsg(&m);  
                            
                            m.setAddr("/C/BA");
                            m.setType('t');
                            m.setArgs('1');
                            c->sendOSCmsg(&m);
                        }
                        else if(strncmp(shot,"head",4)==0){
                            pc.printf("In head\r\n");
                            m.setAddr("/B/HS");
                            m.setType('t');
                            m.setArgs('1');
                            c->sendOSCmsg(&m); 
                            
                            m.setAddr("/C/HS");
                            m.setType('t');
                            m.setArgs('1');
                            c->sendOSCmsg(&m);    
                        }
                        else if(strncmp(shot,"front",5)==0){

                            m.setAddr("/B/FR");
                            m.setType('t');
                            m.setArgs('1');
                            c->sendOSCmsg(&m); 
                            
                            m.setAddr("/C/FR");
                            m.setType('t');
                            m.setArgs('1');
                            c->sendOSCmsg(&m);     
                        }
            
            }

       // }
    }
    
}
}
