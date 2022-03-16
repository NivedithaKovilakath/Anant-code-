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
#include <errno.h>

void err_exit(char *msg)
  {
    printf("client-");
    perror(msg);
    exit(EXIT_FAILURE);
  }

int main()
  {
    int M=1024;
    int N=10;

    union semun arg;
    int buffer_avail, tmp_buffer_avail;
    struct semid_ds ds;

    int semid=semget(CLIENT_SERVER_KEY,3,0666|IPC_CREAT);

    if(semid==-1)
      {
        printf("semaphore segment failed");
      }

    arg.val=0;
    if(semctl(semid,0,SETVAL,arg)==-1)
      {
        err_exit("semctl reader ");
      }
    arg.val=1;
    if(semctl(semid,1,SETVAL,arg)==-1){
      err_exit("semctl writer");
    }
    arg.val=M;
    if(semctl(semid,2,SETVAL,arg)==-1)
      {
        err_exit("semctl buffer");
      }

    if(buffer_avail=semctl(semid,2,SETVAL,arg)==-1)
      {
        err_exit("semctl got offset");
      }

    struct sembuf sops[1];
    struct data_item *data_ptr,*tmp_ptr;

    int shmid=shmget(SHM_KEY,M,0666|IPC_CREAT);
    void* shm_addr=shmat(shmid,0,0);


    if(shm_addr==(void*)-1)
      {
        err_exit("shmat: ");
      }

    for(int i=0;i<N;i++)
      {
        if( (tmp_buffer_avail=semctl(semid,2,GETVAL,arg))==-1 ){
          err_exit("semaphore offset");
        }

        arg.val=0;
        sops[0].sem_num=2;
        sops[0].sem_op=( (short)-1*(short)sizeof(struct data_item) );
        sops[0].sem_flg=0;

        if(semop(semid,sops,1)==-1){
          err_exit("semop available memory check");
        }

        if( (buffer_avail=semctl(semid,2,GETVAL,arg))==-1 ){
          err_exit("semctl got offset");
        }

        if(tmp_buffer_avail !=0){
          buffer_avail+=sizeof(struct data_item);
        }

        data_ptr=(struct data_item *)(shm_addr+M-buffer_avail);

        if(data_ptr==NULL){
          printf("client data ptr is null");
        }

        data_ptr->a=i+1;
        data_ptr->b=i+1;
        data_ptr->pid=getpid();
        data_ptr->slno=i;

        printf("client : pid %d, slnno %d, a %d, b %d, ",getpid(),data_ptr->slno,data_ptr->a,data_ptr->b);
        sops[0].sem_num=1;
        sops[0].sem_op=1;
        sops[0].sem_flg=0;

        if(semop(semid,sops,1)==-1){
          err_exit("semop leave write access");
        }
      }
    return 0;
  }
