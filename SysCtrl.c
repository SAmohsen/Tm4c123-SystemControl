/******************************************************************************
 *
 * Module: System Control
 *
 * File Name: SysCtrl.c
 *
 * Description:
 * System control configures the overall operation of the device and provides information about the
 * device. Configurable features include reset control, NMI operation, power control, clock control, and low-power modes.
 *
 * Author: Sayed Mohsen
 *
 *******************************************************************************/
#include "SysCtrl.h"
#include "SysCtrl_map.h" 


/*OSC Predefined Clocks*/
#define PLL_CLOCK_400 (400000000UL)
#define PLL_CLOCK_200 (200000000UL)
#define PIOSC       (16000000UL) /* Internal oscillator freq */
#define PIOSC_DIV_4  (4000000UL) /* Internal oscillator freq divided by 4 */
#define LFIOSC     (30000UL)  /* Internal 30K oscillator freq */
#define XTAL32K     (32768UL) /* external 32K oscillator freq */
#define MOSC_CLOCK (16000000UL)


/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/*global Variable To Hold Module Status */
//static SysCtrl_statusType g_sysCtrlStatus = SYSCTRL_NOT_INITIALIZED;

/*global Variable To Hold System Control Configurations */
static const SysCtrl_configType *g_sysCtrlConfigurations = (void *)0;

/*The main oscillator supports the use of a select number of crystals from 4 to 25 MHz */
static const unsigned long g_mainOscillatorAttachedCrystal[21] =
	{
		SYSCTL_RCC_XTAL_4MHZ,
		SYSCTL_RCC_XTAL_4_09MHZ,
		SYSCTL_RCC_XTAL_4_91MHZ,
		SYSCTL_RCC_XTAL_5MHZ,
		SYSCTL_RCC_XTAL_5_12MHZ,
		SYSCTL_RCC_XTAL_6MHZ,
		SYSCTL_RCC_XTAL_6_14MHZ,
		SYSCTL_RCC_XTAL_7_37MHZ,
		SYSCTL_RCC_XTAL_8MHZ,
		SYSCTL_RCC_XTAL_8_19MHZ,
		SYSCTL_RCC_XTAL_10MHZ,
		SYSCTL_RCC_XTAL_12MHZ,
		SYSCTL_RCC_XTAL_12_2MHZ,
		SYSCTL_RCC_XTAL_13_5MHZ,
		SYSCTL_RCC_XTAL_14_3MHZ,
		SYSCTL_RCC_XTAL_16MHZ,
		SYSCTL_RCC_XTAL_16_3MHZ,
		SYSCTL_RCC_XTAL_18MHZ,
		SYSCTL_RCC_XTAL_20MHZ,
		SYSCTL_RCC_XTAL_24MHZ,
		SYSCTL_RCC_XTAL_25MHZ,
};

static const unsigned long g_RcgPreiphralOfsset[16] =
{
 SYSCTL_RCGCWD_R_OFFSET ,         
 SYSCTL_RCGCTIMER_R_OFFSET ,      
 SYSCTL_RCGCGPIO_R_OFFSET ,       
 SYSCTL_RCGCDMA_R_OFFSET ,        
 SYSCTL_RCGCHIB_R_OFFSET ,        
 SYSCTL_RCGCUART_R_OFFSET ,       
 SYSCTL_RCGCSSI_R_OFFSET ,        
 SYSCTL_RCGCI2C_R_OFFSET ,        
 SYSCTL_RCGCUSB_R_OFFSET ,        
 SYSCTL_RCGCCAN_R_OFFSET ,        
 SYSCTL_RCGCADC_R_OFFSET ,        
 SYSCTL_RCGCACMP_R_OFFSET ,       
 SYSCTL_RCGCPWM_R_OFFSET ,        
 SYSCTL_RCGCQEI_R_OFFSET ,        
 SYSCTL_RCGCEEPROM_R_OFFSET ,     
 SYSCTL_RCGCWTIMER_R_OFFSET      
};
/**********************************************************************************************************************
*  LOCAL FUNCTION
*********************************************************************************************************************/
static SysCtrl_systemClock SysCtrl_getOscillatorClock(SysCtrl_ClockSourceType clocksource, SysCtrl_ExternalCrystalValueType xtal);
static SysCtrl_systemClock SysCtrl_getXTALClock(SysCtrl_ExternalCrystalValueType attachedCrystal);


