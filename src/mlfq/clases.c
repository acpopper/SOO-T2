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
    .transcurrido_waiting=0,
    .transcurrido_exec=wait,
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


bool allFinished(Queue** colas, int Q, Queue* cola_finished, Queue* cola_running){
  if(!cola_finished->head){
    return false;
  }
  if(cola_running->head){
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
  proceso->prioridad=cola->prioridad;
}


void run_first_priority(Queue** colas, int Q, int tick, Queue* cola_running){
  for(int i=0; i<Q;i++){
    Process* current = colas[i]->head;
    
    while(current){
      
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
    cola_running->head->transcurrido_exec -=1;
  }
  for(int i=0; i<Q;i++){
    Process* current = colas[i]->head;
    while(current){
      if(current->estado==WAITING){     
        current->transcurrido_waiting+=1;
        current->waiting_time+=1;
      }
      else if(current->estado==READY){
        current->ready_time+=1;
      }
      current = current->next;
    }
}
}

void print_de_prueba(Queue** colas, Queue* running, int Q){
  if(running->head){
    printf("Running: %s tiempo aca %i, running in prioridad %i\n", running->head->nombre, running->head->wait-running->head->transcurrido_exec, running->head->prioridad);
  }
  
  for(int i=0; i<Q; i++){
    Process* current = colas[i]->head;
    printf("Cola %i prioridad %i quantum %i:\n",i, colas[i]->prioridad, colas[i]->quantum);
    while(current){
      if(current->estado==READY){
        printf("En cola %i (R): %s \n", i, current->nombre);
      }
      else{
        printf("En cola %i (W): %s tiempo aca %i\n", i, current->nombre, current->transcurrido_waiting);
      }
      
    current=current->next;  
  }
  }
  
}

Queue* find_parent_by_priority(Queue** colas, int Q, int p){
  if(p==Q){
    return colas[0];
  }
  else if(p==-1){
    return colas[Q-1];
  }
  for(int i=0; i<Q; i++){
    if(colas[i]->prioridad==p){
      return colas[i];
    }
  }
  return NULL;
}

// ver si el proceso corriendo cede, se interrumpe o termina
void time_up_check(Queue** colas, Queue* cola_running, Queue* cola_finished, int tick, int Q){
  Process* current = cola_running->head;
  //si termina:
  if(current->cycles==0){
    current->estado=FINISHED;
    current->terminado=tick;
    current->turnaround_time=tick-current->llegada;
    move_to_tail(current, cola_finished);
  }
  else{
    bool interrumpido=false;
    bool cede=false;
    //si lleva running lo que dura el quantum (se interrumpe):
    int K = find_parent_by_priority(colas, Q, current->prioridad)->quantum;
    int ex = (current->transcurrido_exec);
    int w = (current->wait);
    // printf("w %i ex %i K %i\n", w, ex, K);
    if(w-K==ex){
      // printf("Se interrumpe\n");
      interrumpido=true;
      current->estado=READY;
      current->interrumpido+=1;
    }
    if(current->transcurrido_exec==0 && current->wait!=0){
      cede=true;
      current->estado=WAITING;
      current->transcurrido_exec=current->wait;
    }
    if(interrumpido){
      move_to_tail(current, find_parent_by_priority(colas, Q, current->prioridad-1));

    } else if(cede){
      move_to_tail(current, find_parent_by_priority(colas, Q, current->prioridad+1));
    }
  }
}

void waiting_to_ready(Queue** colas, int Q){
  for(int i=0; i<Q; i++){
    Process* current = colas[i]->head;
    while(current){
      if(current->estado==WAITING && current->transcurrido_waiting==current->delay){
        current->estado=READY;
        current->transcurrido_waiting=0;

      }
    current=current->next;  
  }
  }

}


void special_time(Queue** colas, int Q, int tick, int S){
  
  if(!(tick%S)){  
    for(int i=1; i<Q; i++){
      if(colas[i]->head){
        Process* aux;
        Process* current = colas[i]->head;
        while(current){
          aux = current->next;
          move_to_tail(current, colas[0]);
          current=aux;  
        }
        free(aux);
      }
    }
  }
}
