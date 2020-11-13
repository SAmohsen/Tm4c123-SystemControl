/******************************************************************************
 *
 * Module: System Control
 *
 * File Name: SysCtrl.h
 *
 * Description:Header File For System Control Module 
 * System control configures the overall operation of the device and provides information about the
 * device. Configurable features include reset control, NMI operation, power control, clock control, and low-power modes.
 *
 * Author: Sayed Mohsen
 *
 *******************************************************************************/
#ifndef SYSCTR_H
#define SYSCTR_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Main OSC Clock Source Predefined Set Value */
#define SYSCTL_RCC_XTAL_4MHZ    0x00000180  // 4 MHz
#define SYSCTL_RCC_XTAL_4_09MHZ 0x000001C0  // 4.096 MHz
#define SYSCTL_RCC_XTAL_4_91MHZ 0x00000200  // 4.9152 MHz
#define SYSCTL_RCC_XTAL_5MHZ    0x00000240  // 5 MHz
#define SYSCTL_RCC_XTAL_5_12MHZ 0x00000280  // 5.12 MHz
#define SYSCTL_RCC_XTAL_6MHZ    0x000002C0  // 6 MHz
#define SYSCTL_RCC_XTAL_6_14MHZ 0x00000300  // 6.144 MHz
#define SYSCTL_RCC_XTAL_7_37MHZ 0x00000340  // 7.3728 MHz
#define SYSCTL_RCC_XTAL_8MHZ    0x00000380  // 8 MHz
#define SYSCTL_RCC_XTAL_8_19MHZ 0x000003C0  // 8.192 MHz
#define SYSCTL_RCC_XTAL_10MHZ   0x00000400  // 10 MHz
#define SYSCTL_RCC_XTAL_12MHZ   0x00000440  // 12 MHz
#define SYSCTL_RCC_XTAL_12_2MHZ 0x00000480  // 12.288 MHz
#define SYSCTL_RCC_XTAL_13_5MHZ 0x000004C0  // 13.56 MHz
#define SYSCTL_RCC_XTAL_14_3MHZ 0x00000500  // 14.31818 MHz
#define SYSCTL_RCC_XTAL_16MHZ   0x00000540  // 16 MHz
#define SYSCTL_RCC_XTAL_16_3MHZ 0x00000580  // 16.384 MHz
#define SYSCTL_RCC_XTAL_18MHZ   0x000005C0  // 18.0 MHz (USB)
#define SYSCTL_RCC_XTAL_20MHZ   0x00000600  // 20.0 MHz (USB)
#define SYSCTL_RCC_XTAL_24MHZ   0x00000640  // 24.0 MHz (USB)
#define SYSCTL_RCC_XTAL_25MHZ   0x00000680  // 25.0 MHz (USB)


/*reset cause after reset. */
#define SYSCTRL_EXTERNAL_RESET            (unsigned long)1
#define SYSCTRL_POWER_ON_RESET            (unsigned long)2
#define SYSCTRL_BROWN_OUT_RESET           (unsigned long)4
#define SYSCTRL_WDT0_RESET                (unsigned long)8
#define SYSCTRL_SOFTWARE_RESET            (unsigned long)16
#define SYSCTRL_WDT1_RESET                (unsigned long)32
#define SYSCTRL_MOSC_FAILURE_RESET        (unsigned long)65536



/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef enum 
{
	SYSCTRL_NOT_INITIALIZED,
	SYSCTRL_INITIALIZED,
}SysCtrl_statusType;

/*Type Definitions For Reset Causes */
typedef unsigned long SysCtrl_resetCauseType;

/*Type Definitions For Setting PLL Usage. */
typedef enum {
	PLL_OPERATING_NORMALLY,/*Power UP PLL*/
	PLL_POWERED_DOWM/*Power Down PLL*/
}SysCtrl_pllUsageType;

/*Type Definitions For Current  PLL Status. */
typedef enum {
	PLL_IS_UNLOCKED,/*The PLL is unpowered or is not yet locked.*/
	PLL_IS_LOCKED,/*The PLL is powered and locked.*/
} SysCtrl_pllStatusType;

