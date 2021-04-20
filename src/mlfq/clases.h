#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

struct queue;
typedef struct queue Queue;
struct process;
typedef struct process Process;


struct queue {
  int quantum;
  int prioridad;
  Process* procesos;
  bool isEmpty;

} ;

struct process {
  int PID;
  char nombre[32];
  char* estado;
  int elegido;
  int interrumpido;
  int turnaround_time;
  int response_time;
  int inicio;
  int cycles;
  int wait;
  int delay;
  int ready_time;
  int waiting_time;
  Queue* cola;
} ;



