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

  srand(time(NULL));

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


  struct public_key pubK;
  pubK.p = 2725838081;
  pubK.g = 6;
  pubK.A = 1247564666;

  message m;
  m.i32 = 0;
  bloc b;
  int nb;
  while((nb = read(fdin, &m, 4))>0){
    b = bloc32_encrypton(m, &pubK);
    write(fdout, &b, 8);
    m.i32 = 0;
  }

  close(fdin);
  close(fdout);

  return 0;

}
