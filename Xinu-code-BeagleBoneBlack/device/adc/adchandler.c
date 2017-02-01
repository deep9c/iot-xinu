#include <xinu.h>

extern sid32 readSem;

/**************************************************************************
function name:  adchandler
description:    ADC Interrupt handler. Just clear the interrupt status, and 
                then signal the semaphore to notify waiting read function.

input:          xnum-------->IRQ number
date:           11/11/2016
author:         Kai Zhang				 
**************************************************************************/
void adchandler(uint32 xnum)
{
	struct	dentry	*devptr;            // Address of device control blk	
	struct	adc_csreg *csrptr;          // Address of UART's CSR	
	volatile uint32	irqStatus = 0;      // Interrupt identification	
	
	// Get register base address of the device (assume ADC for now) 
	devptr = (struct dentry *) &devtab[ADC];
	csrptr = (struct adc_csreg *) devptr->dvcsr;
	irqStatus = csrptr->irqStatus;      // read interrupt status
	csrptr->irqStatus = irqStatus;      // clear interrupt by write back

	ADCStepDisable(csrptr, 0);
	semcount(readSem);                  //Add semaphore
	signal(readSem);                    //signal and notify read function	
	return;
}