/******************************************************************************
* \Syntax          :void SysCtrl_initPLL(SysCtrl_pllFreqType pllFreq, SysCtrl_systemClock requiredBusClock)       
* \Description     : Function To Init Phase Locked Loop Module                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : pllFreq -> Determine To Use  Pll Divisor "PLL_200MHZ" OR not PLL_400MHZ
									 : requiredBusClock -> desired bus clock To the System 
* \Parameters (out): None                                                                                   
*******************************************************************************/
void SysCtrl_initPLL(SysCtrl_pllFreqType pllFreq, SysCtrl_systemClock requiredBusClock)
{
	SysCtrl_systemClock oscClock;
	SysCtrl_systemDivType div;

	/*bypass PLL while initializing*/
	SYSCTL_RCC2_R |= (1 << BYPASS2); /*The system clock is derived from the OSC source*/

	/*Activate PLL*/
	SYSCTL_RCC2_R &= ~(1 << PWRDN2);

	if (PLL_200MHZ == pllFreq)
	{
		SYSCTL_RCC2_R |= (1 << DIV400); /*Enable PLL Divisor */
		oscClock = PLL_CLOCK_200;
	}

	else if (PLL_400MHZ == pllFreq)
	{
		SYSCTL_RCC2_R &= ~(1 << DIV400); /*Disable PLL Divisor */
		oscClock = PLL_CLOCK_400;
	}

	/*Set system clock divisor.*/
	div = (oscClock / requiredBusClock) - 1;
	SYSCTL_RCC2_R = (SYSCTL_RCC2_R & 0xE03FFFFF) | (div << 22);

	/*wait PLL to lock.*/
	while ((SYSCTL_RIS_R & (1 << PLLLRIS)) == 0); /* wait for PLLRIS bit */

	/* select PLL as the source of system clock*/
	SYSCTL_RCC2_R &= ~(1 << BYPASS2); /* remove bypass and use the PLL */
}

/******************************************************************************
* \Syntax          :void SysCtrl_initSystemClock(const SysCtrl_configType *config_ptr)     
* \Description     : Function To Init System Bus Clock                                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : non Reentrant                                             
* \Parameters (in) : config_ptr -> configure System Bus Clock
* \Parameters (out): None                                                                                   
*******************************************************************************/

void SysCtrl_initSystemClock(const SysCtrl_configType *config_ptr)
{
	SysCtrl_systemClock sysClock = PIOSC;
	SysCtrl_systemDivType div;
	
	/* check if the input configuration pointer is not a NULL_PTR */
	if (config_ptr != (void *)0)
	{
		/*Save Clock Configurations*/
		g_sysCtrlConfigurations = config_ptr;

		/*initilization Sequence*/

		/*Use RCC2 To Extend RCC Functions*/
		SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2; /*Writing in RCC2 Overrides RCC*/

		/*Set oscillator source Clock */
		SYSCTL_RCC2_R = (SYSCTL_RCC2_R & 0xFFFFFF8F) | (g_sysCtrlConfigurations->clocksource << 4);

		if (PLL_OPERATING_NORMALLY == g_sysCtrlConfigurations->plluse)
		{
			/*init PLL */
			SysCtrl_initPLL(g_sysCtrlConfigurations->pll, g_sysCtrlConfigurations->desiredClock);
		}
		else if (PLL_POWERED_DOWM == g_sysCtrlConfigurations->plluse)
		{
			/*MOSC Setup */
			if (CLOCK_SOURCE_MAIN_OSCILLATOR == g_sysCtrlConfigurations->clocksource)
			{
				/*Set external crystal value XTAL*/
				SYSCTL_RCC_R &= ~SYSCTL_RCC_XTAL_M; /*Clear Attached Crystal Value*/
				SYSCTL_RCC_R |= g_mainOscillatorAttachedCrystal[g_sysCtrlConfigurations->xtal];

				/*Enable Main Oscillator*/
				SYSCTL_RCC_R &= ~(1 << MOSCDIS);
			}

			/*Get Oscillator Clock*/
			sysClock = SysCtrl_getOscillatorClock(g_sysCtrlConfigurations->clocksource, g_sysCtrlConfigurations->xtal);

			/*Set system clock divisor.*/
			div = (sysClock / g_sysCtrlConfigurations->desiredClock) - 1;
			SYSCTL_RCC2_R = (SYSCTL_RCC2_R & 0xE03FFFFF) | (div << 22);
		}
	}
}


