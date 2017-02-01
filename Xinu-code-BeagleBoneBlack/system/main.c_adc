#include <xinu.h>


int main(int argc, const char *argv[])
{
	//recvclr();
	kprintf("Start main function\r\n");
	char data[4];
	unsigned int integer = 0;
	unsigned int decimal = 0;
	float voltage;
	while(1)
	{
		read(ADC,(char*)data,4);
		unsigned int vol = *(unsigned int*)data;
		voltage = (vol*1.8/4095)*1000;

		integer = (unsigned int)voltage;
		decimal = (unsigned int)((voltage - integer)*10);
		kprintf("voltage = %d.%d  mv\r\n",integer,decimal);
		sleep(1);
	}
}
