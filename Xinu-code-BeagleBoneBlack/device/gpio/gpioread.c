/* gpioread.c - gpioread */

#include <xinu.h>
#include <gpio.h>
#include <string.h>

/*------------------------------------------------------------------------
 *  gpioread  -  Read character(s) from a gpio device (interrupts enabled)
 *------------------------------------------------------------------------
 */
void gpioread(
	  uint32 _pin 	/* Entry in device switch table	*/
	)
{ 	
	char message[50];
	char message1[50];
	
	unsigned long bit = GPIO_Pin_Id[_pin];
	uint32 data = (m_gpio[GPIO_Pin_Bank[_pin]][kDATAIN/4] & (0x1 << bit)) >> bit;
	kprintf("In gpioread data %d\n\n",data);
	//Call for sending the data to Edge...................
	
	strcpy(message,"0#7#");
	//strcpy(message,itoa(data));
	kprintf("Message in read1 ##### %s\n", message);
	sprintf(message1, "%d", data);
	kprintf("Message in read2 ##### %s\n", message1);
	strncat(message,message1,1);
	kprintf("Message in read3 ##### %s\n", message);
	strncat(message,"#",1);
	kprintf("Message in read4 ##### %s\n", message);
	sendedgedata(message);
	return OK;
}
