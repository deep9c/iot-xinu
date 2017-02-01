/*
All the dirver code is followed by the AM335X chip datasheet
chapter 2 --------- Memory map
chapter 6 --------- Interrupt
chapter 8 --------- Power, Reset, and Clock management
chapter 12 -------- Touch screen controller
*/

#include <xinu.h>

sid32 readSem;			// Read semaphore

//just want to trace all the register I care about
void TestRegAddress(struct adc_csreg* pReg)
{
	kprintf("revison = %x, %x\r\n", (unsigned int)&pReg->revison,pReg->revison);
	kprintf("sysconfig = %x, %x\r\n", &pReg->sysconfig,pReg->sysconfig);
	kprintf("irqStatusRaw = %x, %x\r\n", (unsigned int)&pReg->irqStatusRaw,pReg->irqStatusRaw);
	kprintf("irqStatus = %x, %x\r\n", (unsigned int)&pReg->irqStatus,pReg->irqStatus);
	kprintf("irqEnableSet = %x, %x\r\n", (unsigned int)&pReg->irqEnableSet,pReg->irqEnableSet);	
	kprintf("irqEnableClr = %x, %x\r\n", (unsigned int)&pReg->irqEnableClr,pReg->irqEnableClr);
	kprintf("irqWakeup = %x, %x\r\n", (unsigned int)&pReg->irqWakeup,pReg->irqWakeup);
	kprintf("dmaEnableSet = %x, %x\r\n", (unsigned int)&pReg->dmaEnableSet,pReg->dmaEnableSet);
	kprintf("dmaEnableClr = %x, %x\r\n", (unsigned int)&pReg->dmaEnableClr,pReg->dmaEnableClr);
	kprintf("ctrl = %x, %x\r\n", (unsigned int)&pReg->ctrl,pReg->ctrl);
	kprintf("adcStat = %x, %x\r\n", (unsigned int)&pReg->adcStat,pReg->adcStat);
	kprintf("adcRange = %x, %x\r\n", (unsigned int)&pReg->adcRange,pReg->adcRange);
	kprintf("adcClkDiv = %x, %x\r\n", (unsigned int)&pReg->adcClkDiv,pReg->adcClkDiv);
	kprintf("adcMisc = %x, %x\r\n", (unsigned int)&pReg->adcMisc,pReg->adcMisc);
	kprintf("stepEnable = %x, %x\r\n", (unsigned int)&pReg->stepEnable,pReg->stepEnable);
	kprintf("idleConfig = %x, %x\r\n", (unsigned int)&pReg->idleConfig,pReg->idleConfig);
	kprintf("tsChargeStepconfig = %x, %x\r\n", (unsigned int)&pReg->tsChargeStepconfig,pReg->tsChargeStepconfig);
	kprintf("tsChargeDelay = %x, %x\r\n", (unsigned int)&pReg->tsChargeDelay,pReg->tsChargeDelay);
	int i=0;
	for(i=0;i<16;i++)
	{
		kprintf("stepConfig%d = %x, %x\r\n", i, (unsigned int)&pReg->step[i].stepConfig,pReg->step[i].stepConfig);
		kprintf("stepDelay%d = %x, %x\r\n", i, (unsigned int)&pReg->step[i].stepDelay,pReg->step[i].stepDelay);
	}
	for(i=0;i<2;i++)
	{
		kprintf("fifoCount%d = %x, %x\r\n", i,(unsigned int)&pReg->fifoInfo[i].fifoCount,pReg->fifoInfo[i].fifoCount);
		kprintf("fifoThreshold%d = %x, %x\r\n", i, (unsigned int)&pReg->fifoInfo[i].fifoThreshold,pReg->fifoInfo[i].fifoThreshold);
		kprintf("dmaReq%d = %x, %x\r\n", i, (unsigned int)&pReg->fifoInfo[i].dmaReq,pReg->fifoInfo[i].dmaReq);
	}
	kprintf("fifoData0 = %x, %x\r\n", (unsigned int)&pReg->fifoData0,pReg->fifoData0);
	kprintf("fifoData1 = %x, %x\r\n", (unsigned int)&pReg->fifoData1,pReg->fifoData1);

	int bank =16;
	struct	intc_csreg *csrptr = (struct intc_csreg *)0x48200000;
	//kprintf("pending irq = %x\r\n",csrptr->);
	kprintf("ITR = %x\r\n",csrptr->banks[bank].itr);
	kprintf("mir = %x\r\n",csrptr->banks[bank].mir);
	kprintf("mir_clear = %x\r\n",csrptr->banks[bank].mir_clear);
	kprintf("mir_set = %x\r\n",csrptr->banks[bank].mir_set);
	kprintf("isr_set = %x\r\n",csrptr->banks[bank].isr_set);
	kprintf("isr_clear = %x\r\n",csrptr->banks[bank].isr_clear);
	kprintf("pending_irq = %x\r\n",csrptr->banks[bank].pending_irq);
	kprintf("pending_fiq = %x\r\n",csrptr->banks[bank].pending_fiq);
}

