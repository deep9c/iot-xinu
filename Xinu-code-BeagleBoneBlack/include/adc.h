#ifndef ADC_H
#define ADC_H

struct adc_csreg
{
	volatile uint32 revison;
	volatile uint32 reserve1[3];
	volatile uint32 sysconfig;
	volatile uint32 reserve2[4];
	volatile uint32 irqStatusRaw;
	volatile uint32 irqStatus;
	volatile uint32 irqEnableSet;	
	volatile uint32 irqEnableClr;
	volatile uint32 irqWakeup;
	volatile uint32 dmaEnableSet;
	volatile uint32 dmaEnableClr;
	volatile uint32 ctrl;
	volatile uint32 adcStat;
	volatile uint32 adcRange;
	volatile uint32 adcClkDiv;
	volatile uint32 adcMisc;
	volatile uint32 stepEnable;
	volatile uint32 idleConfig;
	volatile uint32 tsChargeStepconfig;
	volatile uint32 tsChargeDelay;
	struct 
	{
		volatile uint32 stepConfig;
		volatile uint32 stepDelay;
	}step[16];
	struct 
	{
		volatile uint32 fifoCount;
		volatile uint32 fifoThreshold;
		volatile uint32 dmaReq;
	}fifoInfo[2];
	volatile uint32 reserve3;
	volatile uint32 fifoData0;
	volatile uint32 reserve4[63];
	volatile uint32 fifoData1;
};

devcall	adcread(struct dentry* devptr, char* buff, int32 count);
devcall	adcinit (struct	dentry *devptr);
void adchandler(uint32 xnum);

#define ADC_ADDR					(0x44E0D000)


#define ADC_STEPCONFIG_DIFF_CNTRL   (0x01<<25)

//differential mode or single ended mode
#define ADC_SINGLE_ENDED_OPER_MODE  (0)
#define ADC_DIFFERENTIAL_OPER_MODE	(1)

//ADC channel
#define ADC_CHANNEL1            	(0)			//PIN AN0
#define ADC_CHANNEL2            	(1) 		//PIN AN1
#define ADC_CHANNEL3            	(2)   		//PIN AN2
#define ADC_CHANNEL4            	(3)   		//PIN AN3
#define ADC_CHANNEL5            	(4)   		//PIN AN4
#define ADC_CHANNEL6            	(5) 		//PIN AN5
#define ADC_CHANNEL7            	(6) 		//PIN AN6
#define ADC_CHANNEL8            	(7)			//PIN AN7


//SEL_RFM_SWC field in step config register
#define ADC_NEGATIVE_REF_VSSA		(0)			//SEL_RFM pins SW configuration (23:24)
#define ADC_NEGATIVE_REF_XNUR		(1)
#define ADC_NEGATIVE_REF_YNLR		(2)
#define ADC_NEGATIVE_REF_VREFN		(3)

#define ADC_POSITIVE_REF_VDDA		(0)			//SEL_RFP pins SW configuration (12:14)
#define ADC_POSITIVE_REF_XPUL		(1)
#define ADC_POSITIVE_REF_YPLL		(2)
#define ADC_POSITIVE_REF_VREFP		(3)

#define SEL_RFM_SWC_CLEAR			(0X03<<23)	//& ~SEL_RFM_SWC_CLEAR will clear the SEL_RFM_SWC field
#define SEL_RFM_SWC_SHIFT			(23)		//SEL_RFM_SWC is at 23:24 bit

//SEL_INP_SWC field in step config register
#define SEL_INP_SWC_CLEAR			(0XF<<19)	//& ~SEL_INP_SWC_CLEAR will clear the SEL_INP_SWC field
#define SEL_INP_SWC_SHIFT			(19)		//SEL_INP_SWC is at 19:22 bit

//SEL_INM_SWC field in step config register
#define SEL_INM_SWC_CLEAR			(0X0F<<15)	//& ~SEL_INM_SWC_CLEAR will clear the SEL_INM_SWC field
#define SEL_INM_SWC_SHIFT			(15)		//SEL_INM_SWC is at 15:18 bit

//SEL_RFP_SWC field in step config register
#define SEL_RFP_SWC_CLEAR			(0x07<<12)	//& ~SEL_RFP_SWC_CLEAR will clear the SEL_INM_SWC field
#define SEL_RFP_SWC_SHIFT			(12)		//SEL_RFP_SWC is at 15:18 bit


#define ADC_STEPCONFIG_WPNSW_SWC_CLEAR   		(0x01<<11)
#define ADC_STEPCONFIG_WPNSW_SWC_SHIFT  		(0x0000000Bu)

