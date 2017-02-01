/* gpiowrite.c - gpiowrite */

#include <xinu.h>
#include <gpio.h>

/*------------------------------------------------------------------------
 *  gpiowrite  -  Write character(s) to a gpio device (interrupts disabled)
 *------------------------------------------------------------------------
 */
int32	gpiowrite(
	uint32 _pinid,  // pin id 
	uint32 _value // 1 is on and 0 is off
	)
{
	uint32 _pin;
	kprintf("in gpiowrite\n");
	switch(_pinid){
		case 0: 
			_pin=P9_15;
			break;
		case 1:
			_pin=P9_16;
			break;
		default:
			break;
	}

	unsigned long v = (_value & 0x01) << GPIO_Pin_Id[_pin];
	unsigned long mask = 0x1 << GPIO_Pin_Id[_pin];

	// Remove bit
	m_gpio[GPIO_Pin_Bank[_pin]][kDATAOUT/4] &= ~mask;
	// Assign new bit value
	m_gpio[GPIO_Pin_Bank[_pin]][kDATAOUT/4] |= v;
	
	return kSuccess;
	
	return OK;
}
