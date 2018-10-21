/* ============================================================================ */
/* Copyright (c) 2015, Texas Instruments Incorporated                           */
/*  All rights reserved.                                                        */
/*                                                                              */
/*  Redistribution and use in source and binary forms, with or without          */
/*  modification, are permitted provided that the following conditions          */
/*  are met:                                                                    */
/*                                                                              */
/*  *  Redistributions of source code must retain the above copyright           */
/*     notice, this list of conditions and the following disclaimer.            */
/*                                                                              */
/*  *  Redistributions in binary form must reproduce the above copyright        */
/*     notice, this list of conditions and the following disclaimer in the      */
/*     documentation and/or other materials provided with the distribution.     */
/*                                                                              */
/*  *  Neither the name of Texas Instruments Incorporated nor the names of      */
/*     its contributors may be used to endorse or promote products derived      */
/*     from this software without specific prior written permission.            */
/*                                                                              */
/*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" */
/*  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,       */
/*  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR      */
/*  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR            */
/*  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,       */
/*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,         */
/*  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; */
/*  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,    */
/*  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR     */
/*  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,              */
/*  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                          */
/* ============================================================================ */

/******************************************************************************/
/* lnk_msp430fr6989.cmd - LINKER COMMAND FILE FOR LINKING MSP430FR6989 PROGRAMS     */
/*                                                                            */
/*   Usage:  lnk430 <obj files...>    -o <out file> -m <map file> lnk.cmd     */
/*           cl430  <src files...> -z -o <out file> -m <map file> lnk.cmd     */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* These linker options are for command line linking only.  For IDE linking,  */
/* you should set your linker options in Project Properties                   */
/* -c                                               LINK USING C CONVENTIONS  */
/* -stack  0x0100                                   SOFTWARE STACK SIZE       */
/* -heap   0x0100                                   HEAP AREA SIZE            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* Version: 1.173                                                             */
/*----------------------------------------------------------------------------*/

/****************************************************************************/
/* Specify the system memory map                                            */
/****************************************************************************/


    MEMORY
{
/*
    TINYRAM                 : origin = 0x0006, length = 0x001A
    PERIPHERALS_8BIT        : origin = 0x0020, length = 0x00E0
    PERIPHERALS_16BIT       : origin = 0x0100, length = 0x0100
    RAM                     : origin = 0x1C00, length = 0x0800
    INFOA                   : origin = 0x1980, length = 0x0080
    INFOB                   : origin = 0x1900, length = 0x0080
    INFOC                   : origin = 0x1880, length = 0x0080
    //INFOD                   : origin = 0x1800, length = 0x0080
    INFOD                   : origin = 0x1802, length = 0x007C
    //INFOJ                   : origin = 0x1802, length = 0x0002
    //INFOS                   : origin = 0x1800, length = 0x0002, fill = 0xFFFF
   	//FRAM                    : origin = 0x4400, length = 0xBB80
    FRAM                    : origin = 0x4400, length = 0x2000
    FRAM2                   : origin = 0x10000,length = 0x14000
    JTAGSIGNATURE           : origin = 0xFF80, length = 0x0004, fill = 0xFFFF
    BSLSIGNATURE            : origin = 0xFF84, length = 0x0004, fill = 0xFFFF
    IPESIGNATURE            : origin = 0xFF88, length = 0x0008, fill = 0xFFFF
    RESET                   : origin = 0xFFFE, length = 0x0002
    */
     TINYRAM                 : origin = 0x0006, length = 0x001A
    PERIPHERALS_8BIT        : origin = 0x0020, length = 0x00E0
    PERIPHERALS_16BIT       : origin = 0x0100, length = 0x0100
    RAM                     : origin = 0x1C00, length = 0x0800
    INFOA                   : origin = 0x1980, length = 0x0080
    INFOB                   : origin = 0x1900, length = 0x0080
  //  INFOC                   : origin = 0x1880, length = 0x0080
    INFOC                   : origin = 0x188A, length = 0x0076
//    INFOSN                  : origin = 0x1880, length = 0x000A
   // INFOD                   : origin = 0x1800, length = 0x0080
   INFOD                   : origin = 0x1804, length = 0x007A
    INFOJ                   : origin = 0x1802, length = 0x0002
    INFOS                   : origin = 0x1800, length = 0x0002
    FRAM                    : origin = 0x4400, length = 0x2000
 //	  FRAM2                   : origin = 0x10000,length = 0x14000
    JTAGSIGNATURE           : origin = 0xFF80, length = 0x0004, fill = 0xFFFF
    BSLSIGNATURE            : origin = 0xFF84, length = 0x0004, fill = 0xFFFF
    IPESIGNATURE            : origin = 0xFF88, length = 0x0008, fill = 0xFFFF
    RESET                   : origin = 0xFFFE, length = 0x0002
}

