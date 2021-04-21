#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "clases.h"

Queue* queue_init(int cant_colas, int prioridad, int q, int procesos)
{
  Queue* queue = malloc(sizeof(Queue));
  *queue = (Queue) {
    .quantum = (cant_colas-prioridad)*q,
    .isEmpty = true,
    .prioridad = prioridad,
    .n_procesos = procesos,
    .procesos = calloc(procesos, sizeof(Process))
  };
  return queue;
}

Process* process_init(int PID, char nombre[32], int estado,
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
    .cola = cola,
    .elegido=0,
    .interrumpido=0,
    .turnaround_time=0,
    .response_time=0,
    .ready_time=0,
    .waiting_time=0,
    .terminado=0
  };
  return process;
}
