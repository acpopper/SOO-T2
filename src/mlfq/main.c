#include <stdio.h>
#include <stdlib.h>
#include "../file_manager/manager.h"
#include "clases.h"

int main(int argc, char **argv)
{
  long Q = strtol(argv[3], NULL, 10);
  long q = strtol(argv[4], NULL, 10);
  long S = strtol(argv[5], NULL, 10);
  Queue* colas[Q];
  
  InputFile* archivo = read_file(argv[1]);
  int n_procesos = archivo->len;
  
  for(int i=0; i<Q;i++){
    colas[i]=queue_init(Q, Q-1-i, q, n_procesos);
  }
  for(int i=0; i<Q;i++){
    printf("Cola %i, prioridad %i, quantum %i\n",i, colas[i]->prioridad, colas[i]->quantum);
  }
  for(int i = 0; i<archivo->len;i++){
    for(int j = 0; j<6;j++){
      printf("%s\n", archivo->lines[i][j]);
      
    }
  }
  printf("%li, %li, %li\n", Q, q, S);
}
