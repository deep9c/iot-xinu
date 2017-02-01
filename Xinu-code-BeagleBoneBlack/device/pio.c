#include <xinu.h>
#include <gpio.h>
#include <gpio_f.h>
#include <string.h>

int32 iot_write_pin(int32 d_index, int32 data){
	switch(d_index){ 
		case 0:
		gpiowrite(0,data);
		case 1:
		gpiowrite(1,data);
		case 3:
		gpioadcwrite(3,data);
		default:
			break;
	}
	return 1;

}


	
int32 gpio_init_ddl (){
int32 num_pins=4;

	struct gpiopinconfig gpioinitblk;
	struct gpiopinconfig gpiotab[num_pins];
	Beagle_GPIO_Status status;
	Beagle_GPIO_Status status_out;
	int i;
	gpioinitblk._pinindex =0;
	gpioinitblk.direction=kOUTPUT;
	gpioinitblk.isInterruptEnabled =0;
	gpiotab[0] = gpioinitblk;
	
	gpioinitblk._pinindex =1;
	gpioinitblk.direction=kOUTPUT;
	gpioinitblk.isInterruptEnabled =0;
	gpiotab[1] = gpioinitblk;
	
	gpioinitblk._pinindex =3;
	gpioinitblk.direction=kOUTPUT;
	gpioinitblk.isInterruptEnabled =0;
	gpiotab[2] = gpioinitblk;
	
	gpioinitblk._pinindex =7;
	gpioinitblk.direction =kINPUT;
	gpioinitblk.isInterruptEnabled =1;
	gpiotab[3] = gpioinitblk;
	
	
	for(i=0; i<num_pins; i++) {
		if(gpiotab[i].direction == kINPUT){
			int32 input_pinid = getgpiopin(gpiotab[i]._pinindex);//7=P9_23
			status = configurePin(input_pinid,kINPUT);
			enablePinInterrupts( input_pinid, gpiotab[i].isInterruptEnabled );
		} else if(gpiotab[i].direction == kOUTPUT){
			int32 output_pinid = getgpiopin(gpiotab[i]._pinindex);//0=P9_15;
			status = configurePin(output_pinid,kOUTPUT);
		}
		
	}
	return kSuccess;
}
