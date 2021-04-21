#include <stdio.h>
#include <stdlib.h>
#include "../file_manager/manager.h"
#include "clases.h"

int tick=0;


int main(int argc, char **argv)
{
  long Q = strtol(argv[3], NULL, 10);
  long q = strtol(argv[4], NULL, 10);
  long S = strtol(argv[5], NULL, 10);
  Queue* colas[Q];
  // abre input 
  InputFile* archivo = read_file(argv[1]);
  int n_procesos = archivo->len;
  // abre output
  FILE *output_file = fopen(argv[2], "w");
  
  // instancia las colas y se almacenan en lista "colas"
  for(int i=0; i<Q;i++){
    colas[i]=queue_init(Q, Q-1-i, q);
  }

  // print de prueba
  // for(int i=0; i<Q;i++){
  //   printf("Cola %i, prioridad %i, quantum %i\n",i, colas[i]->prioridad, colas[i]->quantum);
  // }

  // Se pobla la lista de los starters con todos los procesos
  Process* starters[n_procesos];
  Queue* finished_queue = queue_init(0, -1, 0);
  Queue* running_queue = queue_init(0, -1, 0);
  Queue* waiting_queue = queue_init(0, -1, 0);

  for(int linea = 0; linea<n_procesos;linea++){
    //variables para instanciar un proceso
    char* nombre=archivo->lines[linea][0];
    int PID = strtol(archivo->lines[linea][1], NULL, 10);
    int estado = WAITING;
    int llegada = strtol(archivo->lines[linea][2], NULL, 10);
    int cycles=strtol(archivo->lines[linea][3], NULL, 10);
    int wait=strtol(archivo->lines[linea][4], NULL, 10);
    int delay=strtol(archivo->lines[linea][5], NULL, 10);
    // printf("%s, %i, %i, %i, %i, %i, %i\n", nombre, PID, estado, llegada, cycles, wait, delay);
    starters[linea]=process_init(PID, nombre, estado, llegada, cycles, wait, delay, NULL);
  }
  while (allFinished(finished_queue, n_procesos)){
    
  }
  
  
  fclose(output_file);
}