/****************************************************************************/
/* Specify the sections allocation into memory                              */
/****************************************************************************/

SECTIONS
{
    GROUP(READ_WRITE_MEMORY)
    {
       .TI.persistent : {}                  /* For #pragma persistent            */
       .cio           : {}                  /* C I/O Buffer                      */
       .sysmem        : {}                  /* Dynamic memory allocation area    */
    } PALIGN(0x0400), RUN_END(fram_rx_start) > 0x4400

    .cinit            : {}  > FRAM          /* Initialization tables             */
    .pinit            : {}  > FRAM          /* C++ Constructor tables            */
    .init_array       : {}  > FRAM          /* C++ Constructor tables            */
    .mspabi.exidx     : {}  > FRAM          /* C++ Constructor tables            */
    .mspabi.extab     : {}  > FRAM          /* C++ Constructor tables            */
#ifndef __LARGE_DATA_MODEL__
    .const            : {} >> FRAM          /* Constant data                     */
#else
    .const            : {} >> FRAM //| FRAM2  /* Constant data                     */
#endif

    .text:_isr        : {}  > FRAM          /* Code ISRs                         */
#ifndef __LARGE_DATA_MODEL__
    .text             : {} >> FRAM          /* Code                              */
#else
    .text             : {} >> FRAM //| FRAM2  /* Code                              */
#endif

    GROUP(IPENCAPSULATED_MEMORY)
    {
       .ipestruct     : {}                  /* IPE Data structure             */
       .ipe           : {}                  /* IPE                            */
       .ipe:_isr      : {}                  /* IPE ISRs                       */
       .ipe_vars      : type = NOINIT{}     /* IPE variables                  */
    } PALIGN(0x0400), RUN_START(fram_ipe_start) RUN_END(fram_ipe_end) > FRAM

    .jtagsignature : {} > JTAGSIGNATURE     /* JTAG Signature                    */
    .bslsignature  : {} > BSLSIGNATURE      /* BSL Signature                     */

    GROUP(SIGNATURE_SHAREDMEMORY)
    {
       .ipesignature   : {}                 /* IPE Signature                     */
       .jtagpassword   : {}                 /* JTAG Password                     */
    } > IPESIGNATURE

    .bss        : {} > RAM                  /* Global & static vars              */
    .data       : {} > RAM                  /* Global & static vars              */
    .TI.noinit  : {} > RAM                  /* For #pragma noinit                */
    .stack      : {} > RAM (HIGH)           /* Software system stack             */
    .tinyram    : {} > TINYRAM              /* Tiny RAM                          */

	//.infoJ		: {} > INFOJ				/* Jump Address of main FW */
	//.infoS		: {} > INFOS				/* FW Signature */
    .infoA     : {} > INFOA              /* MSP430 INFO FRAM  Memory segments */
    .infoB     : {} > INFOB
    .infoC     : {} > INFOC
    .infoD     : {} > INFOD
//	.infoSN	   : {} > INFOSN				/* FW Signature */
    /* MSP430 Interrupt vectors          */
    .reset       : {}               > RESET  /* MSP430 Reset vector         */
}


/****************************************************************************/
/* MPU/IPE Specific memory segment definitons                               */
/****************************************************************************/

