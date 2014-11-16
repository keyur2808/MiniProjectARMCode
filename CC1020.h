
/*                                                                           *
 * Revision history:                                                         *
 *                                                                           *
 * $Log: CC1020.h,v $
 * Revision 1.7  2005/11/22 18:17:43  tos
 * Relocated some macros to another file (modemhw.h).
 *
 * Revision 1.6  2005/11/22 15:46:57  tos
 * Mods:
 * - Implemented support for RSSI measurement.
 *
 * Revision 1.5  2003/08/14 11:06:34  tos
 * Modified some function prototypes to enable PA_POWER as input argument.
 *
 * Revision 1.4  2003/06/27 17:22:15  tos
 * Modified setup routines to allow separat RX/TX settings in ANALOG register.
 *
 * Revision 1.3  2003/06/02 14:43:32  tos
 * Recalibrate if LOCK failure after RX/TX setup.
 *
 * Revision 1.2  2003/05/23 13:28:50  tos
 * Tuning+testing before release.
 *
 * Revision 1.1  2003/05/21 09:48:13  tos
 * Initial version in CVS.
 *
 *
 *                                                                           *
 ****************************************************************************/


/* Constants defined for CC1020 */

/* Register addresses */

#define CC1020_MAIN            0x00
#define CC1020_INTERFACE       0x01
#define CC1020_RESET           0x02
#define CC1020_SEQUENCING      0x03
#define CC1020_FREQ_2A         0x04
#define CC1020_FREQ_1A         0x05
#define CC1020_FREQ_0A         0x06
#define CC1020_CLOCK_A         0x07
#define CC1020_FREQ_2B         0x08
#define CC1020_FREQ_1B         0x09
#define CC1020_FREQ_0B         0x0A
#define CC1020_CLOCK_B         0x0B
#define CC1020_VCO             0x0C
#define CC1020_MODEM           0x0D
#define CC1020_DEVIATION       0x0E
#define CC1020_AFC_CONTROL     0x0F
#define CC1020_FILTER          0x10
#define CC1020_VGA1            0x11
#define CC1020_VGA2            0x12
#define CC1020_VGA3            0x13
#define CC1020_VGA4            0x14
#define CC1020_LOCK            0x15
#define CC1020_FRONTEND        0x16
#define CC1020_ANALOG          0x17
#define CC1020_BUFF_SWING      0x18
#define CC1020_BUFF_CURRENT    0x19
#define CC1020_PLL_BW          0x1A
#define CC1020_CALIBRATE       0x1B
#define CC1020_PA_POWER        0x1C
#define CC1020_MATCH           0x1D
#define CC1020_PHASE_COMP      0x1E
#define CC1020_GAIN_COMP       0x1F
#define CC1020_POWERDOWN       0x20
#define CC1020_TEST1           0x21
#define CC1020_TEST2           0x22
#define CC1020_TEST3           0x23
#define CC1020_TEST4           0x24
#define CC1020_TEST5           0x25
#define CC1020_TEST6           0x26
#define CC1020_TEST7           0x27
#define CC1020_STATUS          0x40
#define CC1020_RESET_DONE      0x41
#define CC1020_RSSI            0x42
#define CC1020_AFC             0x43
#define CC1020_GAUSS_FILTER    0x44
#define CC1020_STATUS1         0x45
#define CC1020_STATUS2         0x46
#define CC1020_STATUS3         0x47
#define CC1020_STATUS4         0x48
#define CC1020_STATUS5         0x49
#define CC1020_STATUS6         0x4A
#define CC1020_STATUS7         0x4B

/* Functions for accessing the CC1020 */

extern void ConfigureCC1020(char Count, short Configuration[]);
extern void WriteToCC1020Register(char addr, char data);
extern void WriteToCC1020RegisterWord(short addranddata);
extern char ReadFromCC1020Register(char addr);
extern void ResetCC1020(void);
extern char CalibrateCC1020(char PA_POWER);
extern char SetupCC1020RX(char RXANALOG, char PA_POWER);
extern char SetupCC1020TX(char TXANALOG, char PA_POWER);
extern void SetupCC1020PD(void);
extern void WakeUpCC1020ToRX(char RXANALOG);
extern void WakeUpCC1020ToTX(char TXANALOG);
extern int  ReadRSSIlevelCC1020(void);

// Time-out values
#define CAL_TIMEOUT    0x7FFE
#define LOCK_TIMEOUT   0x7FFE
#define RESET_TIMEOUT  0x7FFE

// LOCK status definitions
#define  LOCK_NOK         0x00
#define  LOCK_OK          0x01
#define  LOCK_RECAL_OK    0x02

// PA power setting
#define PA_VALUE 0xA0
