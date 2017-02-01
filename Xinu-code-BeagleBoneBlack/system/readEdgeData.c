/* sendEdgeData.c - sendEdgeData */

#include <xinu.h>
#include <string.h>
#include <gpio.h>
int producer_id;
uid32 udpslotClient;
uint32	ipaddr;
char	str[128];
int32	inlen,outlen;			/* Length of data received & data sent	*/

enum {NOT_FOUND=0,FOUND};
static char *ptr;
const char *del;
char *mystrtok(char* string,const char *delim)
{
    int j,flag=NOT_FOUND;
    char *p;
    if(string != NULL)
    {
        ptr=string;
        p=string;
    }
    else
    {
        if(*ptr == '\0')
            return NULL;
 
        p=ptr;
    }
 
    while(*ptr != '\0')
    {
        del=delim;
        while(*del != '\0')
        {
            if(*ptr == *del)
            {
                if(ptr == p)
                {
                    p++;
                    ptr++;
                }
                else
                {
                    *ptr='\0';
                    ptr++;
 
                    return p;
                }
            }
            else
            {
                del++;
            }
        }
        ptr++;
    }
    return p;
}

/*------------------------------------------------------------------------
 *  readEdgeData  -  Read data from edge
 *------------------------------------------------------------------------
 */
uint32	readedgedata(
	  char	*data	/* Data to be sent to edge */	  
	)
{
	kprintf("\n In receive edge data... \n");
	//if(data==#tempsensor){
		/*read(ADC,(char*)data,4);
		unsigned int vol = *(unsigned int*)data;
		unsigned int voltage = (vol*1.8/4095)*1000;

		unsigned int integer = (unsigned int)voltage;
		unsigned int decimal = (unsigned int)((voltage - integer)*10);
		kprintf("voltage = %d.%d  mv\r\n",integer,decimal);
		sendedgedata(decimal);	*/	
	//}
	//else if(data==#ledon){
		
	//}

   // 5 is Temp sensor
   // 7 is button
   // 0 is led

    int i;
    char *p_str,*token;
    char delim[10] = {'#'};
    char *dev_index;
    char *edgedata;
  
 
    for (i = 1; ; i++, data = NULL)
    {
        token = mystrtok(data,delim);
        if (token == NULL)
            break;
	if(i==3){
		dev_index = token;
	} else if (i==4){
		edgedata = token;
		break;
	}
        kprintf("\n%d: %s",i,token);
    }
	
	int32 d_index = atoi(dev_index);
	int32 high_low = atoi(edgedata);
	kprintf("dev_indexval %d\n",d_index);

	iot_write_pin(d_index,high_low);
	
	/*switch(d_index){ 
		case 0:
			gpiowrite(0,high_low);
			break;
		case 1:
			gpiowrite(1,high_low);
			break;
		case 5:   
			gpioadcwrite(5, high_low);     
			break;
		default:
			break;
	}*/
	/*if(d_index==5){
	
		//sleep(1);
		return;	
	} else if(d_index==0){
		kprintf("edgedata is led %d\n",atoi(edgedata));
		gpiowrite(P9_15,atoi(edgedata));
	}*/


	return OK;
}

int32 gpioadcwrite(uint32 _pin, uint32 _data){
	  char adcdata[4];
	read(ADC,(char*)adcdata,4);
	unsigned int vol = *(unsigned int*)adcdata;
	unsigned int voltage = (vol*1.8/4095)*1000;

	unsigned int integer = (unsigned int)voltage;
	unsigned int decimal = (unsigned int)((voltage - integer)*10);
	kprintf("voltage = %d.%d  mv\r\n",integer,decimal);
	char msg[50];
	strcpy(msg,"0#5#1#");
	
	sendedgedata(msg);

}
