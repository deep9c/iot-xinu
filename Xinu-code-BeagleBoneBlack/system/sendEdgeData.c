/* sendEdgeData.c - sendEdgeData */

#include <xinu.h>
#include <string.h>
int producer_id;
uid32 udpslotClient;
uint32	ipaddr;
char	str[128];
int32	inlen,outlen;			/* Length of data received & data sent	*/

/*------------------------------------------------------------------------
 *  sendEdgeData  -  Send data to edge
 *------------------------------------------------------------------------
 */
uint32	sendedgedata(
	  char	*data	/* Data to be sent to edge */	  
	)
{
	kprintf("\n In sendedgedata... \n");
	kprintf("In sendedgedata %s\n",data);	

	//net_init();
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
		kprintf("invalid ip ###### \n");
				return SYSERR;

            }

        }

        if (n >= 256) {

            //INVALID IP;
		kprintf("invalid ip1 ###### \n");
			return SYSERR;

        }

        remIP_int *= 256;

        remIP_int += n;

    }
	/*---- IP conversion to int ends ----- */
	kprintf("int IP = %u \n", remIP_int);
	udpslotClient = udp_register (remIP_int,7790,7770);	//server IP, server/remote port, local port	192.168.0.7 = 3232235527

	//udpslotServer = udp_register (0,8778,7778);		// 0, remote port, local port

	kprintf("\n Registered.check... %u  \n", udpslotClient);

	if (udpslotClient == SYSERR) {

		kprintf("Cannot register with UDP\n");

		return SYSERR;

	}
	//char*  msg;

	char*  replymsg;

	//char	replymsg[30];

	replymsg = "dummyreply";

	//msg = "0#6#3#";//data;
	kprintf("\n Msg sent.before... %s\n",data);
	udp_send(udpslotClient,data,strlen(data));
	

	kprintf("\n Msg sent.... \n");

	udp_release(udpslotClient);
	kprintf("\n udpslotClient released.... \n");
	
	
	return OK;
}
