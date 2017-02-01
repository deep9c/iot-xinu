/*  main.c  - main */

#include <xinu.h>
#include <string.h>
int producer_id;
uid32 udpslotClient;
uint32	ipaddr;
char	str[128];
int32	inlen,outlen;			/* Length of data received & data sent	*/



process producer(void){
	
	kprintf("\n Process started.... \n");	
	
	net_init();
	kprintf("\n net_init called \n");
	
	ipaddr = getlocalip();
	kprintf("\n getlocalip called \n");
	if ((int32)ipaddr == SYSERR) {
		kprintf("Cannot obtain an IP address\n");
	} else {
		kprintf("\n obtained IP \n");
		/* Print the IP in dotted decimal and hex */
		ipaddr = NetData.ipucast;
		sprintf(str, "%d.%d.%d.%d",
			(ipaddr>>24)&0xff, (ipaddr>>16)&0xff,
			(ipaddr>>8)&0xff,        ipaddr&0xff);
	
		kprintf("Obtained IP address  %s   (0x%08x)\n", str,
								ipaddr);
	}
	
	
	char * ip = "192.168.0.15";		//IP of Edge socket server
	
	/*---- IP conversion to int ends ----- */
	
	/* The return value. */ 
    uint32 remIP_int = 0;
    /* The count of the number of bytes processed. */
    int i;
    /* A pointer to the next digit to process. */
    char * start;

    start = ip;
    for (i = 0; i < 4; i++) {
        /* The digit being processed. */
        char c;
        /* The value of this byte. */
        int n = 0;
        while (1) {
            c = * start;
            start++;
            if (c >= '0' && c <= '9') {
                n *= 10;
                n += c - '0';
            }
            /* We insist on stopping at "." if we are still parsing
               the first, second, or third numbers. If we have reached
               the end of the numbers, we will allow any character. */
            else if ((i < 3 && c == '.') || i == 3) {
                break;
            }
            else {
                //INVALID IP;
				return SYSERR;
            }
        }
        if (n >= 256) {
            //INVALID IP;
			return SYSERR;
        }
        remIP_int *= 256;
        remIP_int += n;
    }
    
	/*---- IP conversion to int ends ----- */
	
	udpslotClient = udp_register (remIP_int,7790,7770);	//server IP, server/remote port, local port	192.168.0.7 = 3232235527
	//udpslotServer = udp_register (0,8778,7778);		// 0, remote port, local port
	kprintf("\n Registered.... %u  \n", udpslotClient);
	if (udpslotClient == SYSERR) {
		kprintf("Cannot register with UDP\n");
		return SYSERR;
	}
	
	
	char*  msg;
	char*  replymsg;
	//char	replymsg[30];
	replymsg = "dummyreply";
	msg = "boot#0#";
	udp_send(udpslotClient,msg,strlen(msg));
	kprintf("\n Msg sent.... \n");
	
	inlen = udp_recv(udpslotClient, replymsg,29, 7000);
	kprintf("udp_recv called");
		if (inlen == TIMEOUT) {
			kprintf("\n Response from server Timed Out: 7000ms.... \n");
		} else if (inlen == SYSERR) {
			kprintf("\n Response from server SYSERR.... \n");
			return SYSERR;
		}
	kprintf("\n Reply from server:: %s .... \n",replymsg);
	
	udp_release(udpslotClient);
	kprintf("\n udpslotClient released.... \n");
	
	
	
	
}




process	main(void)
{
	recvclr();
	kprintf("\n Main Started .... \n");
	
	
	producer_id  = create(producer, 4096, 50, "producer",0);
	
	resched_cntl(DEFER_START);	
	resume(producer_id);
	resched_cntl(DEFER_STOP);
	
	return OK;
}


