/****************************************************************************/
/* Microcontroller (PIC) software library for CC1020 application            */
/*                                                                          */
/* File:      cc1020pic.c                                                   */
/*                                                                          */
/* Microcontroller:                                                         */
/*          Microchip PIC16F876                                             */
/*                                                                          */
/* Written for the IAR PIC16 compiler                                       */
/*                                                                          */
/* Author:  Arne Rogndalen, Design Engineer, Chipcon                        */
/*          Torgeir Sundet, FAE Software, Chipcon                           */
/*                                                                          */
/* Contact: Chipcon AS +47 22 95 85 44                                      */
/*          support@chipcon.com                                             */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/* This library contains functions for configuring the CC1020. These        */
/* routines use bit-banging to program the CC1020, faster configuration is  */
/* possible by using a synchronous serial port such as a SPI interface.     */
/* The header file "modemhw.h" contains definitions for the various I/O     */
/* pins, the user should make a similar file to name the pins used to       */
/* communicate with the CC1020. Routines to read and write the calibration  */
/* values in the CC1020 are provided, they are not used in this reference   */
/* application, but are useful in other applications, most notably          */
/* frequency-agile and frequency hopping applications. See application      */
/* note AN009 for more information.                                         */
/* The routines in this file will have to be adapted depending on the MCU   */
/* and compiler used. The method used for shifting data in and out may have */
/* to be changed if the bit ordering for bitfields is different from the    */
/* IAR PIC compiler.                                                        */
/*                                                                          */
/* Configuration routines are included in two versions: one using general   */
/* I/O ports ("bit-banging"), and one using the built-in SPI interface of   */
/* the PIC16F876. If possible, the SPI version should be used, as this is   */
/* much faster. The SPI versions are used if the symbol "SPI" is defined,   */
/* otherwise the general I/O-based version is used.                         */
/****************************************************************************/

/*                                                                           *
 * Revision history:                                                         *
 *                                                                           *
 * $Log: cc1020pic.c,v $
 * Revision 1.9  2005/11/22 18:23:18  tos
 * Mods:
 * - Implemented support for Errata Note 04.
 * - Implemented support for RSSI measurement.
 *
 * Revision 1.8  2003/12/16 15:01:23  tos
 * Removed 5 msec delay/wait before LOCK monitor.
 *
 * Revision 1.7  2003/09/19 12:14:57  tos
 * Mod's in TX-setup:
 * - always tun off DCLK squelch in TX.
 * - turn on TX before PA is increased.
 *
 * Revision 1.6  2003/08/14 11:06:36  tos
 * Modified some function prototypes to enable PA_POWER as input argument.
 *
 * Revision 1.5  2003/07/23 14:36:19  tos
 * Correction of reset + power-down in accordance with design recommendation.
 *
 * Revision 1.4  2003/06/27 17:22:17  tos
 * Modified setup routines to allow separat RX/TX settings in ANALOG register.
 *
 * Revision 1.3  2003/06/02 14:43:33  tos
 * Recalibrate if LOCK failure after RX/TX setup.
 *
 * Revision 1.2  2003/05/23 13:28:54  tos
 * Tuning+testing before release.
 *
 * Revision 1.1  2003/05/21 09:48:17  tos
 * Initial version in CVS.
 *
 *
 *                                                                           *
 ****************************************************************************/


#include <LPC214x.h>
#include "CC1020.h"
#include "modemhw.h"


