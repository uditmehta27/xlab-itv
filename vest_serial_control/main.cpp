#include "mbed.h"
 DigitalOut myled(LED3);
PwmOut motor1(p25);
PwmOut motor2(p26);
PwmOut motor3(p23);
PwmOut motor4(p24);
PwmOut motor5(p22);
PwmOut motor6(p21);
Serial pc(USBTX, USBRX);
Timeout to1;
Timeout to2;
Timeout to3;
Timeout to4;
Timeout to5;
Timeout to6;
int m1,m2,m3,m4,m5;
// Timeout for vibration
void shot1() {

     motor1 = 0;
     myled = 0;
}
void shot2() {
     motor2 = 0;
}
void shot3() {
     motor3 = 0;
}
void shot4() {
     motor4 = 0;
}
void shot5() {
     motor5 = 0;
}
void shot6() {
     motor6 = 0;
}

void updateMotors() {
    
    if(m1) to1.attach(&shot1,0.2);
    if(m2) to2.attach(&shot2,.2);
    if(m3) to3.attach(&shot3,.2);
    if(m4) to4.attach(&shot4,.2);
    if(m5) to5.attach(&shot5,.2);
    //if(m6) to6.attach(&shot6,.2);
}
int main() {
    char i[256];
    while(1){
        if(pc.readable()){
              pc.gets(i,256);   
        switch(i[0]){
            
            case 'A': motor1 = i[1]/10.0;
                    m1 = 1;
                    myled = 1;
                    updateMotors();
                    break;
            case 'B': motor2 = i[1]/10.0;
                    m2 = 1;
                    myled = 0;
                    updateMotors();
                    break;
            case 'C': motor3 = i[1]/10.0;
                    m3 = 1;
                    updateMotors();
                    break;
            case 'D': motor4 = i[1]/10.0;
                    m4 = 1;
                    updateMotors();
                    break;
            case 'E': motor5 = i[1]/10.0;
                    m5 = 1;
                    updateMotors();
                    break;
            default:break;
        }
                    
        }
    }
}
