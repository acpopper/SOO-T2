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
  // abre input 
  InputFile* archivo = read_file(argv[1]);
  int n_procesos = archivo->len;
  // instancia las colas
  for(int i=0; i<Q;i++){
    colas[i]=queue_init(Q, Q-1-i, q, n_procesos);
  }
  // print de prueba
  // for(int i=0; i<Q;i++){
  //   printf("Cola %i, prioridad %i, quantum %i\n",i, colas[i]->prioridad, colas[i]->quantum);
  // }
  // Se pobla la primera cola con todos los procesos
  for(int i = 0; i<n_procesos;i++){
    for(int j = 0; j<6;j++){

      printf("%s\n", archivo->lines[i][j]);
      
    }
  }
}
