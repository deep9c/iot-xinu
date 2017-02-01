#include <xinu.h>

//CLOCK domain register (part)
#define CM_PER                                (0x44E00000)		
#define CM_PER_L3S_CLKSTCTRL                  (0x44E00004)
#define CM_PER_L3_CLKSTCTRL                   (0x44E0000C)
#define CM_PER_L3_INSTR_CLKCTRL               (0x44E000DC)
#define CM_PER_L3_CLKCTRL                     (0x44E000E0)
#define CM_PER_OCPWP_L3_CLKSTCTRL             (0x44E0012C)

#define CM_WKUP                               (0x44E00400)		
#define CM_WKUP_CLKSTCTRL                     (0x44E00400)
#define CM_WKUP_CONTROL_CLKCTRL               (0x44E00404)
#define CM_WKUP_L4WKUP_CLKCTRL                (0x44E0040C)
#define CM_L3_AON_CLKSTCTRL                   (0x44E00418)
#define CM_WKUP_ADC_TSC_CLKCTRL               (0x44E004BC)
#define CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL      (0x44E004CC)

//Interrupt controller register (part)
#define INTERRUPT_CONTROLLER                  (0x48200000)
#define INTC_THRESHOLD                        (0x48200068)

//MACRO to read and write register
#define HWREG(n)	(*((volatile unsigned int *)(n)))

/**************************************************************************
function name:  InitializeADCClock
description:    Initialize system clock for ADC, ADC mainly rely on L4 clk
input:          void
date:           11/10/2016
author:         Kai Zhang				 
**************************************************************************/
void InitializeADCClock()
{
	InitialL4Clock();
}

/**************************************************************************
function name:  InitialL4Clock
description:    Initialize system L4 clk 
input:          void
date:           11/10/2016
author:         Kai Zhang				 
**************************************************************************/
void InitialL4Clock()
{


	HWREG(CM_WKUP_CONTROL_CLKCTRL) |= 0x02;                             //enbale wkup control clk
	while(( HWREG(CM_WKUP_CONTROL_CLKCTRL) & 0x03 ) != 0x02 );          //wait 

	HWREG(CM_WKUP_CLKSTCTRL) |= 0x02;                                   //enbale wkup st clk
	while(( HWREG(CM_WKUP_CLKSTCTRL) & 0x03 ) != 0x02 );                //wait 

	HWREG(CM_WKUP_CLKSTCTRL) |= 0x02;                                   //enbale wkup st clk
	while(( HWREG(CM_WKUP_CLKSTCTRL) & 0x03 ) != 0x02 );                //wait

	HWREG(CM_L3_AON_CLKSTCTRL) |= 0x02;                                 //enbale wkup st clk
	while(( HWREG(CM_L3_AON_CLKSTCTRL) & 0x03 ) != 0x02 );              //wait

	HWREG(CM_WKUP_ADC_TSC_CLKCTRL) |= 0x02;                             //enbale wkup st clk
	while(( HWREG(CM_WKUP_ADC_TSC_CLKCTRL) & 0x03 ) != 0x02 );          //wait

	while(( HWREG(CM_WKUP_CONTROL_CLKCTRL) & (0x03<<16) ) != 0x00 );            //wait wkup control clk fully functional
	while(( HWREG(CM_L3_AON_CLKSTCTRL) & (0x01<<3) ) == 0x00 );                 //wait L3 AON gclk active
	while(( HWREG(CM_WKUP_L4WKUP_CLKCTRL) & (0x03<<16) ) != 0x00 );             //wait L4 WKUP CLK fully functional
	while(( HWREG(CM_WKUP_CLKSTCTRL) & (0x01<<2) ) == 0x00 );                   //wait L4 WKUP GCLK active
	while(( HWREG(CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL) & (0x01<<2) ) == 0x00 );    //wait L4 WKUP aon CLK active
	while(( HWREG(CM_WKUP_CLKSTCTRL) & (0x01<<14) ) == 0x00 );                  //wait ADC fclk active
	while(( HWREG(CM_WKUP_ADC_TSC_CLKCTRL) & (0x03<<16) ) != 0x00 );            //wait ADC module fully functinoal
}

/**************************************************************************
function name:  InitialL3Clock
description:    Initialize system L3 clk 
input:          void
date:           11/10/2016
author:         Kai Zhang				 
**************************************************************************/
void InitialL3Clock()
{	
	HWREG(CM_PER_L3_CLKCTRL) |= 0x02;                                //wake up L3 clock control
	while(( HWREG(CM_PER_L3_CLKCTRL) & 0x03 ) != 0x02 );             //wait 

	HWREG(CM_PER_L3_INSTR_CLKCTRL) |= 0x02;                          //wake up L3 instr clock
	while(( HWREG(CM_PER_L3_INSTR_CLKCTRL) & 0x03 ) != 0x02 );       //wait

	HWREG(CM_PER_L3_CLKSTCTRL) |= 0x02;                              //wake up L3 st clock
	while(( HWREG(CM_PER_L3_CLKSTCTRL) & 0x03 ) != 0x02 );           //wait
		  
	HWREG(CM_PER_OCPWP_L3_CLKSTCTRL) |= 0x02;                        //wake up ocp L3 st clock
	while(( HWREG(CM_PER_OCPWP_L3_CLKSTCTRL) & 0x03 ) != 0x02 );     //wait

	HWREG(CM_PER_L3S_CLKSTCTRL) |= 0x02;                             //wake up L3s st clock
	while(( HWREG(CM_PER_L3S_CLKSTCTRL) & 0x03 ) != 0x02 );          //wait

	while(( HWREG(CM_PER_L3_CLKCTRL) & (0x03<<16) ) != 0x00 );        //wait L3 clock fully functional.
	while(( HWREG(CM_PER_L3_INSTR_CLKCTRL) & (0x03<<16) ) != 0x00 );  //wait L3 instr clock fully functional
	while(( HWREG(CM_PER_L3_CLKSTCTRL) & (0x01<<4) ) == 0x00 );       //wait L3_GCLK active
	while(( HWREG(CM_PER_OCPWP_L3_CLKSTCTRL) & (0x01<<4) ) == 0x00 ); //wait ocpwp L3_GCLK active	
	while(( HWREG(CM_PER_L3S_CLKSTCTRL) & (0x01<<3) ) == 0x00 );      //wait L3S GCLK active
}

/**************************************************************************
function name:  InitialADCInterrupt
description:    Initialize system interrupt for ADC 
input:          void
date:           11/10/2016
author:         Kai Zhang				 
**************************************************************************/
void InitialADCInterrupt(struct	dentry *devptr)
{
	struct	intc_csreg *csrptr = (struct intc_csreg *)0x48200000;   //get intr control register start address
	csrptr->threshold = 0X0FF;                                      //disable the priority threshold
	set_evec(devptr->dvirq, (uint32)devptr->dvintr);                //register the interrupt handler and priority
	csrptr->ilr[devptr->dvirq] &= ~(0x01);                          //set ADC to IRQ interrupt
	csrptr->ilr[devptr->dvirq] |= (0x0A<<2);                        //priority is 10	 

}
