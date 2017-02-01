# define MAX_NUMBER_OF_TOPICS 256
# define MAX_SUBSCRIBERS 8 

/*
 * struct to hold the information regarding the subscriber
 */
typedef struct subscriber{
  pid32 pid;
  topic16 topicId;
  void (*handler)(topic16, void*, uint32);
  int valid;
  } subscriber;
/*
 * stores the hadnler of all subscribers for the broker
 */
extern subscriber subs[MAX_NUMBER_OF_TOPICS][MAX_SUBSCRIBERS];

/*
 * struct to hold the client entries in a buffer
 */
typedef struct bufClient{
  void (*handler)(topic16, void *, uint32);
  topic16 topicId;
  uint32 size;
  void* data;
  struct bufClient * next;
  }bufCli;

/*
 * head of the broker queue
 */
extern bufCli * const headBuf;

