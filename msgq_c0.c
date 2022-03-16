//#include "msgq_c0.h"

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <ctype.h>
#include "msg_q.h"


int main()
  {
    int msgq_id=msgget(MSG_Q_KEY, IPC_CREAT);
    printf("message queue id is %d ", msgq_id);

    size_t msg_size;
    size_t msg_len;

    message m_write;
    message m_read;
    m_write.mtype=MSG_TYPE_C0;
    m_read.mtype=MSG_TYPE_P;


    while(true)
      {
        msg_len=msgrcv(msgq_id, &m_read,MAX_MSG_SIZE,MSG_TYPE_P,0);

        m_read.mtext[msg_len]='\0';
        printf("message received which is %s and of type %ld",m_read.mtext, m_read.mtype);
        int len=strlen(m_read.mtext);
        int i;
        for( i=0;i<len; i++)
          {
            m_write.mtext[i]=toupper(m_read.mtext[i]);
          }

        msg_size=strlen(m_read.mtext);

        msgsnd(msgq_id,&m_write, msg_size,0);

      }
    return 0;
  }
