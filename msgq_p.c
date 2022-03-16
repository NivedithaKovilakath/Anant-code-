#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/types.h>
#include "msg_q.h"
#include <ctype.h>

int main(int argc, char *argv[])
  {
    pid_t c0_pid=fork();
    int msgq_id=msgget(MSG_Q_KEY, IPC_CREAT);

    if(msgq_id==-1)
      {
        printf("incorrect message id created ");
        exit(0);
      }

    printf("message queue id is %d ",msgq_id);

    if(c0_pid==0)
      {
        char *path_child=argv[1];
        if(execl(path_child,path_child,(char*)NULL)==-1)
          {
            perror("execl");
          }
      }

    char str[MAX_LINE_LENGTH];
    size_t msg_size;
    size_t len_rcvd;

    message m_write;
    message m_read;

    while(fgets(str, MAX_LINE_LENGTH, stdin)!=NULL)
      {
        m_write.mtype=MSG_TYPE_P;
        m_write.mtype=MSG_TYPE_C0;

        strcpy(m_write.mtext,str);
        msg_size=strlen(m_write.mtext);

        if(msgsnd(msgq_id, &m_write,msg_size,0)==-1){
          perror("error in message sending ");
          exit(0);
        }

        if((len_rcvd=msgrcv(msgq_id,&m_read,MAX_MSG_SIZE,MSG_TYPE_C0,0))==-1)
          {
            perror("error in messsage receving");
              exit(0);
          }

        m_read.mtext[len_rcvd]='\0';
        printf("message received is %s",m_read.mtext);
        strcpy(str,"");

      }
    return 0;
  }
