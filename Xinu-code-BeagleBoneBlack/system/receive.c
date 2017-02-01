/* receive.c - receive */

#include <xinu.h>


/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
msg_buf msg_bufs[MAX_BUF_NUM];
/*---------------------------------------------------------
 *Utility method to push a message at the end of the buffer 
 *---------------------------------------------------------
 */
int bufPushBack(msg_buf *buf, const umsg32 *item)
{
    if(buf->count == MAX_BUF_SIZE){
        // handle error
	kprintf("The message buffer is already full and you are trying to push the message %d \n", (*item));
	return SYSERR;
    }
    // add the item into the buffer
    buf->msgs[buf->end]= (*item);
    buf->end++;
    kprintf("message has been pushed into the buffer %d \n", (*item));
    if(buf->end == MAX_BUF_SIZE)
        buf->end = 0;
    buf->count++;
    return 1;	
}
/*----------------------------------------------------------
 *Utility method to pop an item from the front of the buffer
 *----------------------------------------------------------
 */

int bufPopFront(msg_buf *buf, umsg32 *item)
{
    if(buf->count == 0){
        // handle error
	kprintf("You are trying to pop from an empty buffer \n");
	return SYSERR;
    }
    (*item) = buf->msgs[buf->start];
    buf->start++;
    if(buf->start == MAX_BUF_SIZE)
        buf->start = 0;
    buf->count--;
    return 1;
}


umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	msg = prptr->prmsg;		/* Retrieve message		*/
	prptr->prhasmsg = FALSE;	/* Reset message flag		*/
	restore(mask);
	return msg;
}
/*-----------------------------------------------------------
 *  receive  -  wait for a particular message from the buffer
 *-----------------------------------------------------------
 */

umsg32  receiveMsg(void){
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prmsgcount == 0) {
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	bufPopFront((msg_bufs+prptr->prmsgbuf), &msg);
					/* Retrieve message from buffer */
	kprintf("message: %d: has been received\n", msg);	
	prptr->prmsgcount--;
	restore(mask);
	return msg;
}
/*-------------------------------------------------------------------
 *  receive  -  wait for a certain number of messages from the buffer
 *-------------------------------------------------------------------
 */

syscall receiveMsgs(umsg32* msgs, uint32 msg_count){
	int index =0;			/* Index of message being read  */
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/	
	
	mask = disable();
	prptr = &proctab[currpid];
	while (prptr->prmsgcount < msg_count) {
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	if(msgs==NULL){
		return SYSERR;	
	}
	for(index=0; index < msg_count; index ++){
		/* Pops the messages one by one from the buffer */
		bufPopFront((msg_bufs+prptr->prmsgbuf), (msgs+index));
		kprintf("message number %d : %d: has been received\n", index, *(msgs+index));
		prptr->prmsgcount--;
	}
	restore(mask);
	return OK;
}

