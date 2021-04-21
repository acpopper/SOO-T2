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
  int n_procesos;
  bool isEmpty;
} ;

struct process {
  int PID;
  char* nombre;
  int estado;
  // numero de veces que fue elegido
  int elegido;
  // numero de veces que fue interrumpido
  int interrumpido;
  // tiempo de termino - tiempo de llegada a la cola
  int turnaround_time;
  // desde que llega hasta que se empieza a ejecutar
  int response_time;
  // cuando llega a la cola
  int llegada;
  // ticks que pasa en CPU cada vez que entra (puede ser interrumpido)
  int cycles;
  // cuantos ciclos pasan antes de ceder cpu
  int wait;
  // cuantos ticks pasa en wait
  int delay;
  // ticks que pasa en ready total
  int ready_time;
  // ticks que pasa en waiting total
  int waiting_time;
  // tick cuando termina
  int terminado;
  Queue* cola;
} ;

// Metodos
Queue* queue_init(int cant_colas, int prioridad, int q, int procesos);
Process* process_init(int PID, char* nombre, int estado, 
int llegada, int cycles, int wait, int delay, Queue* cola);

enum estados{RUNNING, READY, WAITING, FINISHED};