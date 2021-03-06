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
  Queue* cola_running=queue_init(Q, Q-1, -1);
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
  
  while (!allFinished(colas, Q, cola_finished, cola_running)){
    // printf("????ITER %i????\n", tick);
    // todos pasaron 1 tiempo en su estado
    sumar_tick(colas, Q, cola_running);

    // los waiting que pasan a ready lo hacen
    waiting_to_ready(colas, Q);

    //ver si llega otro proceso NUEVO
    llega_alguno(cola_starters, colas, tick);
    
    // Si no hay ninguno corriendo, se corre el de mayor prioridad
    // if(!someone_running(cola_running)){
    //   run_first_priority(colas, Q, tick, cola_running);
    // }
    
    // Si hay uno corriendo se revisa si se termin??, si se acab?? el quantum y/o si se va a wait por su cuenta
    if(someone_running(cola_running)){
      time_up_check(colas, cola_running, cola_finished, tick, Q);
    }


    if(!someone_running(cola_running)){
      run_first_priority(colas, Q, tick, cola_running);
    }

    // SPECIAL TIME
    special_time(colas, Q, tick, S);
    
    //print de colas en cada iteraci??n
    // print_de_prueba(colas, cola_running, Q);
    // printf("\n");

    // Se suma un tick al tiempo
    tick+=1;
  }
  
  Process* current = cola_finished->head;
  while (current)
  { fprintf(output_file, "%s,%i,%i,%i,%i,%i\n", current->nombre, current->elegido, current->interrumpido, current->turnaround_time, current->response_time, current->ready_time+current->waiting_time);
    // printf("Finished: %s, elegido %i, interr %i, turnaround %i, response %i, r+w %i\n", 
    // current->nombre, current->elegido, current->interrumpido, current->turnaround_time, current->response_time, current->ready_time+current->waiting_time);
    current=current->next;
  }
  
  
  printf("aca\n");
  destruccion_total(colas, Q, cola_starters, cola_finished, cola_running);
  printf("aca2\n");
  input_file_destroy(archivo);
  fclose(output_file);
}
