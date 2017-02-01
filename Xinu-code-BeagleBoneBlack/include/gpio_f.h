/* gpio_f.h */

devcall	gpioinit (struct dentry *devptr);
void gpiohandler(uint32 xnum);
/* GPIO Read Function */
void gpioread(uint32 xnum);
int32	gpiowrite(
	uint32 _pin,  // pin id 
	uint32 _value // 1 is on and 0 is off
	);
syscall	writesensor( char *inputdata);
uint32	sendedgedata(
	  char	*data	/* Data to be sent to edge */	  
	);
uint32	readedgedata(
	  //char	*data	/* Data to be sent to edge */	  
	);
int32 gpio_init_ddl();
int32 iot_write_pin(int32 d_index, int32 high_low);
int32 enablePinInterrupts( uint32 _pin, int32 _enable );
int32 configurePin( uint32 _pin, int32 _direction );



