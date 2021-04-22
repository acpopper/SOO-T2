#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "clases.h"

Queue* queue_init(int cant_colas, int prioridad, int q)
{
  Queue* queue = malloc(sizeof(Queue));
  *queue = (Queue) {
    .quantum = (cant_colas-prioridad)*q,
    .prioridad = prioridad,
    .head = NULL,
    .tail = NULL
  };
  return queue;
}

Process* process_init(int PID, char* nombre, int estado,
int llegada, int cycles, int wait, int delay, Queue* cola)
{
  Process* process = malloc(sizeof(Process));
  *process = (Process) {
    .PID=PID,
    .nombre=nombre,
    .estado=estado,
    .llegada=llegada,
    .cycles=cycles,
    .wait=wait,
    .delay=delay,
    .transcurrido=0,
    .parent = cola,
    .elegido=0,
    .interrumpido=0,
    .turnaround_time=0,
    .response_time=0,
    .ready_time=0,
    .waiting_time=0,
    .terminado=0,
    .prioridad=-1,
    .next = NULL,
    .prev = NULL
  };
  return process;
}


bool allFinished(Queue** colas, int Q){
  
  // for(int i=0; i<Q;i++){
  // // printf("Cola %i, prioridad %i, quantum %i\n",i, colas[i]->prioridad, colas[i]->quantum);    
  // Process* current = colas[i]->head;
  //   while(current){
  //     // printf("%s\n",current->nombre);
  //     if(current->estado!=FINISHED){
  //       return true;
  //     }
  //     current = current->next;
  //   }
  // }
  // return true;
  return false;
}

void llega_alguno(Queue* cola_starters, Queue** colas, int tick){
  Queue* cola_inicial = colas[0];
  Process* current = cola_starters->head;
  
  
  while (current){
    Process* aux = current->next;
    if(current->llegada == tick){
      current->estado=READY;
      desanclar(current);
      current->parent=cola_inicial;
      if(!cola_inicial->head){        
        cola_inicial->head=current;
        cola_inicial->tail=current;

      }
      else{
        current->prev=cola_inicial->tail;
        cola_inicial->tail->next=current;
        cola_inicial->tail=current;
      }
    }
    current=aux;
  }
}

void desanclar(Process* proceso){
  if(proceso->prev && proceso->next){            
    proceso->prev->next=proceso->next;
    proceso->next->prev=proceso->prev;
  }
  else if(proceso->prev){
    proceso->parent->tail=proceso->prev;
    proceso->prev->next=NULL;
  }
  else if(proceso->next){            
    proceso->parent->head=proceso->next;
    proceso->next->prev=NULL;            
  } else{
    proceso->parent->head=NULL;
    proceso->parent->tail=NULL;
  }
  proceso->parent=NULL;
}