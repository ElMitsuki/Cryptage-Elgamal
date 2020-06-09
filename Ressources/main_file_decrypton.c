#include "elgamal.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char**argv){

  if(argc != 3){
    printf("Erreur\n");
    exit(1);
  }

  struct private_key privK_test;
  privK_test.p = 2725838081;
  privK_test.a = 16807;

  int fdin;
  int fdout;
  if((fdin = open(argv[1], O_RDONLY))<0){
    perror("open");
    exit(1);
  }
  if((fdout = open(argv[2], O_WRONLY | O_TRUNC))<0){
    perror("open");
    exit(1);
  }

  bloc b;
  b.i64 = 0;
  message m;
  int nb;
  while((nb = read(fdin, &b, 8))>0){
    m = bloc64_decrypton(b, &privK_test);
    write(fdout, &m, 4);
    b.i64 = 0;
  }

  close(fdin);
  close(fdout);

  return 0;

}
