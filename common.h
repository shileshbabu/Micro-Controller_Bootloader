
/*
 * main.h
 *
 *  Created on: 21-Oct-2018
 *      Author: Shilesh
 */
#ifndef COMMON_H_
#define COMMON_H_

#if 0
#pragma DATA_SECTION (signature , ".infoS")
#pragma RETAIN (signature)
unsigned int signature;

#pragma DATA_SECTION (JumpAddr , ".infoJ")
unsigned int JumpAddr; //= 0xFFFF;
#endif

#define SIGNATURE_VAL	0xA5A5
#define SIGNATURE 		0
#define BL_UARTISR 		1
#define BL_TIMERISR 	2
#define BL_RESETVECTOR 	3
#define RESETVECTOR 	4
#define SERIAL_NUM		5
#define END_SEGMENT		0xFE00
#define VECTORS_STORAGE	0x1080
#define VECTORS_START	0xFF90
#define VECTORS_END		0xFFFE
#define SIGN_SEGMENT	0x1040
#define INFO_SEGMENT	0x1000

extern void send_frame(unsigned char *);
extern __interrupt void USCI_A0_ISR();
extern __interrupt void Timer_A (void);
extern void setup();
extern void send_command(unsigned char);
unsigned int parse_data(unsigned char * bufPointer, int size);
void fw_upgrade();
#define RESETISRADDR 0xFFFE

// Command buffer for transmitting command to SOC
unsigned char command_buf[16] = {0};
unsigned char done_flag = 1;
extern unsigned char buffer_full;
unsigned char * temp_addr;
unsigned char DataCount =0;
unsigned char tempCnt = 0;
unsigned char tempCharArray[32];
unsigned char sum = 0;
unsigned char reTxCounts = 0;
#define MAX_COUNTS 5





#endif  /* COMMON_H_ */