/****************************************************************************/
/*  This routine sends new configuration data to the CC1020                 */
/***************************************************************************

void ConfigureCC1020(char Count, short Configuration[])
{
  short val;
  char i;
  
  for (i=0;i<Count;i++) {
    val=Configuration[i];
    WriteToCC1020RegisterWord(val);
  }
   
}

/****************************************************************************/
/* SPI versions of configuration routines. The SPI interface must be        */
/* initialised correctly before use                                         */
/***************************************************************************

#ifdef SPI


/****************************************************************************/
/*  This routine sets up the CC1020 for SPI transfer                        */
/***************************************************************************

void SetupCC1020ForSPI(void)
{
  SSPSTAT=0x40; 
  SSPCON=0x20;
}

/****************************************************************************/
/*  This routine writes to a single CC1020 register                         */
/***************************************************************************

void WriteToCC1020Register(char addr, char data)
{
  char dummy;

  PSEL=0;
   
  dummy=SSPBUF;
  SSPBUF=(addr<<1)|0x01; // Write address to CC1020, write bit is always 1
 
  // Wait until data is written 
  while (BF==0);    

  dummy=SSPBUF;  
  SSPBUF=data;
  while (BF==0);

  PSEL=1;
}

/****************************************************************************/
/*  This routine writes to a single CC1020 register, with data and address  */
/*  given in the same variable                                              */
/***************************************************************************

void WriteToCC1020RegisterWord(short addranddata)
{
  char dummy;
  
  union {
    unsigned short data;
    struct {
      char LowByte;
      char HighByte;
    };
  };

  data=addranddata;
  PSEL=0;
   
  dummy=SSPBUF;
  SSPBUF=LowByte|0x01; // Write address to CC1020, write bit is always 1
 
  // Wait until data is written 
  while (BF==0);    
  
  dummy=SSPBUF;  
  SSPBUF=HighByte;
  while (BF==0);

  PSEL=1;
}

/****************************************************************************/
/*  This routine reads from a single CC1020 register                        */
/***************************************************************************

char ReadFromCC1020Register(char addr)
{
  char Value;

  PSEL=0;
  Value=SSPBUF;
  SSPBUF=(addr<<1)&0xFE; // Write address to CC1020, write bit is always 0
  
  // Wait until data is written 
  while (BF==0);
  SSPOV=0;
  
  // Switch direction 
  PDI=1;
  TRISC|=0x20; // Set up PDATAOUT as an input
  
  SSPBUF=0xFF; // Dummy write 
  
  while (BF==0);
  Value=SSPBUF;
  
  TRISC&=~0x20; // Set PDATAOUT as an output 

  PSEL=1;
  return Value;
}

#else

/****************************************************************************/
/* General I/O pin "bit-bashing" versions of configuration routines.        */
/****************************************************************************/

