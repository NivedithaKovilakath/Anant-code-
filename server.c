#include "cs_header.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <stdio.h>

int semid;
int shmid;
void* shm_addr;
void err_exit(char *msg){

  perror(msg);
  exit(EXIT_FAILURE);
}

void cleanup(int signo){
  if(shmdt(shm_addr)==-1)
    {
      err_exit("smhdt");
    }

  if(shmctl(semid,IPC_RMID,NULL)==-1)
    {
      err_exit("shm rm");
    }

  if(semctl(semid,0,IPC_RMID)==-1){
    err_exit("sem rm");
  }
}

int main()
  {

    signal(SIGINT, &cleanup);
    int M=1024;

    union semun arg;
    int buffer_avail;
    struct semid_ds ds;

    semid=semget(SHM_KEY,3,0666|IPC_CREAT);
    //semaphore has 3 spaces. 0-read. 1-write. 2-amt of memory

    if(semid==-1)
      {
        err_exit("semaphore not created error");
      }

    struct sembuf sops[1];
    struct data_item *data_ptr;
    shmid=shmget(SHM_KEY,M,0666|IPC_CREAT);

    if(shmid==-1)
      {
        err_exit("shared memory error");
      }
    shm_addr=shmat(shmid,NULL,0);
    if(shm_addr==(void*)-1)
      {
        err_exit("shmat");
      }

    int offset=0;
    //to initialise semaphores

    for(int j=0;j<MAX_TRIES; j++)
      {
        arg.buf=&ds;
        if(semctl(semid,0,IPC_STAT,arg)==-1)
          {
            perror("semctl");
            exit(EXIT_FAILURE);
          }


    if(ds.sem_otime!=0)
      {
        break;
        sleep(1);
      }
    }

    if(ds.sem_otime==0){
      printf("semaphore not initialised");
      exit(EXIT_FAILURE);
    }

    for(;;){
      if( (buffer_avail=semctl(semid,2,GETVAL,arg))==-1){
        err_exit("semctl error");
      }

      if(M-buffer_avail==0)
        {
          offset=0;
        }

      if((M-buffer_avail)>=sizeof(struct data_item))
        {
          data_ptr=(struct data_item*)(shm_addr+offset);
          offset+=sizeof(struct data_item);
          offset%=M;

          //printing details
          printf("Server's pid- %d, slno- %d, a-%d, b-%d ",getpid(),data_ptr->slno,data_ptr->a,data_ptr->b);

          sops[0].sem_num=2;
          sops[0].sem_op=sizeof(struct data_item);
          sops[0].sem_flg=0;


        }
    }

    return 0;
  }
