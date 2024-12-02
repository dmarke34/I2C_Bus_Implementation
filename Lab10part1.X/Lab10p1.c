#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#include <usart.h>
#include <string.h>

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF
#pragma config CCP2MX = PORTBE

#include "I2C.h"
#include "I2C_Support.h"
#include "Interrupt.h"
#include "ST7735.h"


#define Circle_Size     20              // Size of Circle for Light
#define Circle_X        60              // Location of Circle
#define Circle_Y        80              // Location of Circle
#define Text_X          52
#define Text_Y          77
#define TS_1            1               // Size of Normal Text
#define TS_2            2               // Size of Big Text


// colors
#define RD               ST7735_RED
#define BU               ST7735_BLUE
#define GR               ST7735_GREEN
#define MA               ST7735_MAGENTA
#define BK               ST7735_BLACK

#define kp               PORTEbits.RE1             // definition Key Pressed LED      

#define Circle_Size     20              // Size of Circle for Light
#define Circle_X        60              // Location of Circle
#define Circle_Y        80              // Location of Circle
#define Text_X          52
#define Text_Y          77
#define TS_1            1               // Size of Normal Text
#define TS_2            2               // Size of Big Text

#define RD               ST7735_RED
#define BU               ST7735_BLUE
#define GR               ST7735_GREEN
#define MA               ST7735_MAGENTA
#define BK               ST7735_BLACK

char tempSecond = 0xff; 
char second = 0x00;
char minute = 0x00;
char hour = 0x00;
char dow = 0x00;
char day = 0x00;
char month = 0x00;
char year = 0x00;
char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;
char alarm_second, alarm_minute, alarm_hour, alarm_date;
char setup_alarm_second, setup_alarm_minute, setup_alarm_hour;

char array1[21]= { 0xa2, 0x62, 0xe2, 0x22, 0x02, 0xc2, 0xe0, 0xa8, 0x90, 0x68, 0x98, 0xb0, 0x30, 0x18, 0x7a, 0x10, 0x38, 0x5a, 0x42, 0x4a, 0x52 };
char txt1[21][4] = {"CH-\0", "CH0\0", "CH+\0", "PRV\0", "NXT\0", "P/P\0","VL-\0", "VL+\0", "EQ0\0", "#00\0","100\0", "200\0", "#01\0", "#02\0", "#03\0", "#04\0", "#05\0", "#06\0", "#07\0", "#08\0", "#09\0" };
int color[21]= { RD, RD, RD, BU, BU, GR, MA, MA, MA, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK };

char TempSecond;

char buffer[31];                        // general buffer for display purpose
char *nbr;                              // general pointer used for buffer
char *txt;

short nec_ok = 0;
unsigned long long Nec_code;
char Nec_code1;
char found;
unsigned char Nec_state = 0;

void Activate_Buzzer();
void Deactivate_Buzzer();
void Initialize_Screen();
void Wait_One_Sec();
void Do_Init();
void DO_DISPLAY_D1(int);

void putch (char c)
{   
    while (!TRMT);       
    TXREG = c;
}

void init_UART()
{
    OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 25);
    OSCCON = 0x70;
}


void Wait_One_Sec()
{
    for (int I = 0; I < 25000; I++);
}


void Do_Init()                      // Initialize the ports 
{ 
    init_UART();                    // Initialize the uart
    OSCCON=0x70;                    // Set oscillator to 8 MHz    
    ADCON1=0x0F;
    TRISB = 0x01;                   // Set bit 0 of PORTB as input otherwise output
    TRISC = 0x00;                   // PORTC as output
    TRISD = 0x0C;                   // Set bit 2 and 3 of PORTD as input otherwise output
    TRISE = 0x00;                   // PORTE as output
    RBPU=0;
    init_INTERRUPT();    
    I2C_Init(100000); 
    DS1621_Init();
    Turn_
}

void Activate_Buzzer()
{
    PR2 = 0b11111001;
    T2CON = 0b00000101;
    CCPR2L = 0b01001010;
    CCP2CON = 0b00111100;
}

