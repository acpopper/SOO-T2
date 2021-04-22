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
    .prioridad=(cola ? cola->prioridad : -1),
    .next = NULL,
    .prev = NULL
  };
  return process;
}


bool allFinished(Queue** colas, int Q, Queue* cola_finished){
  if(!cola_finished->head){
    return false;
  }
  for(int i=0; i<Q;i++){
  // printf("Cola %i, prioridad %i, quantum %i\n",i, colas[i]->prioridad, colas[i]->quantum);    
    Process* current = colas[i]->head;
    while(current){
      // printf("%s\n",current->nombre);
      if(current->estado!=FINISHED){
        return false;
      }
      current = current->next;
    }
  }
  return true;
  
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
      current->prioridad = cola_inicial->prioridad;
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
  proceso->next=NULL;
  proceso->prev=NULL;
}

bool someone_running(Queue* cola_running){
if(cola_running->head){
  return true;
}
return false;
}

void move_to_head(Process* proceso, Queue* cola){
  desanclar(proceso);
  if(cola->head){
    proceso->parent = cola;
    cola->head->prev=proceso;
    proceso->next=cola->head;
    cola->head=proceso;
  }
  else{
    proceso->parent=cola;
    cola->head=proceso;
    cola->tail=proceso;
  }
}

void move_to_tail(Process* proceso, Queue* cola){
  desanclar(proceso);
  if(cola->head){
    proceso->parent = cola;
    cola->tail->next=proceso;
    proceso->prev=cola->tail;
    cola->tail=proceso;
  }
  else{
    proceso->parent=cola;
    cola->head=proceso;
    cola->tail=proceso;
  }
}


void run_first_priority(Queue** colas, int Q, int tick, Queue* cola_running){
  for(int i=0; i<Q;i++){
    Process* current = colas[i]->head;
    while(current){
      // printf("%s\n",current->nombre);
      if(current->estado==READY){
        current->estado=RUNNING;
        if(current->elegido==0){
          current->response_time=tick-current->llegada;
        }
        current->elegido+=1;

        
        move_to_head(current, cola_running);
        break;
      }
      current = current->next;
    }
  }
}

void sumar_tick(Queue** colas, int Q, Queue* cola_running){
  if(cola_running->head){
    cola_running->head->cycles-=1;
    cola_running->head->transcurrido +=1;
  }
  for(int i=0; i<Q;i++){
    Process* current = colas[i]->head;
    while(current){
      current->transcurrido+=1;
      if(current->estado==WAITING){        
        current->waiting_time+=1;
      }
      else if(current->estado==READY){
        current->ready_time+=1;
      }
      current = current->next;
    }
}
}

void print_de_prueba(Queue** colas, Queue* running){
  Process* current = colas[0]->head;
  if(running->head){
    printf("Running: %s (%i) tiempo aca %i\n", running->head->nombre, running->head->estado, running->head->transcurrido);
  }
  while(current){
    printf("En cola: %s (%i) tiempo aca %i\n", current->nombre, current->estado, current->transcurrido);
    
    current=current->next;  
  }
}


