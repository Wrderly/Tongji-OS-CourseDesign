#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 1){
    fprintf(2, "Usage: pingpong...\n");
    exit(1);
  }
  char buf[1] = {'a'};
  int ptc[2],ctp[2];
  pipe(ptc);
  pipe(ctp);
  int pid = fork();
  if(pid > 0){
    write(ptc[1],buf,1);
    read(ctp[0],buf,1);
    printf("%d:received pong\n", getpid());
    wait(&pid);
    exit(0);
  }else if(pid == 0){
    read(ptc[0],buf,1);
    printf("%d:received ping\n", getpid());
    write(ctp[1],buf,1);
    exit(0);
  }else{
    printf("fork error");
  }
  exit(0);
}