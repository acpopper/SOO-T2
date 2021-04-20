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
    .procesos = NULL
  };
  return queue;
}