/**************************************************************************
function name:  ADCConfigure
description:    General ADC module configuration.

input:          devptr -------> base address of ADC register

date:           11/10/2016
author:         Kai Zhang	 
**************************************************************************/
void ADCConfigure(struct adc_csreg *pReg)
{
	//set up sample rate 3MHz
	ADCConfigureAFEClock(pReg, 3000000);
	//Enable touch screen transistor
	//ADCTSTransistorConfig(pReg);
	//Make step config register writable
	ADCStepConfigWriteProtect(pReg, 1);
	//Setup step config register
	StepConfig(pReg);
	//Make ADC module works as general purpose
	ADCWorkModeConfigue(pReg,ADC_GENERAL_PURPOSE_MODE);
	//Enable step config
	//ADCStepEnable(pReg,1);
	//Clear all the interrupts
	ADCClearInterruptStatus(pReg);
	//Enable ADC IRQ
	ADCSetInterruptBit(pReg,ADC_END_OF_SEQUENCE_INTR);
	//kprintf("ADCSetInterruptBit\n");
	//Enable ADC module
	ADCEnableModual(pReg);
}

/**************************************************************************
function name:  ADCConfigureAFEClock
description:    Setup clk divison for ADC, the master input clk is 24MHz,
                if you want the clk is 3Mhz, 24/3 = 8, 8-1 = 7, so just
                write 7 into clk div register.

input:          devptr -------> base address of ADC register
                freq ---------> work frequency
date:           11/10/2016
author:         Kai Zhang	 
**************************************************************************/
void ADCConfigureAFEClock(struct adc_csreg *pReg, unsigned int freq)
{
	unsigned int clkDiv = 24000000/freq - 1;
	pReg->adcClkDiv &= ~(0xFFFF);
	pReg->adcClkDiv |= clkDiv;
}

/**************************************************************************
function name:  ADCTSTransistorConfig
description:    Setup touch screen transistor

input:          devptr -------> base address of ADC register
date:           11/10/2016
author:         Kai Zhang	 
**************************************************************************/
void ADCTSTransistorConfig(struct adc_csreg *pReg)
{
	pReg->ctrl |= (0x01<<7);		//enable touch screen transistor(useless in this case)
}

/**************************************************************************
function name:  ADCStepConfigWriteProtect
description:    Make step config register writable or not writable
                mode.

input:          devptr -------> base address of ADC register
                isWritable----> whether or nor writable
date:           11/10/2016
author:         Kai Zhang	 
**************************************************************************/
void ADCStepConfigWriteProtect(struct adc_csreg *pReg, unsigned int isWritable)
{
	pReg->ctrl &= ~ (0x01<<2);
	pReg->ctrl |= (isWritable<<2);
}
/**************************************************************************
function name:  ADCWorkModeConfigue
description:    Configure ADC work in touch screen mode or general purpose
                mode.

input:          devptr -------> base address of ADC register
                workMode------> 1) 4 wires mode
                                2) 5 wire mode
                                3) 6 wire mode
                                4) GP mode
date:           11/10/2016
author:         Kai Zhang	 
**************************************************************************/
void ADCWorkModeConfigue(struct adc_csreg *pReg,unsigned int workMode)
{
	pReg->ctrl &= ~ ADC_WORK_MODE_CLEAR;
	pReg->ctrl |= (workMode<<ADC_WORK_MODE_SHIFT);
}
/**************************************************************************
function name:  ADCStepEnable
description:    Enable the step module

input:          devptr -------> base address of ADC register
                selectStep----> which step
date:           11/10/2016
author:         Kai Zhang	 
**************************************************************************/
void ADCStepEnable(struct adc_csreg *pReg, unsigned int selectStep)
{
	pReg->stepEnable |= (0x01<<selectStep);
}

