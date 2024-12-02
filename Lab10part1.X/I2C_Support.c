#include <stdio.h>

#include <p18f4620.h>
#include "I2C_Support.h"
#include "I2C.h"


#define ACCESS_CFG      0xAC
#define START_CONV      0xEE
#define READ_TEMP       0xAA
#define CONT_CONV       0x02
#define ACK     1
#define NAK     0


extern unsigned char second, minute, hour, dow, day, month, year;
extern unsigned char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;
extern unsigned char alarm_second, alarm_minute, alarm_hour, alarm_date;
extern unsigned char setup_alarm_second, setup_alarm_minute, setup_alarm_hour;


void DS1621_Init()
{
    char Device = 0x48;               // I2C address for DS1621
    I2C_Write_Cmd_Write_Data(Device, ACCESS_CFG, CONT_CONV);
    I2C_Write_Cmd_Only(Device, START_CONV);
}


int DS1621_Read_Temp()
{
    char Device = 0x48;               // I2C address for DS1621
    char Cmd = READ_TEMP;             // Read Temperature command
    char Data_Ret;
    I2C_Start();                      // Start I2C protocol
    I2C_Write((Device << 1) | 0);     // Device address
    I2C_Write(Cmd);                   // Send register address
    I2C_ReStart();                    // Restart I2C
    I2C_Write((Device << 1) | 1);     // Initialize data read
    Data_Ret = I2C_Read(NAK);         //
    I2C_Stop();
    return Data_Ret;
}

void DS3231_Read_Time()
{
    char Device = 0x68;               // I2C address for DS3231
    char Address = 0x00;              // 'Seconds' register
    char Data_Ret;
    I2C_Start();                      // Start I2C protocol
    I2C_Write((Device << 1) | 0);     // DS3231 address Write mode
    I2C_Write(Address);               // Send register address
    I2C_ReStart();                    // Restart I2C
    I2C_Write((Device << 1) | 1);     // Initialize data read
    second = I2C_Read(ACK);           // Read register 'second'
    minute = I2C_Read(ACK);           // Read register 'minute'
    hour = I2C_Read(ACK);             // Read register 'hour'
    dow = I2C_Read(ACK);              // Read register 'dow'
    day = I2C_Read(ACK);              // Read register 'day'
    month = I2C_Read(ACK);            // Read register 'month'
    year = I2C_Read(NAK);             // Read register 'year' then terminate read sequence
    I2C_Stop();
}

void DS3231_Setup_Time()
{
    char Device = 0x68;               // I2C address for DS3231
    char Address = 0x00;              // 'Seconds' register
    second = 0x20;                    // Set to 20 seconds
    minute = 0x10;                    // Set to 10 minutes
    hour = 0x02;                      // Set to 2 hours
    dow = 0x05;                       // Set to 5th dow
    day = 0x28;                       // Set to 28th day
    month = 0x10;                     // Set to 10th month
    year = 0x21;                      // Set to year 2021
    
    I2C_Start();                      // Start I2C protocol
                                        
    I2C_Write((Device << 1) | 0);     // Device address Write mode
    I2C_Write(Address);               // Send register address
    I2C_Write(second);                // Initialize data read
    I2C_Write(minute);
    I2C_Write(hour);
    I2C_Write(dow);
    I2C_Write(day);
    I2C_Write(month);
    I2C_Write(year);
    
    I2C_Stop();
}
