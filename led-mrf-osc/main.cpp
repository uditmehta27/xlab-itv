#include "mbed.h"
#include<string.h>
#include <vector>
#include "rtos.h"
#include "OSCmsg.h"
#include "MRF24J40.h"
#include "LPD8806.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#define ROWS 8          // Rows of leds
#define COLS 66         // number of leds per row

Serial pc(USBTX, USBRX);    // Serial communication for debugging purposes
MRF24J40 mrf(p11,p12,p13,p14,p21);
Mutex stdio_mutex; 

// Total leds used for light effects; ROWS * COLS
LPD8806 strip = LPD8806(528);

// Since the led strip is daisy chained, it is harder to create effects with a list compared to a regular 2d matrix.
static int grid[8][66] = {10};


/****************************************
* finding_nemo_set_position
*
* @param p      position of the head led within the row  
*
*
****************************************/
void finding_nemo_set_position(int p)
{
    uint8_t r = 0; uint8_t g = 0; uint8_t b = 255;
    int pos;
    for(pos = 1; pos < 66; pos++)
        strip.setPixelColor(p-pos, strip.Color(r/(pos), g/(pos), b/(pos)));
}

/****************************************
* finding_nemo
*
* creates a water-flow effect that is in sync with the scene from the movie finding nemo
* 
* execution time is approximately 50 ms
*
* @param r      red color value
* @param g      green color  
* @param b      blue color   
* @param delay  delay between updating the led layout to "move the water" by 1 led
*
****************************************/
void finding_nemo(uint8_t r, uint8_t g, uint8_t b, uint8_t delay) {
    int i, j;

    int pos = 0;
    int pos2 = pos+66;
    int pos3 = pos2+66;
    int pos4 = pos3+66;
    int pos5 = pos4+66;
    int pos6 = pos5+66;
    int pos7 = pos6+66;
    int pos8 = pos7+66;
    
    int dir = 1;
    int dir2 = 1;
    int dir3 = 1;
    int dir4 = 1;
    int dir5 = 1;
    int dir6 = 1;
    int dir7 = 1;
    int dir8 = 1;
    
    
    for (i=0; i<=((strip.numPixels()/8)); i++) 
    {
        
        finding_nemo_set_position(pos);
        finding_nemo_set_position(pos2);
        finding_nemo_set_position(pos3);
        finding_nemo_set_position(pos4);
        finding_nemo_set_position(pos5);
        finding_nemo_set_position(pos6);
        finding_nemo_set_position(pos7);
        finding_nemo_set_position(pos8);
        strip.show();
        wait_ms(delay);

        for (j=0; j< 66; j++)
        {
            strip.setPixelColor(pos-j, strip.Color(0,0,0));
            strip.setPixelColor(pos2-j, strip.Color(0,0,0));
            strip.setPixelColor(pos3-j, strip.Color(0,0,0));
            strip.setPixelColor(pos4-j, strip.Color(0,0,0));
            strip.setPixelColor(pos5-j, strip.Color(0,0,0));  
        }
        // Bounce off ends of strip
        pos += dir;
        pos2 += dir2;
        pos3 += dir3;
        pos4 += dir4;
        pos5 += dir5;
        pos6 += dir6;
        pos7 += dir7;
        pos8 += dir8;
        
        
        if(pos < 0)
        {
            pos = 1;
            dir = -dir;   
        }
        else if (pos >= strip.numPixels()/8)
        {
            pos = strip.numPixels()/8 - 2;
            dir = -dir;
        }
        
        if(pos2 < strip.numPixels()/8)
        {
            pos2 = strip.numPixels()/8;
            dir2 = -dir2;   
        }
        else if (pos2 >= (strip.numPixels()/8) * 2)
        {
            pos2 = (strip.numPixels()/8) * 2;
            dir2 = -dir2;
        }
        
        if(pos3 < (strip.numPixels()/8) * 2 + 1)
        {
            pos3 = (strip.numPixels()/8) * 2;
            dir3 = -dir3;   
        }
        else if (pos3 >= (strip.numPixels()/8) * 3)
        {
            pos3 = (strip.numPixels()/8) * 3;
            dir3 = -dir3;
        }
        
        if(pos4 < (strip.numPixels()/8) * 3 + 1)
        {
            pos4 = (strip.numPixels()/8) * 3 + 1;
            dir4 = -dir4;   
        }
        else if (pos4 >= (strip.numPixels()/8) * 4)
        {
            pos4 = (strip.numPixels()/8) * 4;
            dir4 = -dir4;
        }
        
        if(pos5 < (strip.numPixels()/8) * 4 + 1)
        {
            pos5 = (strip.numPixels()/8) * 4 + 1;
            dir5 = -dir5;   
        }
        else if (pos5 >= (strip.numPixels()/8) * 5)
        {
            pos5 = (strip.numPixels()/8) * 5;
            dir5 = -dir5;
        }
        
        if(pos6 < (strip.numPixels()/8) * 5 + 1)
        {
            pos6 = (strip.numPixels()/8) * 5 + 1;
            dir6 = -dir6;   
        }
        else if (pos6 >= (strip.numPixels()/8) * 6)
        {
            pos6 = (strip.numPixels()/8) * 6;
            dir6 = -dir6;
        }
        
        if(pos7 < (strip.numPixels()/8) * 6 + 1)
        {
            pos7 = (strip.numPixels()/8) * 6 + 1;
            dir7 = -dir7;   
        }
        else if (pos7 >= (strip.numPixels()/8) * 7)
        {
            pos7 = (strip.numPixels()/8) * 7;
            dir7 = -dir7;
        }
        
        if(pos8 < (strip.numPixels()/8) * 7 + 1)
        {
            pos8 = (strip.numPixels()/8) * 7 + 1;
            dir8 = -dir8;   
        }
        else if (pos8 >= (strip.numPixels()/8) * 8)
        {
            pos8 = (strip.numPixels()/8) * 8;
            dir8 = -dir8;
        }
        
    }
}

