/*
 * main.c
 *
 *  Created on: 21-Oct-2018
 *      Author: Shilesh
 */
#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include "common.h"

void main(void)
{

 	WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT
	// previously configured port settings
	PM5CTL0 &= ~LOCKLPM5;
	MPUCTL0 = MPUPW;                          // Write PWD to access MPU registers
	MPUSAM |= MPUSEGIWE;                      // Writes to user information memory are allowed
	//MPUCTL0 = MPUPW | MPUENA;
	firmware_upgrade_start();
}
