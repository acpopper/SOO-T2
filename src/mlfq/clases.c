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

bool allFinished(Queue* finished_queue, int n_procesos){
  int conteo = 0;
  Process* current = finished_queue->head;
  while(current){
    conteo+=1;
    current = current->next;
  }
  if(conteo==n_procesos){
    return true;
  }
  return false;
}