/****************************************
* init_grid()
*
* converting an array into a grid to make effect creating easier
* 
* execution time is approximately 10 ms
*
*
****************************************/

void init_grid()
{
    int col;
    
    for(col = 0; col < strip.numPixels()/ROWS; col++)
        grid[0][col] = col;
        
    for(col = 0; col < strip.numPixels()/ROWS; col++)
        grid[1][col] = strip.numPixels()/ROWS * 2 - 1 - col;
        
    for(col = 0; col < strip.numPixels()/ROWS-1; col++)
        grid[2][col] = strip.numPixels()/ROWS * 2 + col;
        
    for(col = 0; col < strip.numPixels()/ROWS-1; col++)
        grid[3][col] = strip.numPixels()/ROWS * 4 - 1 - col;
        
    for(col = 0; col < strip.numPixels()/ROWS-1; col++)
        grid[4][col] = strip.numPixels()/ROWS * 4 + col;
        
    for(col = 0; col < strip.numPixels()/ROWS-1; col++)
        grid[5][col] = strip.numPixels()/ROWS * 6 - 1 - col;
        
    for(col = 0; col < strip.numPixels()/ROWS-1; col++)
        grid[6][col] = strip.numPixels()/ROWS * 6 + col;
        
    for(col = 0; col < strip.numPixels()/ROWS-1; col++)
        grid[7][col] = strip.numPixels()/ROWS * 8 - 1 - col;        
}

/****************************************
* rf_receive
*
* function to recieve the command from the main mbeda
* the appropriate effect is executed based on that command
*
* @param data       actualy command value padded with the protocol headers
* @param maxLength  data maxLength 
*
****************************************/
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