/**************************************************************************
function name:  ADCStepDisable
description:    Disable the step module

input:          devptr -------> base address of ADC register
                selectStep----> which step
date:           11/10/2016
author:         Kai Zhang	 
**************************************************************************/
void ADCStepDisable(struct adc_csreg *pReg, unsigned int selectStep)
{
	pReg->stepEnable &= ~(0x01<<selectStep);
}

/**************************************************************************
function name:  ADCClearInterruptStatus
description:    Clear all the interrupt status

input:          devptr -------> base address of ADC register
                
date:           11/10/2016
author:         Kai Zhang	 
**************************************************************************/
void ADCClearInterruptStatus(struct adc_csreg *pReg)
{
	pReg->irqStatus |= 0x7FFF;
}

/**************************************************************************
function name:  ADCEnableModual
description:    Eable the whole ADC module

input:          devptr -------> base address of ADC register
                
date:           11/10/2016
author:         Kai Zhang	 
**************************************************************************/
void ADCEnableModual(struct adc_csreg *pReg)
{
	pReg->ctrl |= 0x01;
}
/**************************************************************************
function name:  ADCDisableModual
description:    Disable the whole ADC module

input:          devptr -------> base address of ADC register
                
date:           11/10/2016
author:         Kai Zhang	 
**************************************************************************/
void ADCDisableModual(struct adc_csreg *pReg)
{
	pReg->ctrl &= ~(0x01);
}

/**************************************************************************
function name:  StepConfig
description:    Set up step config register.

input:          devptr -------> base address of ADC register
                stepSelect----> which step register to use

date:           11/10/2016
author:         Kai Zhang	 
**************************************************************************/
void StepConfig(struct adc_csreg *pReg)
{
	//step 1, singel ended mode
	ADCStepOperationModeControl(pReg,0,ADC_SINGLE_ENDED_OPER_MODE);	
	//step 1, set channel and reference voltage
	ADCStepConfig(pReg, 0, ADC_CHANNEL1, ADC_POSITIVE_REF_VDDA, ADC_CHANNEL1, ADC_NEGATIVE_REF_VSSA);
	//step 1, xpp-on, xnp-off, ypp-on
	ADCStepAnalogSupplyConfig(pReg, 0, 0, 0, 0);
	//step 1, xnn-on, ypn-off, ynn-off, won-off
	ADCStepAnalogGroundConfig(pReg, 0, 0, 0, 0, 0);
	//use FIFO_0
	ADCStepFIFOSelConfig(pReg, 0, ADC_FIFO_0);
	//SW continuous mode
	ADCStepModeConfig(pReg, 0, ADC_MODE_SW_CONTINUOUS);
	//average samples
	ADCStepAverge(pReg,0,16);
}
/**************************************************************************
function name:  ADCStepAverge
description:    set up sample averaging

input:          devptr -------> base address of ADC register
                stepSelect----> which step to select
                sampleNum ----> How many samples you want to average
date:           11/10/2016
author:         Kai Zhang	 
**************************************************************************/
void ADCStepAverge(struct adc_csreg *pReg, int stepSelect, int sampleNum)
{
	if(sampleNum == 2)
		pReg->step[stepSelect].stepConfig |= 0x01<<2;
	else if(sampleNum == 4)
		pReg->step[stepSelect].stepConfig |= 0x02<<2;
	else if(sampleNum == 8)
		pReg->step[stepSelect].stepConfig |= 0x03<<2;	
	else if(sampleNum == 16)
		pReg->step[stepSelect].stepConfig |= 0x04<<2;
	else
		return;
}
/**************************************************************************
function name:  ADCStepOperationModeControl
description:    set up operation mode, it has 2 modes, Single End mode and
                Differential mode.

input:          devptr -------> base address of ADC register
                stepSelect----> which step to select
                mode ---------> 1 : differential mode
								0 : single ended mode
date:           11/10/2016
author:         Kai Zhang	 
**************************************************************************/
void ADCStepOperationModeControl(struct adc_csreg *pReg, unsigned int stepSelect, unsigned int mode)
{
	if(mode)
	{
		pReg->step[stepSelect].stepConfig |= ADC_STEPCONFIG_DIFF_CNTRL;
	}
	else
	{
		pReg->step[stepSelect].stepConfig &= ~ADC_STEPCONFIG_DIFF_CNTRL;
	}
}

