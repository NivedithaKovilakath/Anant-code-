#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <ctype.h>

#define MSG_QUEUE_H
#define MSG_Q_KEY 1000
#define MSG_TYPE_P 2
#define MSG_TYPE_CO 1
#define MAX_LINE_LENGTH 100
#define MAX_MSG_SIZE 200

typedef struct msg
{
  long m_type;
  char m_text[MAX_LINE_LENGTH];
}message;

int main()
  {
    int msgq_id=msgget(MSG_Q_KEY, IPC_CREAT);
    printf("message queue id is %d ", msgq_id);

    size_t msg_size;
    size_t msg_len;

    message m_write;
    message m_read;
    m_write.m_type=MSG_TYPE_CO;
    m_read.m_type=MSG_TYPE_P;

    while(1)
      {
        msg_len=msgrcv(msgq_id, &m_read,MAX_MSG_SIZE,MSG_TYPE_P,0);

        m_read.m_text[msg_len]='\0';
        printf("message received which is %s and of type %ld",m_read.m_text, m_read.m_type);

        for(int i=0;i<strlen(m_read.m_text); i++)
          {
            m_write.m_text[i]=toupper(m_read.m_text[i]);
          }

        msg_size=strlen(m_read.m_text);

        msgsnd(msgq_id,&m_write, msg_size,0);





      }
    return 0;
  }