/****************************************
* super_bowl_wave
*
* creates a flow effect that is in sync with the super bowl touchdown scene; 
* the rgb value can be changed so that the color can match the color of the team that scored
* 
* execution time is approximately 25 ms
*
* @param r      red color value
* @param g      green color  
* @param b      blue color   
* @param delay  delay between updating the led layout to "move the color" by 1 led
*
****************************************/
void super_bowl_wave(uint8_t r, uint8_t g, uint8_t b, uint8_t delay) {
    int i;

    int pos = 0;
    int pos2 = pos+strip.numPixels()/ROWS * 2 - 1;
    int pos3 = pos2+1;
    int pos4 = pos3+strip.numPixels()/ROWS * 2 - 1;
    int pos5 = pos4+1;
    int pos6 = pos5+strip.numPixels()/ROWS * 2 - 1; 
    int pos7 = pos6+1;
    int pos8 = pos7+strip.numPixels()/ROWS * 2 - 1;

    for (i=0; i<8; i++)
    {
        strip.setPixelColor(pos4, strip.Color(r, g, b));
        strip.setPixelColor(pos5, strip.Color(r, g, b));
        pos4--;
        pos5++;
        strip.show();
        wait_ms(delay);
    }
    pos4 = pos3+strip.numPixels()/ROWS * 2 - 1;
    pos5 = pos4+1;
    for (i=0; i<(strip.numPixels()/ROWS); i++) 
    {
        
        strip.setPixelColor(pos-2, strip.Color(r, g, b));
        strip.setPixelColor(pos2+4, strip.Color(r, g, b));
        strip.setPixelColor(pos3, strip.Color(r, g, b));
        strip.setPixelColor(pos4-8, strip.Color(r, g, b));
        strip.setPixelColor(pos5+8, strip.Color(r, g, b));
        strip.setPixelColor(pos6, strip.Color(r, g, b));
        strip.setPixelColor(pos7-4, strip.Color(r, g, b));
        strip.setPixelColor(pos8+2, strip.Color(r, g, b));
    
        strip.show();
        wait_ms(delay);
        pos++;
        pos2--;
        pos3++;
        pos4--;
        pos5++;
        pos6--;
        pos7++;
        pos8--;   
    }
    
    for (i=0; i<((strip.numPixels())); i++)
        strip.setPixelColor(i, strip.Color(0,0,0));
    strip.show();
}


// Used for sending and receiving
char txBuffer[128];
char rxBuffer[128];
int rxLen;

typedef enum {NEMO,ROCKET,BEYONCE,BOWL,NILL1,FLASH1,DEFAULT,OFF} stat;
volatile stat status = DEFAULT;
        


/****************************************
* led_control
*
* the control function; after decoding the command recieved from the main mbed, 
* this functions makes the appropriate function call to saisfy that command;
*
* @param args      decoded command received from mbed
*
****************************************/
void led_control(void const* args){
    while(1){
      // printf("in led control   : %s\r\n",status);
        if(status == NEMO){
            printf("NEMO\r\n");
           //finding_nemo(0,0,255,50);
        }
         if(status == BEYONCE){
           stdio_mutex.lock();
           pc.printf("BEYONCE\r\n");
           super_bowl_wave(0,255,0, 1);
           stdio_mutex.unlock();
        }
        
         if(status == ROCKET){
           printf("ROCKET\r\n"); 
           //rocket_launch(63, 255, 255,120);
           status = DEFAULT;  
        }
        
         if(status == BOWL){
          printf("BOWL\r\n");
          //super_bowl_wave(0,255,0, 1);
         
         
        }
         if(status == NILL1){
           printf("nill\r\n");
           //turnOffStrip();
           status = DEFAULT;
        }
         
        if(status == OFF){
           //switch_off();
           //turnOffStrip();
           //strobe(strip.Color(255,0, 0),100);
           status = DEFAULT;
        }          
}

}


