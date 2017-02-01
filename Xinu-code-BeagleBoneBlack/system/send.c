/* send.c - send */

#include <xinu.h>

extern msg_buf msg_bufs[MAX_BUF_NUM];
/*------------------------------------------------------------------------
 *  send  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	send(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (prptr->prhasmsg) {
		restore(mask);
		return SYSERR;
	}
	prptr->prmsg = msg;		/* Deliver message		*/
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/

	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}
/*-----------------------------------------------------------------------------
 *  send  -  Pass a message to a process' buffer and start recipient if waiting
 *-----------------------------------------------------------------------------
 */

syscall sendMsg (pid32 pid, umsg32 msg){
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	int temp=0;
	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}
	prptr = &proctab[pid];
	if (prptr->prmsgcount>=MAX_BUF_SIZE) {
		restore(mask);
		return SYSERR;
	}
	if(prptr->prmsgcount==0){
	// fetch a buffer for the new message
	for(temp=0; temp< MAX_BUF_NUM; temp++){
		if(msg_bufs[temp].count==0){
			prptr->prmsgbuf = temp;
			break;						
		}
	}
	}
	// push the message into the buffer
	bufPushBack(&(msg_bufs[prptr->prmsgbuf]), &msg);
	prptr->prmsgcount++;
	/* If recipient waiting or in timed-wait make it ready */
	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}

/*--------------------------------------------------------------------------------------------
 *  sendMsgs  -  Pass a given a number of messages to a process and start recipient if waiting
 *--------------------------------------------------------------------------------------------
 */
uint32 sendMsgs (pid32 pid, umsg32* msgs, uint32 msg_count){
	int i;
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;		
	}
	prptr = &proctab[pid];	
	if((prptr->prmsgcount+msg_count)>MAX_BUF_SIZE){
		return SYSERR;	   	/* Return error if there is chance of buffer overflow */
	}	
	for(i=0;i<msg_count;i++){
		sendMsg(pid,msgs[i]);	/* Send the messages one by one */
	}
	kprintf("%d messages have been successfully sent", msg_count);
	restore(mask);
	return msg_count;
}
/*------------------------------------------------------------------------
 *  send  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
uint32 sendnMsg (uint32 pid_count, pid32* pids, umsg32 msg){
	int i;
	intmask	mask;			/* Saved interrupt mask		*/
	for(i=0; i < pid_count; i++){
		if(sendMsg(pids[i],msg)==SYSERR){
			restore(mask);			
			return SYSERR;		
		}	
	}
	kprintf("msg: %d: has been sent successfully to %d processes", msg, pid_count);
	restore(mask);
	return pid_count;
}

