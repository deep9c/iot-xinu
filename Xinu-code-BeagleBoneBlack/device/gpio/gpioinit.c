/* gpioinit.c - gpioinit */

#include <xinu.h>
#include <gpio.h>


/*------------------------------------------------------------------------
 *  gpioinit  -  Initialize gpio pns for a gpio device
 *------------------------------------------------------------------------
 */
struct gpioblk *gpio_blk_input;
struct gpioblk *gpio_blk_output;
struct gpiopinconfig gpioinitblk;
struct gpiopinconfig gpiotab[4];
devcall	gpioinit(struct dentry	*devptr)
{
	kprintf("gpio initialize\r\n");
	// need to be in ddl
	int32 isInterruptEnabled = 0;
	int32 _pintype; //gpio or adc; 0 is gpio 1 is adc
	int32 input_pinid; 
	int32 outputpins[1];
	uint32 num_pin_output; 
	int32 dir_in;
	int32 dir_out;
	uint32 num_pins;

	Beagle_GPIO_Status status_in;
	Beagle_GPIO_Status status_out;

	_pintype = 0;
	outputpins[0] = P9_15;
	num_pin_output = 1; 
	
	input_pinid =  P9_23;

	dir_in = kINPUT;
	dir_out = kOUTPUT;
	
	isInterruptEnabled = 1;
	
	
	m_controlModule = (unsigned long *)GPIO_CONTROL_REG;
	int i=0;
	for(i=0; i<4; i++){
		m_gpio[i] = (unsigned long *) GPIO_Base[i] ;
	}

	/*num_pins = 2;
	gpioinitblk._pinindex = 0;
	gpioinitblk.direction = kOUTPUT;
	gpioinitblk.isInterruptEnabled = 0;
	gpiotab[0] = gpioinitblk;

	gpioinitblk._pinindex = 7;
	gpioinitblk.direction = kINPUT;
	gpioinitblk.isInterruptEnabled = 1;
	gpiotab[1] = gpioinitblk;
	
	for(i=0; i<num_pins; i++) {
		if(gpiotab[i].direction == kINPUT){
			int32 input_pinid = getgpiopin(gpiotab[i]._pinindex);//7=P9_23
			status_in = configurePin(input_pinid,kINPUT);
			enablePinInterrupts( input_pinid, gpiotab[i].isInterruptEnabled );
		} else if(gpiotab[i].direction == kOUTPUT){
			int32 output_pinid = getgpiopin(gpiotab[i]._pinindex);//0=P9_15;
			status_out = configurePin(output_pinid,kOUTPUT);
		}
		
	}*/
	gpio_init_ddl ();

	// Configure input pins
	/*Beagle_GPIO_Status status_in = configurePin(P9_23,dir_in);
	kprintf("status_in is %d\n", status_in);
	 //For input switch
	enablePinInterrupts( input_pinid, isInterruptEnabled );	

	
	Beagle_GPIO_Status status_out;
	for(i=0; i<num_pin_output; i++){
		status_out = configurePin(outputpins[i],dir_out);
		//enablePinInterrupts( outputpins[i], 0 );
	}*/	

	//Populating the input gpio struct	
	//if(status_in == kSuccess){	
		char* buf = getmem(sizeof(struct gpioblk));	
		gpio_blk_input = (struct gpioblk *)buf;
		kprintf("setting GPIO base addr for INPUT %d\n",GPIO_Base[GPIO_Pin_Bank[input_pinid]]);
		gpio_blk_input->gpio_base_address=GPIO_Base[GPIO_Pin_Bank[input_pinid]];
		gpio_blk_input->initialized=1;
		gpio_blk_input->pin_num=input_pinid;
		gpio_blk_input->direction=dir_in;
		gpio_blk_input->is_interrupt_enabled=isInterruptEnabled;
	//}

	//Populating the output gpio struct linked list
	//if(status_out == kSuccess){		
		//char* buf = getmem(sizeof(struct gpioblk));	
		gpio_blk_output = (struct gpioblk *)buf;
		kprintf("setting GPIO base addr for OUTPUT %d\n",GPIO_Base[GPIO_Pin_Bank[input_pinid]]);
		gpio_blk_output->gpio_base_address=GPIO_Base[GPIO_Pin_Bank[input_pinid]];
		gpio_blk_output->initialized=1;
		gpio_blk_output->pin_num=outputpins[0];
		gpio_blk_output->direction=dir_out;
		gpio_blk_output->is_interrupt_enabled=isInterruptEnabled;
		struct gpioblk *gpio_blk_output_curr;	
		gpio_blk_output_curr = gpio_blk_output;
		for(i=1; i<num_pin_output; i++){
			struct gpioblk *gpio_blk_output_next;
			char* buf = getmem(sizeof(struct gpioblk));	
			gpio_blk_output_next = (struct gpioblk *)buf;
			gpio_blk_output_next->gpio_base_address=GPIO_Base[GPIO_Pin_Bank[input_pinid]];
			gpio_blk_output_next->initialized=1;
			gpio_blk_output_next->pin_num=outputpins[0];
			gpio_blk_output_next->direction=dir_out;
			gpio_blk_output_next->is_interrupt_enabled=isInterruptEnabled;
			gpio_blk_output_curr->next = gpio_blk_output_next;
			gpio_blk_output_next = gpio_blk_output_curr;			
		}
	//}

	InitialGPIOInterrupt(devptr);
	kprintf("GPIO init complete !!! \n");
	return OK;
}