/****************************************
* beyonce_inctro
*
* creates a light-flash effect to mimic the camera flashes when beyonce enters the stage in the concert scene
* 
* execution time is approximately 5 ms
*
* @param r      red color value
* @param g      green color  
* @param b      blue color   
* @param delay  delay to hold the random leds for a specific time before clearing; very small as this function mimics a camera flash
*
****************************************/
void beyonce_intro(uint8_t r, uint8_t g, uint8_t b, int delay) 
{
    int grey, lGrey, dGrey, white, i;
    for (i=0; i<((strip.numPixels())); i++)
        strip.setPixelColor(i, strip.Color(0,0,0));
    strip.show();
    
    for (i=0; i<(8); i++)
    {
        grey = rand() % strip.numPixels();
        lGrey = rand() % strip.numPixels();
        dGrey = rand() % strip.numPixels();
        white = rand() % strip.numPixels();
        strip.setPixelColor(grey, strip.Color(64,64,64));
        strip.setPixelColor(grey+1, strip.Color(64,64,64));
        strip.setPixelColor(lGrey, strip.Color(8,8,8));
        strip.setPixelColor(lGrey+1, strip.Color(8,8,8));
        strip.setPixelColor(dGrey, strip.Color(192,192,192));
        strip.setPixelColor(dGrey+1, strip.Color(192,192,192));
        strip.setPixelColor(white, strip.Color(255,255,255));
    }
    strip.show();
    wait_ms(delay);
}

/****************************************
* rocket_launch
*
* a sequence of light effects measured out o match the rocket launch sequence
* the light turns from a light blue shade to a dark purple shade as the rocket blasts off in to the space
* as the sequence shifts to a scene from gravity, the lights change in brightness to give a twinkling star effect
* 
* execution time is approximately 50 seconds
*
* @param r      red color value
* @param g      green color  
* @param b      blue color   
* @param delay  delay between changing the shade to match the sky background in the rocket launch sequence
*
****************************************/
void rocket_launch(uint8_t r, uint8_t g, uint8_t b, int delay) 
{
    int i, j;
    int array[30] = {0};
    int star = 0;
                
    while(r)
    {
        wait_ms(delay);
        for (i=0; i<((strip.numPixels())); i++)
        {
            strip.setPixelColor(i, strip.Color(r,g,b));
        } 
        strip.show(); 
        g--;
        r--;
    }

    while(g>128)
    {
        wait_ms(delay);
        for (i=0; i<((strip.numPixels())); i++)
        {
            strip.setPixelColor(i, strip.Color(r,g,b));
        } 
        strip.show(); 
        g--;
    }

    while(b>128)
    {
        wait_ms(delay);
        for (i=0; i<((strip.numPixels())); i++)
        {
            strip.setPixelColor(i, strip.Color(r,g,b));
        } 
        strip.show(); 
        b--;
    }
    for(i = 0; i<25; i++)
    {
        array[i] = rand() % strip.numPixels();
        star = rand() % 255;
        strip.setPixelColor(array[i], strip.Color(star, star, star));
    }
    strip.show();
    for(i =0; i < 500; i++)
    {
        for(j = 0; j < 5; j++)
        {
            star = rand() % 255;
            strip.setPixelColor(array[rand() % 25], strip.Color(star, star, star));
        }
        strip.show();
        wait_ms(99);
    }
    
    /*for(i = 0; i < 500; i++)
    {
        for(j = 0; j < 5; j++)
        {
            star = rand() % 255;
            strip.setPixelColor(array[rand() % 25], strip.Color(star, star, star));
        }
        strip.show();
        wait_ms(500);
    } */
}

/****************************************
* beyonce_middle
*
* creates a red-light effect to match the middle portion of beyonce's concert sequence
* 
* execution time is approximately 10 ms
*
* @param r      red color value
* @param g      green color  
* @param b      blue color   
* @param delay  delay between updating the led layout to "move the effect" by 1 led
*
****************************************/
void beyonce_middle(uint8_t r, uint8_t g, uint8_t b, int delay) 
{
    int row, col;
    int x;
    int y = 10;
    for(x = 0; x < 33; x++)
    {
        for(row = 0; row < ROWS; row++)
            for(col = 0; col < x; col++)
            {
                strip.setPixelColor(grid[row][col], strip.Color(r,g,b));
                strip.setPixelColor(grid[row][COLS - 1 - col], strip.Color(r,g,b));
            }
                
        strip.show();
        wait_ms(delay);
        for (row=0; row<((strip.numPixels())); row++)
            strip.setPixelColor(row, strip.Color(0,0,0));
    }
    
    for(x = 33; x >0; x--)
    {
        for(row = 0; row < ROWS; row++)
            for(col = 0; col < x; col++)
            {
                strip.setPixelColor(grid[row][col], strip.Color(r,g,b));
                strip.setPixelColor(grid[row][COLS - 1 - col], strip.Color(r,g,b));
            }
                
        strip.show();
        wait_ms(delay);
        for (row=0; row<((strip.numPixels())); row++)
            strip.setPixelColor(row, strip.Color(0,0,0));
    }
}