/******************************************************************************
* \Syntax          :SysCtrl_resetCauseType SysCtrl_getResetCause (void)     
* \Description     : Detecet a reset cause since the previous power-on reset.                                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : non Reentrant                                             
* \Parameters (in) : None
* \Parameters (out): reset Cause                                                                                   
*******************************************************************************/
SysCtrl_resetCauseType SysCtrl_getResetCause (void)
{
	/*Read Reset Cause */
	SysCtrl_resetCauseType resetCause = SYSCTL_RESC_R ; 
	
	/*Clear Reset Cause*/
	SYSCTL_RESC_R = 0 ; /*Writing a 0 to any reset cause clears it.*/
	
	/*Return Reset Cause*/
	return resetCause ; 
}

/******************************************************************************
* \Syntax          : SysCtrl_pllStatusType SysCtrl_getPllStatus( void ) 
* \Description     : get Current PLL Status either "POWERED/LOCKED" OR"UNPOWERD/UNLOCKED"                                                                                              
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : non Reentrant                                             
* \Parameters (in) : None
* \Parameters (out): reset Cause                                                                                   
*******************************************************************************/
SysCtrl_pllStatusType SysCtrl_getPllStatus( void )
{
	SysCtrl_pllStatusType currentPllStatus=((SYSCTL_PLLSTAT_R ==0) ? PLL_IS_UNLOCKED : PLL_IS_LOCKED); 
	return currentPllStatus ; 
}

/******************************************************************************
* \Syntax          : void SysCtrl_peripheralEnable(SysCtrl_systemPeriphralType periphral ,SysCtrl_periphralInstanceType instance)
* \Description     : enable Clock Gate For Any Periphral instance                                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : non Reentrant                                             
* \Parameters (in) : periphral / perphiral instance 
* \Parameters (out): None                                                                                   
*******************************************************************************/
void SysCtrl_peripheralEnable(SysCtrl_systemPeriphralType periphral ,SysCtrl_periphralInstanceType instance)
{
	*((volatile unsigned long *)(SYSCTL_RCGCBASE+g_RcgPreiphralOfsset[periphral])) |= (1<<instance) ; 
}
/******************************************************************************
* \Syntax          : void SysCtrl_peripheralDisable(SysCtrl_systemPeriphralType periphral ,SysCtrl_periphralInstanceType instance)
* \Description     : Disable Clock Gate For Any Periphral instance                                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : non Reentrant                                             
* \Parameters (in) : periphral / perphiral instance 
* \Parameters (out): None                                                                                   
*******************************************************************************/

void SysCtrl_peripheralDisable(SysCtrl_systemPeriphralType periphral ,SysCtrl_periphralInstanceType instance)
{
	*((volatile unsigned long *)(SYSCTL_RCGCBASE+g_RcgPreiphralOfsset[periphral])) &= ~(1<<instance) ; 
}