#ifdef _IPE_ENABLE
   #define IPE_MPUIPLOCK 0x0080
   #define IPE_MPUIPENA 0x0040
   #define IPE_MPUIPPUC 0x0020

   // Evaluate settings for the control setting of IP Encapsulation
   #if defined(_IPE_ASSERTPUC1)
        #if defined(_IPE_LOCK ) && (_IPE_ASSERTPUC1 == 0x08))
         fram_ipe_enable_value = (IPE_MPUIPENA | IPE_MPUIPPUC |IPE_MPUIPLOCK);
        #elif defined(_IPE_LOCK )
         fram_ipe_enable_value = (IPE_MPUIPENA | IPE_MPUIPLOCK);
      #elif (_IPE_ASSERTPUC1 == 0x08)
         fram_ipe_enable_value = (IPE_MPUIPENA | IPE_MPUIPPUC);
      #else
         fram_ipe_enable_value = (IPE_MPUIPENA);
      #endif
   #else
      #if defined(_IPE_LOCK )
         fram_ipe_enable_value = (IPE_MPUIPENA | IPE_MPUIPLOCK);
      #else
         fram_ipe_enable_value = (IPE_MPUIPENA);
      #endif
   #endif

   // Segment definitions
   #ifdef _IPE_MANUAL                  // For custom sizes selected in the GUI
      fram_ipe_border1 = (_IPE_SEGB1>>4);
      fram_ipe_border2 = (_IPE_SEGB2>>4);
   #else                           // Automated sizes generated by the Linker
      fram_ipe_border2 = (fram_ipe_end + 0x400)>> 4;
      fram_ipe_border1 = fram_ipe_start >> 4;
   #endif

   fram_ipe_settings_struct_address = Ipe_settingsStruct >> 4;
   fram_ipe_checksum = ~((fram_ipe_enable_value & fram_ipe_border2 & fram_ipe_border1) | (fram_ipe_enable_value & ~fram_ipe_border2 & ~fram_ipe_border1) | (~fram_ipe_enable_value & fram_ipe_border2 & ~fram_ipe_border1) | (~fram_ipe_enable_value & ~fram_ipe_border2 & fram_ipe_border1));
#endif

#ifdef _MPU_ENABLE
   #define MPUPW (0xA500)    /* MPU Access Password */
   #define MPUENA (0x0001)   /* MPU Enable */
   #define MPULOCK (0x0002)  /* MPU Lock */
   #define MPUSEGIE (0x0010) /* MPU Enable NMI on Segment violation */

   __mpu_enable = 1;
   // Segment definitions
   #ifdef _MPU_MANUAL // For custom sizes selected in the GUI
      mpu_segment_border1 = _MPU_SEGB1 >> 4;
      mpu_segment_border2 = _MPU_SEGB2 >> 4;
      mpu_sam_value = (_MPU_SAM0 << 12) | (_MPU_SAM3 << 8) | (_MPU_SAM2 << 4) | _MPU_SAM1;
   #else // Automated sizes generated by Linker
      mpu_segment_border1 = fram_rx_start >> 4;
      mpu_segment_border2 = fram_rx_start >> 4;
      mpu_sam_value = 0x1513; // Info R, Seg3 RX, Seg2 R, Seg1 RW
   #endif
   #ifdef _MPU_LOCK
      #ifdef _MPU_ENABLE_NMI
         mpu_ctl0_value = MPUPW | MPUENA | MPULOCK | MPUSEGIE;
      #else
         mpu_ctl0_value = MPUPW | MPUENA | MPULOCK;
      #endif
   #else
      #ifdef _MPU_ENABLE_NMI
         mpu_ctl0_value = MPUPW | MPUENA | MPUSEGIE;
      #else
         mpu_ctl0_value = MPUPW | MPUENA;
      #endif
   #endif
#endif

/****************************************************************************/
/* Include peripherals memory map                                           */
/****************************************************************************/

-l msp430fr6989.cmd

