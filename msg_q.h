#ifndef MSG_Q_H_
#define MSG_Q_H_

#define MSG_Q_KEY 1000
#define MSG_TYPE_P 2 //two types for writing- could be upper or lower case
#define MSG_TYPE_C0 1 //one type for reading reading info
#define MAX_LINE_LENGTH 100
#define MAX_MSG_SIZE 200


typedef enum
{
  false,
  true
}bool;

typedef struct msg
{
  long mtype;
  char mtext[MAX_LINE_LENGTH];
}message;


#endif // MSG_Q_H_
