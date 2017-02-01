/*event.c- handling events*/
#include <xinu.h>
/*
 * this table stores the information related to all the topics 
 */ 
subscriber subs[MAX_NUMBER_OF_TOPICS][MAX_SUBSCRIBERS]; 
// broker queue head
bufCli head;
bufCli * const headBuf = &head;
// pid for the broker process
pid32 brokerPid = -1;

/*
 * removes the processes from the broker's queue
 */
void removeFromBufQueue(){
	intmask	mask;			/* Saved interrupt mask		*/
	mask = disable();
	bufCli * tp = headBuf->next;
	tp->handler(tp->topicId, tp->data, tp->size);
	//kprintf("handler invoked for data %d", tp->data);
	headBuf->next= tp->next;
	headBuf->data--;
	//free the memory allocated in the heap
	freemem((char*)tp->data, sizeof(char)*(tp->size)); 	
	freemem((char *)tp, sizeof(bufCli));
	restore(mask);
}

/*
 * adds the processes to the broker's queue
 */
void appendToBufQueue(subscriber *sbcr, void * data,uint32 size){
	intmask	mask;			/* Saved interrupt mask		*/
	mask = disable();
	int i;	
	bufCli * tp = headBuf;
	while(tp->next!=NULL){
    	  tp= tp->next;
	}
	tp->next = (bufCli *)getmem(sizeof(bufCli));
	tp= tp->next;
	tp->handler = sbcr->handler;
        tp->topicId = sbcr->topicId;
	tp->size = size;
	tp->data = (char*)getmem(sizeof(char)*size);
	// allocate space in the heap for storing 
	//the data and pass it to broker queue	
	for(i=0; i< size;i++){
	  *((char*)tp->data+i)= *((char *)data+i);	
	}	
	tp->next = NULL;
	headBuf->data++;
	restore(mask);
}

/*
 * this contains the code for the broker process once it's created
 */
process broker(void){ 
	while (headBuf->data>0){
	  removeFromBufQueue();
	}
	return OK;
}

/*----------------------------------------------------------
 *  subscribe  - subscribes to a specific topic given its id
 *----------------------------------------------------------
 */

syscall subscribe(topic16 topic, void (*hndlr)(topic16, void*, uint32)){
	intmask	mask;			/* Saved interrupt mask		*/
	mask = disable();
	int i,flag=-1;
	topic16 lower = topic & 0x00FF; /* lower 8 bits which contain the actual topic id */
	if ((lower > MAX_NUMBER_OF_TOPICS)||(lower <0)) {
		restore(mask);
		return SYSERR;
	}
	// iterate through the topic table and find a place for the new subscriber
	for(i=0; i<MAX_SUBSCRIBERS; i++){
	  if(subs[lower][i].valid==0 &&(flag == -1)){   			
		flag = i;		
	  }
	  else if((subs[lower][i].valid==1)&&(subs[lower][i].pid==currpid)){
	  	restore(mask);
		// return error if the process has already subscribed there in the topic
		kprintf("the process: %d has already subscribed to the topic %d\n", currpid, topic);		
		return SYSERR;
	  }	
	}		
	  if(flag == -1){
		// return error if the topic array is filled up
		kprintf("topic array filled up %d for topic:\n", topic);
	     	restore(mask);
		return SYSERR;	
	  }
	// fill out the entries for the subscriber
	subs[lower][flag].handler= hndlr;
	subs[lower][flag].pid= currpid;
	subs[lower][flag].topicId= topic;
	subs[lower][flag].valid=1;
	kprintf("process: %d is subscribing to topic: %d\n",currpid,lower);
	restore(mask);		/* Restore interrupts */
	return OK;
}

/*------------------------------------------------------------------
 *  unsubscribe  -  unsubscribe from a particular topic given its id
 *------------------------------------------------------------------
 */

syscall unsubscribe(topic16 topic){
	intmask	mask;			/* Saved interrupt mask		*/
	mask = disable();
	int i=0;
	topic16 lower = topic & 0x00FF;
	if ((lower > MAX_NUMBER_OF_TOPICS)||(lower <0)) {
		restore(mask);
		return SYSERR;
	}
	for(i=0; i< MAX_SUBSCRIBERS; i++){
		if((subs[lower][i].pid==currpid)&&(subs[lower][i].valid==1)){
		// its a lazy deletion from the subscribers' queue, setting the valid to 0	
		  subs[lower][i].valid=0;
		  kprintf("process: %d is unsubscribing from topic: %d\n",currpid,lower);	
		  restore(mask);
		  return OK;			
		}	
	}
	kprintf(" the process: %d could not unsubscribe from the topic: %d\n",currpid, lower);
	restore(mask);	
	return SYSERR;	
}

/*----------------------------------------------------------
 *  publish  -  publishes an event to all of its subscribers
 *----------------------------------------------------------
 */
syscall publish(topic16 topic, void * data, uint32 size){
	intmask	mask;			/* Saved interrupt mask		*/
	mask = disable();
	int i;
	topic16 lower = topic & 0x00FF;
	topic16 upper = (topic & 0xFF00)>>8;	
	kprintf("process : %d is publishing data of size %u\n", currpid, size);	        
	// iterate through all the subscribers and put them in broker queue
	for(i=0; i<MAX_SUBSCRIBERS;i++){
		if((subs[lower][i].valid==1)&&(subs[lower][i].topicId==topic||upper==0)){
		  appendToBufQueue(&(subs[lower][i]), data, size);		
		}	
	}
	if(isbadpid(brokerPid)|| proctab[brokerPid].prstate != PR_READY){
	// create and resume the broker process
	  brokerPid=create(broker, 4096, 50, "broker", 0);
	  resume(brokerPid);				
	}
	restore(mask);
	return OK;	
}

