#ifndef SYSCTR_MAP_H
#define SYSCTR_MAP_H


/*Run Mode Clock Gating Control*/
#define SYSCTL_RCGCBASE   0x400fe600


#define SYSCTL_RCGCWD_R_OFFSET         0x00
#define SYSCTL_RCGCTIMER_R_OFFSET      0x04
#define SYSCTL_RCGCGPIO_R_OFFSET       0x08
#define SYSCTL_RCGCDMA_R_OFFSET        0x0C
#define SYSCTL_RCGCHIB_R_OFFSET        0x14
#define SYSCTL_RCGCUART_R_OFFSET       0x18
#define SYSCTL_RCGCSSI_R_OFFSET        0x1C
#define SYSCTL_RCGCI2C_R_OFFSET        0x20
#define SYSCTL_RCGCUSB_R_OFFSET        0x28
#define SYSCTL_RCGCCAN_R_OFFSET        0x34
#define SYSCTL_RCGCADC_R_OFFSET        0x38
#define SYSCTL_RCGCACMP_R_OFFSET       0x3C
#define SYSCTL_RCGCPWM_R_OFFSET        0x40
#define SYSCTL_RCGCQEI_R_OFFSET        0x44
#define SYSCTL_RCGCEEPROM_R_OFFSET     0x58
#define SYSCTL_RCGCWTIMER_R_OFFSET     0x5C
	


#define SYSCTL_RCC_R            (*((volatile unsigned long *)0x400FE060))
#define SYSCTL_RCC_XTAL_M       0x000007C0  // Crystal Value
#define MOSCDIS 0


#define SYSCTL_RCC2_R				(*((volatile  unsigned long *)0x400FE070))
#define PWRDN2 13
#define BYPASS2 11
#define DIV400 30 
#define SYSCTL_RCC2_USERCC2     0x80000000  // Use RCC2

#define SYSCTL_RIS_R            (*((volatile  unsigned long*)0x400FE050))
#define PLLLRIS 6

#define SYSCTL_PLLSTAT_R        (*((volatile  unsigned long *)0x400FE168))
#define SYSCTL_RESC_R           (*((volatile unsigned long *)0x400FE05C))



#endif 
