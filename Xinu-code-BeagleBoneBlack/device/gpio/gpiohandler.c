/* ttyhandler.c - ttyhandler */

#include <xinu.h>
#include <gpio.h>
/*------------------------------------------------------------------------
 *  ttyhandler  -  Handle an interrupt for a tty (serial) device
 *------------------------------------------------------------------------
 */
void gpiohandler(uint32 xnum) 
{
	struct gpioblk *gpio;
	gpio = gpio_blk_input;

	uint32 _pin = gpio->pin_num;
	unsigned long gpio_base = gpio->gpio_base_address;
	//unsigned long* status0Reg = m_gpio[GPIO_Pin_Bank[_pin]][kIRQSTATUS_0/4] ;
	//unsigned long* status1Reg = m_gpio[GPIO_Pin_Bank[_pin]][kIRQSTATUS_1/4] ;
	kprintf("in Handler pin addr %d\n",gpio->pin_num);
	unsigned long* status0Reg = (unsigned long *)(gpio_base+kIRQSTATUS_0);	//IRQ status 0
	unsigned long* status1Reg = (unsigned long *)(gpio_base+kIRQSTATUS_1);	//IRQ status 1

	unsigned long status0 = *status0Reg;
	unsigned long status1 = *status1Reg;
	kprintf("status0 = %x, status1 = %x\n", status0, status1);
	*status0Reg = status0;		//clear 
	*status1Reg = status1;		//clear
	

	unsigned long* gpioDataIn = (unsigned long*)(gpio_base + kDATAIN);
	kprintf("gpio input data = %x\n",*gpioDataIn);

	gpioread(_pin);

}
