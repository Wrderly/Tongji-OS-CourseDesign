#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
pipeline(int fd)
{
  int pnum;
  if(read(fd,&pnum,sizeof(int))==0){
    close(fd);
    exit(1);
  }
  printf("prime %d\n",pnum);
  int cp[2];
  pipe(cp);
  if(fork()==0){
    close(cp[1]);
    pipeline(cp[0]);
    exit(0);
  }
  close(cp[0]);
  int num;
  while(read(fd,&num,sizeof(int))){
    if(num%pnum!=0){
      write(cp[1],&num,sizeof(int));
    }
  }
  close(fd);
  close(cp[1]);
  wait(0);
  exit(0);
}

int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);
  if(fork()==0){
    close(p[1]),
    pipeline(p[0]);
    exit(0);
  }
  close(p[0]);
  for(int i=2;i<=35;++i){
    write(p[1],&i,sizeof(int));
  }
  close(p[1]);
  wait(0);
  exit(0);
}