/****************************************
* fill_color_blue
*
* creates a under water effect that is in sync with some scenes from the movie finding nemo
* 
* execution time is approximately 1 ms
*
* @param r      red color value
* @param g      green color  
* @param b      blue color   
* @param delay  delay not used in this sequence anymore
*
****************************************/
void fill_color_blue(uint8_t r, uint8_t g, uint8_t b, int delay) 
{
    int x;
    /*
    int row, col;
    
    int y = 10;
    for(x = 0; x < 33; x++)
    {
        for(row = 0; row < ROWS; row++)
            for(col = 0; col < x; col++)
            {
                strip.setPixelColor(grid[row][col], strip.Color(r,g,b));
                strip.setPixelColor(grid[row][COLS - 1 - col], strip.Color(r,g,b));
            }
                
        strip.show();
        wait_ms(delay);
    }
    */
    for(x = 0; x < (strip.numPixels()); x++)
    {
        strip.setPixelColor(x, strip.Color(r,g,b));
    }
    strip.show();
    
}

/****************************************
* finding_nemo
*
* clears all the leds. picth black ceiling
* 
* execution time is approximately 1 ms
*
****************************************/
void clear()
{
    int x;
    for (x=0; x<((strip.numPixels())); x++)
        strip.setPixelColor(x, strip.Color(0,0,0));
    strip.show();
}


/****************************************
* equalizer
*
* creates a equalizer effect that is in sync with the ending part of beyonce's concert sequence
* 
* execution time is approximately 15 ms
*
* @param r      red color value
* @param g      green color  
* @param b      blue color   
* @param delay  delay between updating the led layout to "move the equalizer" by 1 led up or down
*
****************************************/
void equalizer(uint8_t r, uint8_t g, uint8_t b, int delay)
{
    int i;
    int row = 0;
    int col = 0;
    
    int randValues[14] = {0};
    for(i = 0 ; i < 14; i++)
        randValues[i] = rand() % 7;
    int temp = rand() % 7;
    
    for(i = temp; i >=0; i--)
    {
        for(row = 7; row > i; row--)
        {
            for(col = 0; col < 3; col++)
            {
                strip.setPixelColor(grid[row+randValues[0]][col], strip.Color(r,g,b));
                strip.setPixelColor(grid[row+randValues[1]][col+5], strip.Color(r,g,b));
                strip.setPixelColor(grid[row+randValues[2]][col+10], strip.Color(r,g+r,b));
                strip.setPixelColor(grid[row+randValues[3]][col+15], strip.Color(r,g+r,b));
                strip.setPixelColor(grid[row+randValues[4]][col+20], strip.Color(r,g+r,b));
                strip.setPixelColor(grid[row+randValues[5]][col+25], strip.Color(r,g,b));
                strip.setPixelColor(grid[row+randValues[6]][col+30], strip.Color(r,g,b));
                strip.setPixelColor(grid[row+randValues[7]][col+35], strip.Color(r,g+r,b));
                strip.setPixelColor(grid[row+randValues[8]][col+40], strip.Color(r,g,b));
                strip.setPixelColor(grid[row+randValues[9]][col+45], strip.Color(r,g+r,b));
                strip.setPixelColor(grid[row+randValues[10]][col+50], strip.Color(r,g,b));
                strip.setPixelColor(grid[row+randValues[11]][col+55], strip.Color(r,g,b));
                strip.setPixelColor(grid[row+randValues[12]][col+60], strip.Color(r,g+r,b));
                strip.setPixelColor(grid[row+randValues[13]][col+65], strip.Color(r,g,b));
            }
        }
        

        strip.show();
        wait_ms(delay);
        for (row=0; row<((strip.numPixels())); row++)
            strip.setPixelColor(row, strip.Color(0,0,0));
    }
    
    for(i = 0; i < temp; i++)
    {
        for(row = 7; row > i; row--)
        {
            for(col = 0; col < 3; col++)
            {
                strip.setPixelColor(grid[row+randValues[0]][col], strip.Color(r,g,b));
                strip.setPixelColor(grid[row+randValues[1]][col+5], strip.Color(r,g,b));
                strip.setPixelColor(grid[row+randValues[2]][col+10], strip.Color(r,g+r,b));
                strip.setPixelColor(grid[row+randValues[3]][col+15], strip.Color(r,g+r,b));
                strip.setPixelColor(grid[row+randValues[4]][col+20], strip.Color(r,g+r,b));
                strip.setPixelColor(grid[row+randValues[5]][col+25], strip.Color(r,g,b));
                strip.setPixelColor(grid[row+randValues[6]][col+30], strip.Color(r,g,b));
                strip.setPixelColor(grid[row+randValues[7]][col+35], strip.Color(r,g+r,b));
                strip.setPixelColor(grid[row+randValues[8]][col+40], strip.Color(r,g,b));
                strip.setPixelColor(grid[row+randValues[9]][col+45], strip.Color(r,g+r,b));
                strip.setPixelColor(grid[row+randValues[10]][col+50], strip.Color(r,g,b));
                strip.setPixelColor(grid[row+randValues[11]][col+55], strip.Color(r,g,b));
                strip.setPixelColor(grid[row+randValues[12]][col+60], strip.Color(r,g+r,b));
                strip.setPixelColor(grid[row+randValues[13]][col+65], strip.Color(r,g,b));
            }
        }
        strip.show();
        wait_ms(delay);
        for (row=0; row<((strip.numPixels())); row++)
            strip.setPixelColor(row, strip.Color(0,0,0));
    }
    
}