/*Type Definitions For Current  PLL Divisor. */
typedef enum {
	PLL_200MHZ ,/*Divide PLL By 2.. Provide small Range Of Freqs*/
	PLL_400MHZ,/*Divide PLL..Provide wider Range Of Freqs*/
}SysCtrl_pllFreqType;


/*Type Definitions For Selecting the input source for the OSC. */
typedef enum {
	CLOCK_SOURCE_MAIN_OSCILLATOR,/*External, Default 16Mhz launchpad*/
	CLOCK_SOURCE_PRECISION_INTERNAL_OSCILLATOR,/*16MHZ*/
	CLOCK_SOURCE_PRECISION_INTERNAL_OSCILLATOR_DIVDED_4,/*4MHZ*/
	CLOCK_SOURCE_LOW_FREQUENCY_INTERNAL_OSCILLATOR,/*30-kHz internal oscillator*/
	CLOCK_SOURCE_HIBERNTION_OSCILLATOR =7/*32.768-kHz external oscillator*/
}SysCtrl_ClockSourceType;

/*Type Definition For Main OSC Attached Crystal Clock Source Value*/
typedef enum 
{
	XTAL_4MHZ  ,  
	XTAL_4_09MHZ ,
	XTAL_4_91MHZ ,
	XTAL_5MHZ    ,
	XTAL_5_12MHZ ,
	XTAL_6MHZ    ,
	XTAL_6_14MHZ ,
	XTAL_7_37MHZ ,
	XTAL_8MHZ    ,
	XTAL_8_19MHZ ,
	XTAL_10MHZ   ,
	XTAL_12MHZ   ,
	XTAL_12_2MHZ ,
	XTAL_13_5MHZ ,
	XTAL_14_3MHZ ,
	XTAL_16MHZ  ,
	XTAL_16_3MHZ,
	XTAL_18MHZ  ,
	XTAL_20MHZ  ,
	XTAL_24MHZ ,
	XTAL_25MHZ,
}SysCtrl_ExternalCrystalValueType;

/*Type Defintion For System Periphrals*/
typedef enum {
	SYSCTL_WD,         
 SYSCTL_TIMER,    
 SYSCTL_GPIO,      
 SYSCTL_CDMA,        
 SYSCTL_CHIB,       
 SYSCTL_CUART,  
 SYSCTL_CSSI,        
 SYSCTL_CI2C,        
 SYSCTL_CUSB,        
 SYSCTL_CCAN,        
 SYSCTL_CADC,        
SYSCTL_CACMP,       
 SYSCTL_CPWM,        
 SYSCTL_CQEI,        
 SYSCTL_CEEPROM,     
SYSCTL_CWTIMER,     
}SysCtrl_systemPeriphralType;

/*instance For For each Periphral ..Ex =Uart0*/
typedef unsigned char  SysCtrl_periphralInstanceType ; 

typedef unsigned long SysCtrl_systemDivType ;
typedef unsigned long SysCtrl_systemClock ;

/*Sytyem Contrl Configurations*/
typedef struct {
	SysCtrl_ClockSourceType clocksource; 
	SysCtrl_ExternalCrystalValueType xtal;
	SysCtrl_pllUsageType plluse ;
	SysCtrl_pllFreqType pll;
	SysCtrl_systemClock desiredClock ; 
}SysCtrl_configType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
extern SysCtrl_configType sysCtrl_Config ;
 
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
void SysCtrl_initClock(const SysCtrl_configType *config_ptr);
void SysCtrl_peripheralEnable(SysCtrl_systemPeriphralType periphral ,SysCtrl_periphralInstanceType instance) ; 
SysCtrl_resetCauseType SysCtrl_getResetCause (void) ; 
void SysCtrl_initSystemClock(const SysCtrl_configType *config_ptr) ; 
void SysCtrl_initPLL(SysCtrl_pllFreqType pllFreq, SysCtrl_systemClock requiredBusClock) ; 
void SysCtrl_peripheralDisable(SysCtrl_systemPeriphralType periphral ,SysCtrl_periphralInstanceType instance);
#endif  /* SYSCTR_H */

/**********************************************************************************************************************
 *  END OF FILE: SYSCTR_H.h
 *********************************************************************************************************************/
