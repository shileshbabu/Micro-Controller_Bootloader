/*
 * fram_uart.c
 *
 *  Created on: 21-Oct-2018
 *      Author: Shilesh
 */


#include "fram_uart.h"
#include <msp430.h>
#include <stdlib.h>

#define POLY 0x8408

// Types is command communications with SoC
void send_command(unsigned char cmdtype)
{
    unsigned short crc_buff;
    unsigned char i;

    switch (cmdtype)
    {
    case CMD_PACKET_ACK:
        command_buf[0] = REQUEST;
        command_buf[1] = CMD_WU_FW_UPG;
        command_buf[2] = CMD_PACKET_ACK;
        break;
    case CMD_REQUEST_FW:
        command_buf[0] = REQUEST;
        command_buf[1] = CMD_REQUEST_FW;
        command_buf[2] = 1;
        crc_buff = SCI_uwCCC16((unsigned char)command_buf,(sizeof(command_buf)-2));
        command_buf[14] = (unsigned char)(crc_buff >> 8);
        command_buf[15] = (unsigned char)(crc_buff);
        break;
    case REQUEST_FW_EVENT:
        command_buf[0] = PERIODIC;	// SOF
        command_buf[1] = CMD_REQUEST_FW;
        command_buf[2] = 1;
        crc_buff = SCI_uwCCC16((unsigned char)command_buf,(sizeof(command_buf)-2));
        command_buf[14] = (unsigned char)(crc_buff >> 8);
        command_buf[15] = (unsigned char)(crc_buff);
        break;
    case CMD_UPGRADE_DONE:
        command_buf[0] = REQUEST;
        command_buf[1] = CMD_WU_FW_UPG;
        command_buf[2] = CMD_UPGRADE_DONE;
        break;
    case CMD_RE_TX:
        command_buf[0] = REQUEST;
        // Packet recieved, Checksum wrong, resend request
        command_buf[1] = CMD_WU_FW_UPG;
        command_buf[2] = CMD_RE_TX;
        command_buf[3] = sum;
        break;
    case CMD_OVER_SIZE:
        command_buf[0] = 0x55;	// SOF
        // Packet recieved, Checksum wrong, resend request
        command_buf[1] = CMD_WU_FW_UPG;
        command_buf[2] = CMD_OVER_SIZE;
        break;
    default:
        break;
    }
    send_frame(command_buf);
    for (i = 0; i < 16; i++)
        command_buf[i] = 0;
}

// Fill and send UART transmit data buf register
void send_frame(unsigned char * tx_data)
{
    unsigned char i;
    //unsigned char *cast = (unsigned char *) tx_data;
    for (i = 0; i < 16; i++){
        while (!(UCA0IFG & UCTXIFG));
        UCA0TXBUF = (unsigned char)tx_data[i];
        }
}

unsigned short SCI_uwCCC16(unsigned char *msg, unsigned short size)
{
    unsigned char i;
    unsigned int data;
    unsigned int crc = 0xffff;

    if (size == 0)
        return (~crc);

    do{
        for (i=0, data=(unsigned int)0xff & *msg++;
                i < 8;
                i++, data >>= 1)
        {
            if ((crc & 0x0001) ^ (data & 0x0001))
                crc = (crc >> 1) ^ POLY;
            else  crc >>= 1;
        }
    } while (--size);

    crc = ~crc;
    data = crc;
    crc = (crc << 8) | (data >> 8 & 0xff);

    return (crc);

}

void clock_setup()
{
    /* Clock System Setup */
    CSCTL0_H = CSKEY >> 8;               /* Unlock CS registers */
    CSCTL1 = DCOFSEL_6;                  /*  Set DCO to 8MHz */
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;  /* Set SMCLK = MCLK = DCO */
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     /* Set all dividers to 1 */
    CSCTL0_H = 0;                             /* Lock CS registers */
}
void setup()
{
    clock_setup();
    // Uart setup - 115200 baudrate
    UARTSetup();
}

// This will setup UART for maximux Baudrate 115200.
void UARTSetup()
{
    /* UART USCI0_A0 P4.2 -> UCARx P4.3 -> UCATx */
    P2SEL0 |= BIT0 | BIT1;
    P2SEL1 &=~ BIT0 | BIT1;
    /*UART configuration for USCI0 channle */
    UCA0CTLW0 = UCSWRST;           /* Put eUSCI in reset */
    UCA0CTLW0 |= UCSSEL__SMCLK;    /* CLK = SMCLK */
    /*Calcculation for baudrate */
    /*for 460800 baudrate*/
    UCA0BR0 =17;
    UCA0MCTLW |= 0x4A00;
    UCA0BR1 = 0x00;
    UCA0CTLW0 &= ~UCSWRST;
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A0_VECTOR)))
#endif
void USCI_A0_ISR(void)
{
    switch(__even_in_range(UCA0IV,USCI_UART_UCRXIFG))
    {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
        buf = UCA0RXBUF;
        if(buf != '\n')
        {
            if (buf != '\r')
            {
                // Just take the data byte received.
                buffer[c++] = buf;
                UCA0IFG &= ~UCRXIFG;
            }
        }
        else
        {
            buffer_full = 1;
            c = 0;
            P1OUT ^= 0x01;
            UCA0IFG &= ~UCRXIFG;

        }
        break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
    }
}
