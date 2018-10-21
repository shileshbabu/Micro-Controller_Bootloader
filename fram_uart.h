/*
 * fram_uart.h
 *
 *  Created on: 21-Oct-2018
 *      Author: Shilesh
 */
#ifndef FRAM_UART_H_
#define FRAM_UART_H_


/*
 * Pre-process directives
 */
// Below are the data fields that will go along CMD_UDU_FW_UPG_CMD.
#define		CMD_PACKET_ACK		0x01
#define		CMD_RE_TX			0x02
#define 	CMD_OVER_SIZE		0x03
#define		CMD_UPGRADE_DONE	0x04
#define 	CMD_UPG_ERROR		0x05
#define		CMD_REQUEST_FW		0x1C
#define		CMD_WU_FW_UPG		0x1D
#define		REQUEST_FW			0x06
#define 	REQUEST_FW_EVENT	0x07
#define		REQUEST				'R'
#define 	PERIODIC			'S'

/*
 * Glb variable
 */
char buffer[77] = {0};
extern unsigned char sum;
extern unsigned char command_buf[16];
/*
 * Function declarations
 */
void send_command(unsigned char);
void send_frame(unsigned char *);
void UARTSetup();
unsigned short SCI_uwCCC16(unsigned char *msg, unsigned short size);





#endif /* FRAM_UART_H_ */
