#ifndef CS_HEADER_H_
#define CS_HEADER_H_
#include <sys/types.h>

#define CLIENT_SERVER_KEY 0x13061
#define SHM_KEY 0x13411
#define MAX_TRIES 10

struct data_item{
  pid_t pid;
  int slno;
  int a;
  int b;
};

union semun{
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};


#endif // CS_HEADER_H_
