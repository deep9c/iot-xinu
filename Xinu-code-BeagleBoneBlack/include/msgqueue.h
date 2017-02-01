/* queue.h - firstid, firstkey, isempty, lastkey, nonempty		*/

/* Queue structure declarations, constants, and inline functions	*/

/* Default # of queue entries: 1 per process plus 2 for ready list plus	*/
/*			2 for sleep list plus 2 per semaphore		*/

#define	MAXMSG 10

struct	qmsgentry	{	/* The message entry in the message queue */
	pid32	pid;		/* Process id for the target process	*/
	umsg32	msg;		/* Index of next process or tail	*/
};

extern	struct qmsgentry queuemsgtab[];

extern int32 qmsgcount;
extern int32 qemptyslotcount;

#define	getqueueindex()	((qmsgcount%10 == 0)?(qmsgcount = 0):(qmsgcount))
#define incrqindex()	(qmsgcount=(qmsgcount%10)+1)
#define decrqindex()	(qmsgcount=(qmsgcount)-1)
#define incrempycount()	(qemptyslotcount=(qemptyslotcount)+1)
#define decrempycount() (qemptyslotcount = (qemptyslotcount) -1)
#define enquemsgq(qmsg) ({ queuemsgtab[getqueueindex()] = qmsg; incrqindex();decrempycount();})
#define readmsgq(index)	(queuemsgtab[index])
#define delmsg(index)	({ queuemsgtab[index].pid = -1; queuemsgtab[index].pid = "";incrempycount();})// decrqindex(); qmsgreadcount=qmsgreadcount+1})
#define getreadqindex() ((qmsgreadcount%10 == 0)?(qmsgreadcount = 0):(qmsgreadcount))



