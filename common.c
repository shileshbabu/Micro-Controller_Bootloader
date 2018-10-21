/*
 * common.c
 *
 *  Created on: 21-Oct-2018
 *      Author: shilesh
 */
#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include "/inc/common.h"

void FRAMWrite(void);
unsigned int *FRAM_write_ptr;
unsigned long data;
unsigned int jumpAddress = 0;
extern char buffer[77];
unsigned char c = 0;
unsigned char buf = 0;

void firmware_upgrade_start()
{
    /* upgrading new firmware to check signature pointer value */
    if ((*(unsigned int *)0x1800 == 0xFFFF) || *(unsigned int *)0x1800 == 0x5A5A)
    {
        __disable_interrupt();
        /* set all pin configuration */
        setup_pin_port();
        /*  Erase flash memory for new FW. */
        for (temp_addr = (unsigned char *)0x6400; temp_addr < (unsigned char *)0xFF01; temp_addr++)
            *(unsigned int *)(temp_addr+0x0) = 0xFFFF;
        for (temp_addr = (unsigned char *)0xFF90; temp_addr < (unsigned char *)0xFFFE; temp_addr++)
            *(unsigned int *)(temp_addr+0x0) = 0xFFFF;
        __enable_interrupt();
        if (*(unsigned int *)0x1800 == 0x5A5A)
            send_command(CMD_REQUEST_FW);
        else
            send_command(REQUEST_FW_EVENT);
            // Only if buffer is full of data then only enter into the flashing mechanism
            if (buffer_full)
            {
                sum = 0;
                //This is the start of frame
                if (buffer[0] == ':')
                {
                    // Get the data byte count. Starting frm: 1
                    DataCount = parse_data(&buffer[1], 2);
                    // Get the start address of the frame. Starting frm: 3
                    temp_addr = parse_data(&buffer[3], 4);
                    // Get the Data type. Starting frm: 7
                    if (parse_data(&buffer[7], 2))
                    {
                        __disable_interrupt();
                        done_flag = 0;
                       jumpMainFW();
                    }
                    else
                    {
                        // If Address of the frame is not in vector range, then immediate flash write
                        // after Sumcheck verification. Starting frm: 9 to 73
                        if (temp_addr < (unsigned int*)0xFFFE)
                        {
                            if ((temp_addr == (unsigned char*)0xFF88) || (temp_addr == (unsigned char*)0xFF84) || (temp_addr == (unsigned char*)0xFF80))
                            {
                                for (tempCnt = 0; tempCnt < DataCount; tempCharArray[tempCnt] = 0,tempCnt++);
                                for (tempCnt = 0; tempCnt < 77; buffer[tempCnt] = 0,tempCnt++);
                                buffer_full = 0;
                                send_command(CMD_PACKET_ACK);
                            }
                            else
                            {
                                for (tempCnt = 0; tempCnt < DataCount; tempCnt++)
                                    tempCharArray[tempCnt] = (unsigned char)parse_data(&buffer[9+tempCnt*2], 2);
                                // Check first if the data is valid
                                if (calc_crc(&buffer[1],(DataCount + 4)))
                                {
                                        for (tempCnt = 0; tempCnt < DataCount; tempCnt++,temp_addr++)
                                            //flash_write(temp_addr, tempCharArray[tempCnt], 0);
                                            *(unsigned char *) temp_addr = tempCharArray[tempCnt];
                                        for (tempCnt = 0; tempCnt < DataCount; tempCharArray[tempCnt] = 0,tempCnt++);
                                        for (tempCnt = 0; tempCnt < 77; buffer[tempCnt] = 0,tempCnt++);
                                        buffer_full = 0;
                                        send_command(CMD_PACKET_ACK);
                                }
                                else
                                {
                                    Safe_backup();
                                    if (reTxCounts++ < MAX_COUNTS)
                                    send_command(CMD_RE_TX);
                                    send_command(REQUEST_FW_EVENT);
                                }
                            }
                            // If not matching then send ReTx frame command
                            buffer_full = 0;
                        }
                        else
                        {
                            for (tempCnt = 0; tempCnt < DataCount; tempCnt++)
                                tempCharArray[tempCnt] = (unsigned char)parse_data(&buffer[9+tempCnt*2], 2);
                            //jumpAddress = *(unsigned int*)tempCharArray;
                            *(unsigned int*)0x1802 = *(unsigned int*)tempCharArray;
                            buffer_full = 0;
                            send_command(CMD_PACKET_ACK);
                        }
                    }
                }
                else
                {
                    if (reTxCounts++ < MAX_COUNTS)
                    send_command(CMD_RE_TX);
                }
            }
        }
        *(unsigned int *)0x1800 = 0xA5A5;
        __delay_cycles(800000);
        send_command(CMD_UPGRADE_DONE);
        __delay_cycles(800000);
        WDTCTL = 0x5600 | WDTHOLD;
    }
    else if (signature == 0xA5A5)
    {
        jumpAddress  = *(unsigned int *)0x1802;
        asm("   MOV.W jumpAddress, pc");
    }
}

void jumpMainFW()
{
    WDTCTL = WDTPW | WDTHOLD;                       // Stop watchdog timer
}
unsigned char calc_crc(unsigned char * data, int count)
{
    unsigned char i = 0;
    unsigned char checksum = 0;
    for (i = 0; i < count; i++)
        sum += (unsigned char)parse_data(&data[i*2], 2);
    sum =~sum + 1;
    checksum = (unsigned char)parse_data(&data[i*2], 2);
    return ((checksum == sum) ? 1:0);
}
void vector_copy(unsigned char * From, unsigned char * To_addr, unsigned char * UpTo_addr)
{
    while (UpTo_addr != To_addr)
        *To_addr++ = *From++;
}
void flash_write(unsigned int flash_addr, unsigned int data, unsigned char type)
{
    unsigned char * temp_loc = (unsigned char *)(flash_addr + 0x0);
    __disable_interrupt();


    *temp_loc = data;
    if (type)
        *(temp_loc+1) = (data & 0xff00)>>8;


    __enable_interrupt();
}

// Data parse
unsigned int parse_data(unsigned char * bufPointer, int size)
{
    unsigned int return_val = 0;
    unsigned char max = size;
    while((--size) >=0)
    {
        if ((*(bufPointer + size) <= 0x39) && (*(bufPointer + size) >= 0x30))
            return_val |= (*(bufPointer + size) - 0x30) << ((max - (size+1))*4);
        else if ((*(bufPointer + size) <= 0x46) && (*(bufPointer + size) >= 0x41))
            return_val |= (*(bufPointer + size) - 0x37) << ((max - (size+1))*4);
    }
    return return_val;
}

void Safe_backup()
{
    if (BLResetVector != *(unsigned int *)RESETISRADDR)
    {
        for (temp_addr = (unsigned char *)0x6400; temp_addr < (unsigned char *)0xFF01; temp_addr++)
            *(unsigned int *)(temp_addr+0x0) = 0xFFFF;
        for (temp_addr = (unsigned char *)0xFF90; temp_addr < (unsigned char *)0xFFFE; temp_addr++)
            *(unsigned int *)(temp_addr+0x0) = 0xFFFF;
    }
}