void Deactivate_Buzzer()
{
    CCP2CON = 0x0;
    PORTBbits.RB3 = 0;
}

void Initialize_Screen()
{
    LCD_Reset();
    TFT_GreenTab_Initialize();
    fillScreen(ST7735_BLACK);
  
    /* TOP HEADER FIELD */
    txt = buffer;
    strcpy(txt, "ECE3301L Fall 21-S2");  
    drawtext(2, 2, txt, ST7735_WHITE, ST7735_BLACK, TS_1);

    strcpy(txt, "LAB 10 ");  
    drawtext(50, 10, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
}

void DO_DISPLAY_D1(int T)                   // Function to set D1 according to Temp reading
{
    if (T < 10){                            // Set D1 off if temp below 10
        PORTD = 0x00;
    }
    else if(T >= 10 && T <= 19){            // Set D1 red if temp between 10 and 19
        PORTD = 0x10;
    }
    else if(T >= 20 && T <= 29){            // Set D1 green if temp between 20 and 29
        PORTD = 0x20;
    }
    else if(T >= 30 && T <= 39){            // Set D1 yellow if temp between 20 and 39
        PORTD = 0x30;
    }
    else if(T >= 40 && T <= 49){            // Set D1 blue if temp between 40 and 49
        PORTD = 0x40;
    }
    else if(T >= 50 && T <= 59){            // Set D1 purple if temp between 50 and 59
        PORTD = 0x50;
    }
    else if(T >= 60 && T <= 69){            // Set D1 cyan if temp between 60 and 69
        PORTD = 0x60;
    }
    else if(T >= 70){                       // Set D1 white if temp above 70
        PORTD = 0x70;
    }
}

void main() 
{ 
    
    Do_Init();
    Initialize_Screen();
    // Initialization  
      DS3231_Setup_Time();
      while (1)
      {

        DS3231_Read_Time();                                     // Read Time from DS3231
        if(tempSecond != second)
        {
            tempSecond = second;
            signed char tempC = DS1621_Read_Temp();            // Set tempC to Temperature reading from DS1621
            signed char tempF = (tempC * 9 / 5) + 32;          // Convert to Fahrenheit
            printf ("%02x:%02x:%02x %02x/%02x/%02x",hour,minute,second,month,day,year);
            printf (" Temperature = %d degreesC = %d degreesF\r\n", tempC, tempF);
            DO_DISPLAY_D1(tempF);                              // Display color according to Temp reading
        }
        if (nec_ok == 1)
        {
            nec_ok = 0;
            INT0_isr();
            Nec_code1 = (char)((Nec_code >> 8));
            INTCONbits.INT0IE = 1;          // Enable external interrupt
            INTCON2bits.INTEDG0 = 0;        // Edge programming for INT0 falling edge
            char found = 0xff;

            for (int i = 0; i < 21; i++)
            {
                if (array1[i] == Nec_code1)                   // Check if the button matches code in array
                {
                    found = i;                                // Set found as the position of matching code           
                    printf("NEC_Code = %08lx %x, %d \r\n", Nec_code, Nec_code1, found);
                    break;
                }
            }
            
            if (found == 8)                                  // Check if button pressed is EQ
            {
                DS3231_Setup_Time();                         // If yes, preset time
                printf ("Time Change \r\n");            
            }

            if (found != 0xff)
            {
                kp = 1;                                     // Turn D2 LED on
                fillCircle(Circle_X, Circle_Y, Circle_Size, color[found]);
                drawCircle(Circle_X, Circle_Y, Circle_Size, ST7735_WHITE);
                drawtext(Text_X, Text_Y, txt1[found], ST7735_WHITE, ST7735_BLACK, TS_1);
                Activate_Buzzer();                          // Turn Buzzer on
                Wait_One_Sec();                             // Wait one second
                Deactivate_Buzzer();                        // Turn Buzzer off
                kp = 0;                                     // Turn D2 LED off
            }
        }
    }
   
}