/**************************************************************************
function name:  ADCTSStepConfig
description:    For a given step, set up the reference voltage and input
input:          devptr ----------> base address of ADC register
                stepSelect-------> which step register to use
                positiveChannel--> input channel
                positiveRef------> positive reference voltage (VDDA)
                negativeChannel--> negative channel, used for differential
                negativeRef------> negative reference voltage (VSSA)
date:           11/10/2016
author:         Kai Zhang				 
**************************************************************************/
void ADCStepConfig(struct adc_csreg *pReg, unsigned int stepSelect, 
					unsigned int positiveChannel,unsigned int positiveRef,
					unsigned int negativeChannel,unsigned int negativeRef)
{
	//configure the nagtive reference
	pReg->step[stepSelect].stepConfig &= ~ SEL_RFM_SWC_CLEAR;                //clear
	pReg->step[stepSelect].stepConfig |= negativeRef<<SEL_RFM_SWC_SHIFT;     //write

	//configure the positive input channel
	pReg->step[stepSelect].stepConfig &= ~ SEL_INP_SWC_CLEAR;                //clear
	pReg->step[stepSelect].stepConfig |= positiveChannel<<SEL_INP_SWC_SHIFT; //write

	//configure the negative input channel,
	pReg->step[stepSelect].stepConfig &= ~ SEL_INM_SWC_CLEAR;                //clear
	pReg->step[stepSelect].stepConfig |= negativeChannel<<SEL_INM_SWC_SHIFT; //write

	//configure the positive reference
	pReg->step[stepSelect].stepConfig &= ~ SEL_RFP_SWC_CLEAR;                 //clear
	pReg->step[stepSelect].stepConfig |= positiveRef<<SEL_RFP_SWC_SHIFT;      //write
}

/**************************************************************************
function name:  ADCTSStepAnalogSupplyConfig
description:    For a given step, set up the reference voltage and input.
                Notice that only AIN0~AIN4 needs this setting, since these
                5 pins can also be used in touch screen, so there are some
                pull up voltage connect to this 5 pins. Drivers for
                general purpose just need to let these pins floating.

input:          devptr -------> base address of ADC register
                stepSelect----> which step register to use
                xppsw--> ON/OFF
                xnpsw------> ON/OFF
                yppsw--> ON/OFF
date:           11/10/2016
author:         Kai Zhang		 
**************************************************************************/
void ADCStepAnalogSupplyConfig(struct adc_csreg *pReg, unsigned int stepSelect,
									unsigned int xppsw, unsigned int xnpsw, unsigned int yppsw)
{
	pReg->step[stepSelect].stepConfig &= ~ ADC_STEPCONFIG_XPPSW_SWC_CLEAR;
	pReg->step[stepSelect].stepConfig |= xppsw<<ADC_STEPCONFIG_XPPSW_SWC_SHIFT;

	pReg->step[stepSelect].stepConfig &= ~ ADC_STEPCONFIG_XNPSW_SWC_CLEAR;
	pReg->step[stepSelect].stepConfig |= xnpsw<<ADC_STEPCONFIG_XNPSW_SWC_SHIFT;

	pReg->step[stepSelect].stepConfig &= ~ ADC_STEPCONFIG_YPPSW_SWC_CLEAR;
	pReg->step[stepSelect].stepConfig |= yppsw<<ADC_STEPCONFIG_YPPSW_SWC_SHIFT;
}