#define ADC_STEPCONFIG_XNNSW_SWC_CLEAR   		(0x01<<6)
#define ADC_STEPCONFIG_XNNSW_SWC_SHIFT  		(0x00000006u)

#define ADC_STEPCONFIG_XNPSW_SWC_CLEAR   		(0x01<<9)
#define ADC_STEPCONFIG_XNPSW_SWC_SHIFT  		(0x00000009u)

#define ADC_STEPCONFIG_XPPSW_SWC_CLEAR   		(0x01<<5)
#define ADC_STEPCONFIG_XPPSW_SWC_SHIFT  		(0x00000005u)

#define ADC_STEPCONFIG_YNNSW_SWC_CLEAR   		(0x01<<8)
#define ADC_STEPCONFIG_YNNSW_SWC_SHIFT  		(0x00000008u)

#define ADC_STEPCONFIG_YPNSW_SWC_CLEAR   		(0x01<<10)
#define ADC_STEPCONFIG_YPNSW_SWC_SHIFT  		(0x0000000Au)

#define ADC_STEPCONFIG_YPPSW_SWC_CLEAR   		(0x01<<7)
#define ADC_STEPCONFIG_YPPSW_SWC_SHIFT  		(0x00000007u)



// FIFO field
#define ADC_FIFO_0                           	(0)
#define ADC_FIFO_1                           	(1)
#define ADC_FIFO_SELECTION_CLEAR			 	(0X01<<26)
#define ADC_FIFO_SELECTION_SHIFT			 	(26)

//MODE field
#define ADC_MODE_CLEAR						 	(0X03)
#define ADC_MODE_SW_ONE_SHOT					(0X00)
#define ADC_MODE_SW_CONTINUOUS					(0x01)
#define ADC_MODE_HW_ONE_SHOT					(0X10)
#define ADC_MODE_HW_CONTINUOUS					(0x11)
#define ADC_MODE_SHIFT							(0x00)

//ADC work mode
#define ADC_FOUR_WIRE_MODE                   	(1)		//for touch screen
#define ADC_FIVE_WIRE_MODE                   	(2)		//for touch screen
#define ADC_GENERAL_PURPOSE_MODE             	(0)		//for general purpose
#define ADC_WORK_MODE_CLEAR						(0x03<<5)
#define ADC_WORK_MODE_SHIFT						(5)		

//ADC interrupt bit set
#define ADC_END_OF_SEQUENCE_INTR				(0x01<<1)	//end of sequence interrupt


void ADCStepModeConfig(struct adc_csreg *pReg, unsigned int stepSelect,unsigned int mode);
void ADCStepFIFOSelConfig(struct adc_csreg *pReg, unsigned int stepSelect,unsigned int fifoNO);
void ADCStepAnalogGroundConfig(struct adc_csreg *pReg, unsigned int stepSelect,
								 unsigned int xnnsw,unsigned int ypnsw, unsigned int ynnsw,
                                 unsigned int wpnsw);
void ADCStepAnalogSupplyConfig(struct adc_csreg *pReg, unsigned int stepSelect,
									unsigned int xppsw, unsigned int xnpsw, unsigned int yppsw);
void ADCStepConfig(struct adc_csreg *pReg, unsigned int stepSelect, 
					unsigned int positiveChannel,unsigned int positiveRef,
					unsigned int negativeChannel,unsigned int negativeRef);
void ADCStepOperationModeControl(struct adc_csreg *pReg, unsigned int stepSelect, unsigned int mode);
void StepConfig(struct adc_csreg *pReg);
void ADCConfigure(struct adc_csreg *pReg);
void ADCEnableModual(struct adc_csreg *pReg);
void ADCDisableModual(struct adc_csreg *pReg);
void ADCClearInterruptStatus(struct adc_csreg *pReg);
void ADCClearInterruptStatus(struct adc_csreg *pReg);
void ADCStepEnable(struct adc_csreg *pReg, unsigned int selectStep);
void ADCWorkModeConfigue(struct adc_csreg *pReg,unsigned int workMode);
void ADCStepConfigWriteProtect(struct adc_csreg *pReg, unsigned int isWritable);
void ADCTSTransistorConfig(struct adc_csreg *pReg);
void ADCConfigureAFEClock(struct adc_csreg *pReg, unsigned int freq);
void ADCSetInterruptBit(struct adc_csreg *pReg, unsigned int irqEnable);
void InitialADCInterrupt(struct	dentry *devptr);

void InitialL4Clock();
void InitializeADCClock();

#endif