/****************************************
* spider_man
*
* creates a sequence of effects that is in sync with the trailer from the movie the mazing spider man 2
* 
* execution time is approximately 20 seconds
*
* @param r      red color value
* @param g      green color  
* @param b      blue color   
* @param delay  delay between updating the led layout to match the effect timings in the sequence
*
****************************************/
void spider_man(uint8_t r, uint8_t g, uint8_t b, int delay)
{
    wait_ms(8000);
    int row, col, num, repeat;
    int color;
    for(repeat = 0; repeat < 1; repeat++)
    {
        for(num = 0; num < 4; num++)
        {
            for(row = 3-num; row < 5+num; row++)
            {
                for(col = 25-num*6; col < 41+num*6; col++)
                {
                    strip.setPixelColor(grid[row][col], strip.Color(255,192,0));
                }
            }
              strip.show();
              wait_ms(delay);
        }
        for (row=0; row<((strip.numPixels())); row++)
            strip.setPixelColor(row, strip.Color(0,0,0));
        strip.show();
    }
    for (row=0; row<((strip.numPixels())); row++)
        strip.setPixelColor(row, strip.Color(0,0,0));
    strip.show();
    wait_ms(8250);
    
    for(num = 0; num < 8; num++)
    {
        for(row = 0; row < num; row++)
        {
            for(col = 0; col < 8*num; col++)
            {
                color = rand() % 255;
                strip.setPixelColor(grid[row][col], strip.Color(color,color,color));
            }
        }
          strip.show();
          wait_ms(100);
    }
    
    
    for (row=0; row<((strip.numPixels())); row++)
        strip.setPixelColor(row, strip.Color(0,0,0));
    strip.show();
    wait_ms(4750);
    
    for(row = 0; row < ROWS; row++)
    {
        for(col = 0; col < 50; col++)
        {
            strip.setPixelColor(grid[row][col], strip.Color(192+rand()%60,0,0));
        }
    }
    strip.show();
    wait_ms(2000);
    for (row=0; row<((strip.numPixels())); row++)
        strip.setPixelColor(row, strip.Color(0,0,0));
    strip.show();
}