/**************************************************************************
function name:  ADCStepAnalogGroundConfig
description:    This function is similar to ADCStepAnalogSupplyConfig,
                the difference is that this will configure the pull down
                voltage.

input:          devptr -------> base address of ADC register
                stepSelect----> which step register to use
                xppsw--> ON/OFF
                xnpsw------> ON/OFF
                yppsw--> ON/OFF
date:           11/10/2016
author:         Kai Zhang				 
**************************************************************************/
void ADCStepAnalogGroundConfig(struct adc_csreg *pReg, unsigned int stepSelect,
								 unsigned int xnnsw,unsigned int ypnsw, unsigned int ynnsw,
                                 unsigned int wpnsw)
{
	pReg->step[stepSelect].stepConfig &= ~ ADC_STEPCONFIG_XNNSW_SWC_CLEAR;
	pReg->step[stepSelect].stepConfig |=  xnnsw<<ADC_STEPCONFIG_XNNSW_SWC_SHIFT;

	pReg->step[stepSelect].stepConfig &= ~ ADC_STEPCONFIG_YPNSW_SWC_CLEAR;
	pReg->step[stepSelect].stepConfig |=  ypnsw<<ADC_STEPCONFIG_YPNSW_SWC_SHIFT;

	pReg->step[stepSelect].stepConfig &= ~ ADC_STEPCONFIG_YNNSW_SWC_CLEAR;
	pReg->step[stepSelect].stepConfig |=  ynnsw<<ADC_STEPCONFIG_YPNSW_SWC_SHIFT;

	pReg->step[stepSelect].stepConfig &= ~ ADC_STEPCONFIG_WPNSW_SWC_CLEAR;
	pReg->step[stepSelect].stepConfig |=  wpnsw<<ADC_STEPCONFIG_WPNSW_SWC_SHIFT;	
}

/**************************************************************************
function name:  ADCTSStepFIFOSelConfig
description:    Select which fifo to use (0 or 1)

input:          devptr -------> base address of ADC register
                stepSelect----> which step register to use
                fifo----------> FIFO_0 or FIFO_1

date:           11/10/2016
author:         Kai Zhang				 
**************************************************************************/
void ADCStepFIFOSelConfig(struct adc_csreg *pReg, unsigned int stepSelect,unsigned int fifoNO)
{
	pReg->step[stepSelect].stepConfig &= ~ ADC_FIFO_SELECTION_CLEAR;
	pReg->step[stepSelect].stepConfig |= fifoNO<<ADC_FIFO_SELECTION_SHIFT;
}

/**************************************************************************
function name:  ADCTSStepModeConfig
description:    Select which fifo to use (0 or 1)
input:          devptr -------> base address of ADC register
                stepSelect----> which step register to use
                mode----------> 00: SW enable, one shot (Hardware will disable 
                                    the step module after one capture)
                                01: SW enable, continuous
                                10: HW synchronized, one shot
                                11: HW synchronized, continuous
date:           11/10/2016
author:         Kai Zhang				 
**************************************************************************/
void ADCStepModeConfig(struct adc_csreg *pReg, unsigned int stepSelect,unsigned int mode)
{
	pReg->step[stepSelect].stepConfig &= ~ ADC_MODE_CLEAR;
	pReg->step[stepSelect].stepConfig |= mode<<ADC_MODE_SHIFT;
}

/**************************************************************************
function name:  ADCSetInterruptBit
description:    Enable interrupt, there're 10 interrupt bits can be enable.
input:          devptr -------> base address of ADC register
                irqEnable-----> which irq you want to enable
date:           11/10/2016
author:         Kai Zhang				 
**************************************************************************/
void ADCSetInterruptBit(struct adc_csreg *pReg, unsigned int irqEnable)
{
	pReg->irqEnableSet |=irqEnable;
}


/**************************************************************************
function name:  adcinit
description:    ADC module initial. It needs to:
                1. Configure all the clock domains for ADC
                2. Open the system interrupt and ADC interrupt
                3. Configure the ADC module 
input:          devptr -------> base address of ADC register
date:           11/10/2016
author:         Kai Zhang				 
**************************************************************************/

devcall	adcinit (struct	dentry *devptr)
{
	//kprintf("adc init start running\r\n");
	struct adc_csreg* pReg = (struct adc_csreg*)devptr->dvcsr; //get the register start address

	//create a semaphore, when interrupt finish reading, it will add sem by one, and read function
	//will wait this semaphore
	readSem = semcreate(0);										
	InitializeADCClock();
	InitialADCInterrupt(devptr);
	//kprintf("finish adc interrupt setting\r\n");
	ADCConfigure(pReg);
	//kprintf("finish adc configure setting\r\n");
	//TestRegAddress(pReg);
	return 1;
}
