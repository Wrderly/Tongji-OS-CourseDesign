#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

int getOneLine(char *buf) {
  char ch;
  char *str = buf;
  while (read(0, &ch, 1) == 1 && ch != '\n')
    *buf++ = ch;//从标准输入中读取一行存入buf
  return strlen(str);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(2, "xargs take one argument at least.");
    exit(1);
  }
  char *args[MAXARG];
  for (int i = 0; i < argc - 1; ++i) {
    args[i] = argv[i + 1];
  }//复制除了第一个参数的所有命令行参数
  char buf[MAXPATH] = {0};
  while (getOneLine(buf)) {
    args[argc - 1] = buf;//每次读取一行并存在末尾
    args[argc] = 0;
    if (fork() == 0) {
      exec(argv[1], args);//由子进程去执行
      exit(0);
    }
    wait(0);
    memset(buf, 0, MAXPATH);//清空
  }
  exit(0);
}