/******************************************************************************
* \Syntax          :static SysCtrl_systemClock SysCtrl_getXTALClock(SysCtrl_ExternalCrystalValueType attachedCrystal)     
* \Description     : Function To get Attached Crystal Clock                                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : non Reentrant                                             
* \Parameters (in) : attachedCrystal -> attached Crystal
* \Parameters (out):   moscClk  -> MOSC Attached Crystal Clock Value                                                                             
*******************************************************************************/

static SysCtrl_systemClock SysCtrl_getXTALClock(SysCtrl_ExternalCrystalValueType attachedCrystal)
{
	SysCtrl_systemClock moscClk;
	switch (attachedCrystal)
	{

	case XTAL_4MHZ:
		moscClk = 4000000UL;
		break;
	case XTAL_4_09MHZ:
		moscClk = 4096000UL;
		break;
	case XTAL_4_91MHZ:
		moscClk = 4915200UL;
		break;
	case XTAL_5MHZ:
		moscClk = 5000000UL;
		break;
	case XTAL_5_12MHZ:
		moscClk = 5120000UL;
		break;
	case XTAL_6MHZ:
		moscClk = 6000000UL;
		break;
	case XTAL_6_14MHZ:
		moscClk = 6144000UL;
		break;
	case XTAL_7_37MHZ:
		moscClk = 7372800UL;
		break;
	case XTAL_8MHZ:
		moscClk = 8000000UL;
		break;
	case XTAL_8_19MHZ:
		moscClk = 8192000UL;
		break;
	case XTAL_10MHZ:
		moscClk = 10000000UL;
		break;
	case XTAL_12MHZ:
		moscClk = 12000000UL;
		break;
	case XTAL_12_2MHZ:
		moscClk = 12288000UL;
		break;
	case XTAL_13_5MHZ:
		moscClk = 13560000UL;
		break;
	case XTAL_14_3MHZ:
		moscClk = 14318180UL;
		break;
	case XTAL_16MHZ:
		moscClk = 16000000UL;
		break;
	case XTAL_16_3MHZ:
		moscClk = 16384000UL;
		break;
	case XTAL_18MHZ:
		moscClk = 18000000UL;
		break;
	case XTAL_20MHZ:
		moscClk = 20000000UL;
		break;
	case XTAL_24MHZ:
		moscClk = 24000000UL;
		break;
	case XTAL_25MHZ:
		moscClk = 25000000UL;
		break;
	}
	return moscClk;
}


/******************************************************************************
* \Syntax          :static SysCtrl_systemClock SysCtrl_getOscillatorClock(SysCtrl_ClockSourceType clocksource, 
																								SysCtrl_ExternalCrystalValueType xtal)     
* \Description     : Function To get Current OSC Clock                                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : non Reentrant                                             
* \Parameters (in) : clocksource -> oscillator Clock Source
									 :  xtal -> Attached Crystal
* \Parameters (out):   sysClock  -> current System Clock                                                                          
*******************************************************************************/
static SysCtrl_systemClock SysCtrl_getOscillatorClock(SysCtrl_ClockSourceType clocksource, SysCtrl_ExternalCrystalValueType xtal)
{
	SysCtrl_systemClock sysClock = PIOSC;
	switch (clocksource)
	{
	case CLOCK_SOURCE_MAIN_OSCILLATOR:
		sysClock = SysCtrl_getXTALClock(xtal);
		break;
	case CLOCK_SOURCE_PRECISION_INTERNAL_OSCILLATOR:
		sysClock = PIOSC;
		break;
	case CLOCK_SOURCE_PRECISION_INTERNAL_OSCILLATOR_DIVDED_4:
		sysClock = PIOSC_DIV_4;
		break;
	case CLOCK_SOURCE_LOW_FREQUENCY_INTERNAL_OSCILLATOR:
		sysClock = LFIOSC;
		break;
	case CLOCK_SOURCE_HIBERNTION_OSCILLATOR:
		sysClock = XTAL32K;
		break;
	}
	return sysClock;
}
/**********************************************************************************************************************
 *  END OF FILE: FileName.c
 *********************************************************************************************************************/
