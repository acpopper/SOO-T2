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
  Queue** colas=calloc(Q, sizeof(Queue*));
  
  // abre input 
  InputFile* archivo = read_file(argv[1]);
  int n_procesos = archivo->len;
  // abre output
  FILE *output_file = fopen(argv[2], "w");
  
  // instancia las colas y se almacenan en lista "colas"
  for(int i=0; i<Q;i++){
    colas[i]=queue_init(Q, Q-1-i, q);
  }
  Queue* cola_starters=queue_init(Q, Q-1, -1);
  Queue* cola_finished=queue_init(Q, Q-1, -1);
  // print de prueba
  // for(int i=0; i<Q;i++){
  //   printf("Cola %i, prioridad %i, quantum %i\n",i, colas[i]->prioridad, colas[i]->quantum);
  // }  

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
    Process* current = process_init(PID, nombre, estado, llegada, cycles, wait, delay, NULL);
    if(linea==0){
      cola_starters->head=current;
      cola_starters->tail=current;
      current->parent=cola_starters;
    }
    else{
      current->prev=cola_starters->tail;
      cola_starters->tail->next=current;
      current->parent=cola_starters;
      cola_starters->tail=current;
    }
  }
  
  //print de prueba
  // Process* current = colas[0]->head;
  // for(int linea = 0; linea<n_procesos;linea++){
  //   printf("%i\n", current->llegada);
  //   current=current->next;  
  // }
  
  while (!allFinished(colas, Q, cola_finished) && tick < 6){
    printf("Iter %i\n", tick);
    llega_alguno(cola_starters, colas, tick);
    // print de prueba
    // Process* current = colas[0]->head;
    // while(current){
    //   if(current->estado==READY){
    //   printf("%s\n", current->nombre);
    //   }
    //   current=current->next;  
    // }
    
    if(!someone_running(colas, Q)){
      run_first_priority(colas, Q);
    }
    
    Process* current = colas[0]->head;
    while(current){
      printf("%s, %i\n", current->nombre, current->estado);
      
      current=current->next;  
    }



    tick+=1;
  }
  
  
  fclose(output_file);
}