/****************************************
* finding_nemo_stream
*
* creates a water-flow effect that is in sync with the scene from the movie finding nemo
* 
* execution time is approximately 50 ms
*
* @param r      red color value
* @param g      green color  
* @param b      blue color   
* @param delay  delay between updating the led layout to "move the water" by 1 led
*
****************************************/
void nemo_stream(uint8_t r, uint8_t g, uint8_t b, int delay) 
{
    int i;
    int row = 0;
    int col = 0;
    int color= 0;
    int randCols[20] = {0};
    int randRows[20] = {0};
    for(i = 0 ; i < 20; i++)
    {
        randCols[i] = rand() % 66;
        randRows[i] = rand() % 7;
    }
    
    for(i = 7; i >=0; i--)
    {
        color = rand() % 10;
        for(row = 7; row > i; row--)
        {
            for(col = 0; col < 30; col++)
            {
                
                strip.setPixelColor(grid[row][randCols[col]], strip.Color(r,(rand()%32)/2/row,b/row));
                if(color<2)
                    strip.setPixelColor(grid[row][randCols[col]], strip.Color(16,16,16));
                strip.setPixelColor(grid[row+6][randCols[col]],strip.Color(0,0,0));
            }           
        }
        
        strip.show();
        wait_ms(50);
    }
    
    for (row=0; row<((strip.numPixels())); row++)
        strip.setPixelColor(row, strip.Color(0,0,10));
}

/****************************************
* gravity_falling_to_earth
*
* creates a debris effect that is in sync with spacecraft falling back to earth scene from the movie gravity
* 
* execution time is approximately 20 ms
*
* @param r      red color value
* @param g      green color  
* @param b      blue color   
* @param delay  delay between updating the led layout to "move the debris" by 1 led
*
****************************************/
void gravity_falling_to_earth(uint8_t r, uint8_t g, uint8_t b, int delay) 
{
    int i;
    int row = 0;
    int col = 0;
    int color= 0;
    int randCols[20] = {0};
    int randRows[20] = {0};
    for(i = 0 ; i < 20; i++)
    {
        randCols[i] = rand() % 66;
        randRows[i] = rand() % 7;
    }
    
    for(i = 7; i >=0; i--)
    {
        for(row = 7; row > i; row--)
        {
            for(col = 0; col < 4; col++)
            {
                strip.setPixelColor(grid[row][randCols[col]], strip.Color(r/row,(rand()%192)/2/row,b));
                strip.setPixelColor(grid[row+6][randCols[col]],strip.Color(0,0,0));
            }           
        }
        
        strip.show();
        wait_ms(50);
    }
    
    for (row=0; row<((strip.numPixels())); row++)
        strip.setPixelColor(row, strip.Color(0,0,0));
    
    
    
    /*
    // fire code
    
    int red, orange, yellow, lime, i;
    
    for (i=rand()%8; i<((strip.numPixels())-8); i+=8)
    {
        strip.setPixelColor(i, strip.Color(255,0,0));
        strip.setPixelColor(i+1, strip.Color(239,16,0));
        strip.setPixelColor(i+2, strip.Color(223,32,0));
        strip.setPixelColor(i+3, strip.Color(207,48,0)); 
    }
    strip.show();
    wait_ms(delay);
    */
}


