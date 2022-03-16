#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#define bufsize 20
int msgCount=0;

void signalHandler(int sig)
  {
    if(sig==SIGUSR2)
      {
        printf("the messages between c1 and c2 are %d ", msgCount);

        kill(0,SIGINT);
      }

    else if(sig==SIGUSR1)
      {
        msgCount++;
      }
  }


int main()
  {
    int pipe1[2];//send data from 1 to 2
    int pipe2[2];//to send data from 2 to 1
    pipe(pipe1);
    pipe(pipe2);

    char m='4';
    char n='2';

   char buf1[bufsize];
   char buf2[bufsize];
   buf1[bufsize-1]='\0';
   buf2[bufsize-1]='\0';

   strcpy(buf1,m);
   strcpy(buf2,n);

   pid_t pid1,pid2;

   pid1=fork();

   if(pid1==-1)
     {
       printf("error in forking 1");
     }

   else if(pid1>0)//parent P
     {
       pid2 =fork();
     }

   if(pid2==-1)
     {
       printf("error in forking 2");
     }

   if(pid1>0 && pid2>0)
     {
       //parent process
       //close all the ends of the pipe
       close(pipe1[0]);
       close(pipe1[1]);
       close(pipe2[0]);
       close(pipe2[1]);

       kill(getppid(),SIGUSR2);
     }

   if(pid1!=0 && pid2==0)
     {
       close(pipe1[0]);
       close(pipe2[1]);
       write(pipe1[1],&buf1, bufsize);
       m=read(pipe2[0],line, bufsize);
     }

   return 0;

  }