int32 enablePinInterrupts( uint32 _pin, int32 _enable )
{
	unsigned long v = 0x1 << GPIO_Pin_Id[_pin];
	
	if ( _enable )
	{
		m_gpio[GPIO_Pin_Bank[_pin]][kSYSCONFIG/4] |= 0x01;
		m_gpio[GPIO_Pin_Bank[_pin]][kIRQSTATUS_SET_0/4] |= v;
		m_gpio[GPIO_Pin_Bank[_pin]][kIRQSTATUS_SET_1/4] |= v;
		m_gpio[GPIO_Pin_Bank[_pin]][kRISINGDETECT/4] |= v;	//rising detection
		m_gpio[GPIO_Pin_Bank[_pin]][kFALLINGDETECT/4] |= v;	//falling detection
	}
	else
	{
		m_gpio[GPIO_Pin_Bank[_pin]][kIRQSTATUS_CLR_0/4] |= v;
		m_gpio[GPIO_Pin_Bank[_pin]][kIRQSTATUS_CLR_1/4] |= v;
	}
	return kSuccess;
}


int32 configurePin( uint32 _pin, int32 _direction )
{
	/*if ( !m_active )
		return kFail;
	
	assert(GPIO_Pin_Bank[_pin]>=0);
	assert(GPIO_Pin_Id[_pin]>=0);
	*/
	// Set Pin as GPIO on the pad control
	kprintf("inside configurePin## pinid %d\n",_pin);

	m_controlModule[GPIO_Pad_Control[_pin]/4] |= 0x07;

	unsigned long v = 0x1 << GPIO_Pin_Id[_pin];

	
	if ( _direction == kINPUT)
	{
		m_gpio[GPIO_Pin_Bank[_pin]][kOE/4] |= v;

	}
	else
	{
		m_gpio[GPIO_Pin_Bank[_pin]][kOE/4] &= ~v;
	}
	
	// Disable Interrupts by default
	//m_gpio[GPIO_Pin_Bank[_pin]][kIRQSTATUS_CLR_0/4] |= v;
	//m_gpio[GPIO_Pin_Bank[_pin]][kIRQSTATUS_CLR_1/4] |= v;

	return kSuccess;
}

int32 getgpiopin (uint32 index){
	switch(index){
		case 0:
			return P9_15;
		case 7:
			return P9_23;
		default:
			return P9_23;

	}

}

void InitialGPIOInterrupt(struct dentry *devptr)
{
	struct	intc_csreg *csrptr = (struct intc_csreg *)0x48200000;   //get intr control register start address
	csrptr->threshold = 0X0FF;                                      //disable the priority threshold
	set_evec(devptr->dvirq, (uint32)devptr->dvintr);                //register the interrupt handler and priority
	kprintf("devptr->dvirq is %d\n",devptr->dvirq);
	csrptr->ilr[devptr->dvirq] &= ~(0x01);                          //set GPIO to IRQ interrupt
	csrptr->ilr[devptr->dvirq] |= (0x0B<<2);                        //priority is 11	 

}