/****************************************
* main
*
* sets up the communication between the  main mbed
* inits the leds to check if the leds are working properly
* continuosly checks for a command from the main mbed
* if new command received, then executes the appropriate function
*
****************************************/
int main(){
    strip.begin();
    init_grid();

        
    OSCmsg m;
    OSCmsg *recv;

    OSCclass *c=new OSCclass;
    pc.baud(9600);
    pc.printf("Start----- Light COntroller!\r\n");
    
    //Thread t1(led_control);
    char *command=(char *)malloc(128);
    char mode;
    char color[2];
    //initialize table
    printf("Start----- Light COntroller!\r\n");
    int value;
    int i;
    for (i=0; i < strip.numPixels(); i++) 
    {
        strip.setPixelColor(i, strip.Color(255,255,255));
        wait_ms(1);  
        strip.show(); 
    }   
    
    
    wait_ms(2000);
    for (i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, 0);  // turn all pixels off   
    } 
    strip.show(); 
    
    int r = 0;
    int g = 255;
    int b = 0;
    
    /****OSC message temp***/
    char add[5];
    
    int flag = 0;
    int flag1 = 0;

    while(1) {
       // printf("receiving");
        rxLen = rf_receive(rxBuffer, 128);
        
        if(rxLen > 0) {
               printf("received");     
            recv= c->getOSCmsg(rxBuffer);
            printf("Address is %s with type %c and msg %c \r\n",recv->getAddr(),recv->getType(),recv->getArgs());
            strncpy(add,recv->getAddr(),5);
           
           //check if the message is for LED Control
           printf("add 2 is %c\r\n",add[1]);
           if(add[1] == 'A'){
                //check for the mode-strobe,on,flash,wave
                mode = add[3];
               
               switch(mode){
                    
                    case'O':  //switch on LED
                               color[0] = recv->getArgs();
                               pc.printf("color is: %s and \r\n",color);
                              pc.printf("Status %d\r\n",status);
                              
                               break;
                     case'S':  //switch on LED
                               color[0] = recv->getArgs();
                               break;
                     case'W':  //switch on LED
                               color[0] = recv->getArgs();
                               break;
                    case'R':  //switch on LED
                                color[0] = recv->getArgs();
                               break;
                      case'K':  //switch on LED
                               color[0] = recv->getArgs();
                               break;
                     case 'N':  //switch on LED
                                color[0] = recv->getArgs();
                               //pc.printf("color is: %s and K\r\n",color);
                              //values = color_table.find(atoi(color))->second;
                              status = OFF;
                               printf("Status %d\r\n",status);
                               break;
                    case 'P':  //switch on LED
                                value = recv->getArgs() - '0';
                               printf("Value is %d\r\n",value);
                              switch(value){
                                    case 0: clear();
                                            pc.printf("Clear\r\n");
                                            break;
                                            
                                    case 1: clear();
                                            nemo_stream(0,0,255,500);
                                            pc.printf("finding nemo %d   %d\r\n",flag,flag1);
                                            flag = 0;
                                            flag1 = 0;
                                            break;
                                            
                                    case 2: if(flag == 0)
                                            {
                                                clear();
                                                rocket_launch(63, 255, 255,120);
                                                pc.printf("rocket launch%d   %d\r\n",flag,flag1);
                                                flag = 1;
                                            }
                                            break;
                                            
                                    case 3: clear();
                                            beyonce_intro(128, 128, 128,25);
                                            pc.printf("beyonce intro%d   %d\r\n",flag,flag1);
                                            break;
                                            
                                    case 4: clear();
                                            super_bowl_wave(16,0,255, 1);
                                            pc.printf("super bow%d   %d\r\n",flag,flag1);
                                            break;
                                            
                                    case 5: clear();
                                            equalizer(255,0,0,40);
                                            pc.printf("Equalizer\r\n");
                                            break;
                                            
                                    case 6: clear();
                                            beyonce_middle(255,0,0,1);
                                            pc.printf("beyonce middle\r\n");
                                            break;
                                            
                                    case 7: 
                                            fill_color_blue(0,0,255,1);
                                            pc.printf("beyonce middle\r\n");
                                            flag1 = 0;
                                            break;
                                            
                                    case 8: if(flag1 == 0)
                                            {
                                            flag1 = 1;
                                            clear();
                                            spider_man(0,0,0,200);  
                                            pc.printf("Spider Man\r\n");
                                            }
                                            break;
                                            
                                    case 9: clear();
                                            gravity_falling_to_earth(255,0,0,500);
                                            pc.printf("gravity falling\r\n");
                                            break;
                                            
                                    default: break;
                                }
                               break;
                               
                    default:  break;
                }
            
            }
            
      }
    }             
}  
          