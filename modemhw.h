
/*                                                                           *
 * Revision history:                                                         *
 *                                                                           *
 * $Log: modemhw.h,v $
 * Revision 1.2  2005/11/22 18:22:05  tos
 * Included some useful macroes (relocated from cc1020.h).
 *
 * Revision 1.1  2003/05/21 09:48:19  tos
 * Initial version in CVS.
 *
 *
 *                                                                           *
 ****************************************************************************/


/* Pin usage definitions */
#define PDO         RC4
#define PDI         RC5
#define PCLK        RC3
#define PSEL        RC2
#define DIO         RC1
#define DCLK        RB0
#define CHP_OUT     RB2

#define RD_LED      RA2
#define BUTTON1     RA2
#define CARRIER_LED RA4
#define TD_LED      RA5
#define BUTTON2     RA5

#define READY       RC0
#define DATA_OUT    RC6
#define DATA_IN     RC7
#define AWAKE       RB1
#define SYNC        RA4
#define PD          RB4
#define RX_TX       RB5


// Time-out values
#define CAL_TIMEOUT    0x7FFE
#define LOCK_TIMEOUT   0x7FFE

// Maximum number of re-calibration attempts upon calibration failure
#define CAL_ATTEMPT_MAX   4

// LOCK status definitions
#define  LOCK_NOK         0x00
#define  LOCK_OK          0x01
#define  LOCK_RECAL_OK    0x02


// Macros for turning on and off the LEDs (for testing only)
#define FALSE 0
#define TRUE (!FALSE)

// RXLED = Red
#define SET_RXLED(STATE)                           \
  TRISA = (STATE) ? (TRISA&~0x04) : (TRISA|0x04);

// TXLED = Yellow
#define SET_TXLED(STATE)                           \
  TRISA = (STATE) ? (TRISA&~0x20) : (TRISA|0x20);


// State definitions
enum StateType {IDLE_STATE=0,TXRX_STATE=1,TX_PN9_STATE=2,RX_STATE=3};
