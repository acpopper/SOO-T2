#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "clases.h"

Queue* queue_init(int prioridad, int cant_colas, int q)
{
  Queue* queue = malloc(sizeof(Queue));
  *queue = (Queue) {
    .quantum = (cant_colas-prioridad)*q
  };
  return queue;
}