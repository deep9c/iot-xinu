/* writesensor.c - write */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  write  -  Write one or more bytes to a device
 *------------------------------------------------------------------------
 */
syscall	writesensor(
	  char		*inputdata	/* Input data from ethernet	*/
	)
{
	//parse inputdata
	//call either gpiowrite or adcwrite
	return OK;
}