/****************************************************************************/
/*  This routine writes to a single CC1020 register                         */
/***************************************************************************

void WriteToCC1020Register(char addr, char data)
{
  short val;
  
  val=(short) (addr&0x7F)<<9 | (short) data &0x00FF;
  WriteToCC1020RegisterWord(val);
}

/****************************************************************************/
/*  This routine writes to a single CC1020 register, with address and data  */
/*  given in the same variable                                              */
/***************************************************************************

void WriteToCC1020RegisterWord(short addranddata)
{
  char  BitCounter;
  char Low;
  char High;
  union {
    unsigned short data;
    struct
    {
      char LowByte;
      char HighByte;
    };
  };
  
  PSEL=1;

  data=addranddata;

  PSEL=0;
    
  Low=LowByte;

  // Send address bits 
  for (BitCounter=0;BitCounter<7;BitCounter++)
  {
    PCLK=0;
    PDI=((Low&0x80)>>7);
    Low=Low<<1;
    PCLK=1;
  }
  // Send read/write bit 
  // Ignore bit in data, always use 1 
  
  PCLK=0;
  PDI=1;
  PCLK=1;
  PCLK=0;

  High=HighByte;

  // Send data bits 
  for (BitCounter=0;BitCounter<8;BitCounter++)
  {
    PCLK=0;
    PDI=((High&0x80)>>7);
    High=High<<1;
    PCLK=1;
  }
  PCLK=0;

  PSEL=1;
}

/****************************************************************************/
/*  This routine reads from a single CC1020 register                        */
/***************************************************************************

char ReadFromCC1020Register(char addr)
{
  char BitCounter;
  char Byte;

  PSEL=1;
  
  Byte=addr<<1;
  PSEL=0;
    
  // Send address bits
  for (BitCounter=0;BitCounter<7;BitCounter++)
  {
    PCLK=0;
    PDI=((Byte&0x80)>>7);
    Byte=Byte<<1;
    PCLK=1;
  }
  // Send read/write bit 
  // Ignore bit in data, always use 0 
  
  PCLK=0;
  PDI=0;
  PCLK=1;
 
  PCLK=0;
    
  // Receive data bits 
  
  PDI=1;
   
  TRISC|=0x20; // Set up PDATA as an input
    
  for (BitCounter=0;BitCounter<8;BitCounter++)
  {
    PCLK=1;
    Byte=Byte<<1;
    Byte=Byte|PDO;
    PCLK=0;
  }
  
  TRISC&=~0x20; // Set up PDATA as an output again
  
  PSEL=1;

  return Byte;
}

#endif
  
/****************************************************************************/
/*  This routine resets the CC1020, clearing all registers.                 */
/***************************************************************************  

void ResetCC1020(void)
{
  // Reset CC1020
  WriteToCC1020Register(CC1020_MAIN, 0x0F&~0x01);

  // Bring CC1020 out of reset
  WriteToCC1020Register(CC1020_MAIN, 0x1F);
}


/****************************************************************************/
/*  This routine calibrates the CC1020                                      */
/*  Returns 0 if calibration fails, non-zero otherwise. Checks the LOCK     */
/*  to check for success.                                                   */
/***************************************************************************

char CalibrateCC1020(char PA_POWER)
{
  volatile int TimeOutCounter;
  volatile int nCalAttempt;

  // Turn off PA to avoid spurs during calibration in TX mode
  WriteToCC1020Register(CC1020_PA_POWER,0x00);

  // Calibrate, and re-calibrate if necessary:
  for (nCalAttempt = CAL_ATTEMPT_MAX; (nCalAttempt>0); nCalAttempt--) {

    // Start calibration
    WriteToCC1020Register(CC1020_CALIBRATE,0xB4);

    // Monitor actual calibration start (ref. Errata Note 04 - CC1020)
    for(TimeOutCounter=CAL_TIMEOUT; ((ReadFromCC1020Register(CC1020_STATUS)&0x80)==0x80)&&(TimeOutCounter>0); TimeOutCounter--);
    // Important note:
    // In active mode the CC1020 should theoretically initiate an internal action/process more or less
    // instantly upon receiving any command from e.g. an MCU. However, upon sending a [calibration start]
    // command to CC1020, tests shows that the [STATUS.CAL_COMPLETE]-signal sometimes remains asserted
    // (or idle) close to 100 usec after the command has been originally issued. Consequently this process
    // must be carefully monitored to avoid premature PLL LOCK monitoring; do not proceed with subsequent
    // PLL LOCK monitoring before the calibration has actually completed inside the CC1020! Errata Note 04
    // suggests that [calibration start] should be monitored by a fixed timeout > 100 usec. However, the
    // above method implements an adaptive monitoring of [calibration start], which basically waits for the
    // [STATUS.CAL_COMPLETE]-signal to initialise/deassert (indicating calibration has actually started)
    // before proceeding with monitoring calibration complete and PLL LOCK. Chipcon considers both methods
    // safe, and thus leaves it up to the user, which one to use.

    // Monitor calibration complete
    for(TimeOutCounter=CAL_TIMEOUT; ((ReadFromCC1020Register(CC1020_STATUS)&0x80)==0x00)&&(TimeOutCounter>0); TimeOutCounter--);

    // Monitor lock
    for(TimeOutCounter=LOCK_TIMEOUT; ((ReadFromCC1020Register(CC1020_STATUS)&0x10)==0x00)&&(TimeOutCounter>0); TimeOutCounter--);

    // Abort further recalibration attempts if successful LOCK
    if((ReadFromCC1020Register(CC1020_STATUS)&0x10) == 0x10) {
      break;
    }
  }

  // Restore PA setting
  WriteToCC1020Register(CC1020_PA_POWER, PA_POWER);

  // Return state of LOCK_CONTINUOUS bit
  return ((ReadFromCC1020Register(CC1020_STATUS)&0x10)==0x10);
}

/****************************************************************************/
/*  This routine puts the CC1020 into RX mode (from TX). When switching to  */
/*  RX from PD, use WakeupC1020ToRX first                                   */
/***************************************************************************

char SetupCC1020RX(char RXANALOG, char PA_POWER)
{
  volatile int TimeOutCounter;
  char lock_status;

  // Switch into RX, switch to freq. reg A
  WriteToCC1020Register(CC1020_MAIN,0x11);

  // Setup bias current adjustment
  WriteToCC1020Register(CC1020_ANALOG,RXANALOG);

  // Monitor LOCK
  for(TimeOutCounter=LOCK_TIMEOUT; ((ReadFromCC1020Register(CC1020_STATUS)&0x10)==0)&&(TimeOutCounter>0); TimeOutCounter--);

  // If PLL in lock
  if((ReadFromCC1020Register(CC1020_STATUS)&0x10)==0x10){
    // Indicate PLL in LOCK
    lock_status = LOCK_OK;
  // Else (PLL out of LOCK)
  }else{
    // If recalibration ok
    if(CalibrateCC1020(PA_POWER)){
      // Indicate PLL in LOCK
      lock_status = LOCK_RECAL_OK;
    // Else (recalibration failed)
    }else{
      // Indicate PLL out of LOCK
      lock_status = LOCK_NOK;
    }
  }

  // Switch RX part of CC1020 on
  WriteToCC1020Register(CC1020_MAIN,0x01);

  // Return LOCK status to application
  return (lock_status);
}

/****************************************************************************/
/*  This routine puts the CC1020 into TX mode (from RX). When switching to  */
/*  TX from PD, use WakeupCC1020ToTX first                                  */
/***************************************************************************

char SetupCC1020TX(char TXANALOG, char PA_POWER)
{
  volatile int TimeOutCounter;
  char lock_status;

  // Turn off PA to avoid frequency splatter
  WriteToCC1020Register(CC1020_PA_POWER,0x00);

  // Setup bias current adjustment
  WriteToCC1020Register(CC1020_ANALOG,TXANALOG);

  // Switch into TX, switch to freq. reg B
  WriteToCC1020Register(CC1020_MAIN,0xC1);

  // Monitor LOCK
  for(TimeOutCounter=LOCK_TIMEOUT; ((ReadFromCC1020Register(CC1020_STATUS)&0x10)==0)&&(TimeOutCounter>0); TimeOutCounter--);

  // If PLL in lock
  if((ReadFromCC1020Register(CC1020_STATUS)&0x10)==0x10){
    // Indicate PLL in LOCK
    lock_status = LOCK_OK;
  // Else (PLL out of LOCK)
  }else{
    // If recalibration ok
    if(CalibrateCC1020(PA_POWER)){
      // Indicate PLL in LOCK
      lock_status = LOCK_RECAL_OK;
    // Else (recalibration failed)
    }else{
      // Indicate PLL out of LOCK
      lock_status = LOCK_NOK;
    }
  }

  // Restore PA setting
  WriteToCC1020Register(CC1020_PA_POWER,PA_POWER);

  // Turn OFF DCLK squelch in TX
  WriteToCC1020Register(CC1020_INTERFACE,ReadFromCC1020Register(CC1020_INTERFACE)&~0x10);

  // Return LOCK status to application
  return (lock_status);
}

/****************************************************************************/
/*  This routine puts the CC1020 into power down mode. Use WakeUpCC1020ToRX */
/*  followed by SetupCC1020RX or WakeupCC1020ToTX followed by SetupCC1020TX */
/*  to wake up from power down                                              */
/***************************************************************************

void SetupCC1020PD(void)
{
  // Put CC1020 into power-down
  WriteToCC1020Register(CC1020_MAIN,0x1F);

  // Turn off PA to minimise current draw
  WriteToCC1020Register(CC1020_PA_POWER,0x00);
}

/****************************************************************************/
/*  This routine wakes the CC1020 up from PD mode to RX mode                */
/***************************************************************************

void WakeUpCC1020ToRX(char RXANALOG)
{
  volatile int i;

  // Turn on xtal oscillator core
  WriteToCC1020Register(CC1020_MAIN,0x1B);

  // Setup bias current adjustment
  WriteToCC1020Register(CC1020_ANALOG,RXANALOG);

  // Insert wait routine here, must wait for xtal oscillator to stabilise, 
  // typically takes 2-5ms.
  for (i=0x0260; i > 0; i--);

  // Turn on bias generator
  WriteToCC1020Register(CC1020_MAIN,0x19);

  // Wait for 150 usec
  for (i=0x0010; i > 0; i--);

  // Turn on frequency synthesiser
  WriteToCC1020Register(CC1020_MAIN,0x11);
}

/****************************************************************************/
/*  This routine wakes the CC1020 up from PD mode to TX mode                */
/***************************************************************************

void WakeUpCC1020ToTX(char TXANALOG)
{
  volatile int i;

  // Turn on xtal oscillator core
  WriteToCC1020Register(CC1020_MAIN,0xDB);

  // Setup bias current adjustment
  WriteToCC1020Register(CC1020_ANALOG,TXANALOG);

  // Insert wait routine here, must wait for xtal oscillator to stabilise, 
  // typically takes 2-5ms.
  for (i=0x0260; i > 0; i--);

  // Turn on bias generator
  WriteToCC1020Register(CC1020_MAIN,0xD9);

  // Wait for 150 usec
  for (i=0x0010; i > 0; i--);

  // Turn on frequency synthesiser
  WriteToCC1020Register(CC1020_MAIN,0xD1);
}

/****************************************************************************/
/*  This routine acquires the average RSSI level in dBm                        */
/***************************************************************************
int ReadRSSIlevelCC1020(void){
  char i;
  unsigned char RSSI[5];
  unsigned char FILTER;
  int RSSI_dBm;

  // RSSI formula based on CC1020 datahseet and application note 30:
  // P = 1.5 x RSSI - 3 x VGA_SETTING - RSSI_Offset [dBm]

  /*
  VGA_SETTING based on SmartRF Studio - CC1020, v5.3.0.0
  Note: these settings could be subject to optimisation.
  
  434MHz range:
  -------------------------------------------------
  12.5 => VGA3 = 0x31 => VGA_SETTING = 0x11 = 17dec
  25   => VGA3 = 0x2F => VGA_SETTING = 0x0F = 15dec
  50   => VGA3 = 0x2E => VGA_SETTING = 0x0E = 14dec
  100  => VGA3 = 0x2D => VGA_SETTING = 0x0D = 13dec
  150  => VGA3 = 0x2F => VGA_SETTING = 0x0F = 15dec
  200  => VGA3 = 0x32 => VGA_SETTING = 0x12 = 18dec
  500  => VGA3 = 0x33 => VGA_SETTING = 0x13 = 19dec
  
  868MHz range:
  -------------------------------------------------
  12.5 => VGA3 = 0x2F => VGA_SETTING = 0x0F = 15dec
  25   => VGA3 = 0x2E => VGA_SETTING = 0x0E = 14dec
  50   => VGA3 = 0x2D => VGA_SETTING = 0x0D = 13dec
  100  => VGA3 = 0x2D => VGA_SETTING = 0x0D = 13dec
  150  => VGA3 = 0x2E => VGA_SETTING = 0x0E = 14dec
  200  => VGA3 = 0x30 => VGA_SETTING = 0x10 = 16dec
  500  => VGA3 = 0x34 => VGA_SETTING = 0x14 = 20dec
  *

  // Get current channel spacing
  FILTER = ReadFromCC1020Register(CC1020_FILTER)&0x7F;

  // Calculate average RSSI in dBm (initially without offset):
  for(i = 0; i < 5; i++) {
    RSSI[i] = ReadFromCC1020Register(CC1020_RSSI);
  }
  RSSI_dBm = (int)((((unsigned int)RSSI[0]+(unsigned int)RSSI[1]+(unsigned int)RSSI[2]+(unsigned int)RSSI[3]+(unsigned int)RSSI[4])*3)/(2*5));
  RSSI_dBm = ((RSSI_dBm*3) - (3*(int)(ReadFromCC1020Register(CC1020_VGA3)&0x1F)*2)) / 2;

  // Calculate average RSSI in dBm with offset, according to frequency band:
  // Active frequency is in 800 MHz band
  if( (ReadFromCC1020Register(CC1020_ANALOG)&0x80) == 0x80 ) {
    switch ( FILTER ) {
      case 0x58://12.5 kHz spacing
      case 0x3F://12.5 kHz spacing, optimal sensitivity
        RSSI_dBm = RSSI_dBm - 95;
        break;
      case 0x2F://25 kHz spacing
        RSSI_dBm = RSSI_dBm - 96;
        break;
      case 0x2B://50 kHz spacing
        RSSI_dBm = RSSI_dBm - 95;
        break;
      case 0x25://100 kHz spacing
        RSSI_dBm = RSSI_dBm - 96;
        break;
      case 0x22://150 kHz spacing
        RSSI_dBm = RSSI_dBm - 99;
        break;
      case 0x01://200 kHz spacing
        RSSI_dBm = RSSI_dBm - 99;
        break;
      case 0x00://500 kHz spacing
        RSSI_dBm = RSSI_dBm - 101;
        break;
    }
  // Active frequency is in 434 MHz band
  } else {
    switch ( FILTER ) {
      case 0x38://12.5 kHz spacing
      case 0x2F://25 kHz spacing
        RSSI_dBm = RSSI_dBm - 102;
        break;
      case 0x2B://50 kHz spacing
        RSSI_dBm = RSSI_dBm - 100;
        break;
      case 0x25://100 kHz spacing
        RSSI_dBm = RSSI_dBm - 99;
        break;
      case 0x22://150 kHz spacing
        RSSI_dBm = RSSI_dBm - 101;
        break;
      case 0x01://200 kHz spacing
        RSSI_dBm = RSSI_dBm - 102;
        break;
      case 0x00://500 kHz spacing
        RSSI_dBm = RSSI_dBm - 103;
        break;
      }
  }

  return(RSSI_dBm);
}
*/																			*/