#include <stdio.h>
#include <stdlib.h>
#include "../file_manager/manager.h"


int main(int argc, char **argv)
{
  long Q = strtol(argv[3], NULL, 10);
  long q = strtol(argv[4], NULL, 10);
  long S = strtol(argv[5], NULL, 10);
  InputFile* archivo = read_file(argv[1]);
  for(int i = 0; i<archivo->len;i++){
    for(int j = 0; j<6;j++){
      printf("%s\n", archivo->lines[i][j]);
    }
  }
  printf("%li, %li, %li\n", Q, q, S);
}
