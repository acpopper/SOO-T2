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
  Process* head;
  Process* tail;
} ;

struct process {
  int PID;
  char* nombre;
  int estado;
  // numero de veces que fue elegido [MOD]
  int elegido;

  // numero de veces que fue interrumpido [MOD]
  int interrumpido;

  // tiempo de termino - tiempo de llegada a la cola [MOD]
  int turnaround_time;

  // desde que llega hasta que se empieza a ejecutar [MOD]
  int response_time;

  // cuando llega a la cola
  int llegada;

  // ticks que pasa en CPU 
  int cycles;

  // cuantos tiempos pasan antes de ceder cpu
  int wait;

  // cuanto tiempo ha pasado en el estado waiting [MOD]
  int transcurrido_waiting;

  // cuanto tiempo falta para wait [MOD]
  int transcurrido_exec;

  // cuantos ticks pasa en wait
  int delay;

  // ticks que pasa en ready total [MOD]
  int ready_time;

  // ticks que pasa en waiting total [MOD]
  int waiting_time;

  // tick cuando termina [MOD]
  int terminado;

  //prioridad de la cola actual, se usará para cambiar de cola [MOD]
  int prioridad;
  Queue* parent;
  Process* next;
  Process* prev;
} ;

// Metodos
Queue* queue_init(int cant_colas, int prioridad, int q);
Process* process_init(int PID, char* nombre, int estado, 
int llegada, int cycles, int wait, int delay, Queue* cola);

enum estados{RUNNING, READY, WAITING, FINISHED};

bool allFinished(Queue** colas, int Q, Queue* cola_finished, Queue* cola_running);
void llega_alguno(Queue* cola_starters, Queue** colas, int tick);
void desanclar(Process* proceso);
bool someone_running(Queue* cola_running);
void run_first_priority(Queue** colas, int Q, int tick, Queue* cola_running);
void move_to_head(Process* proceso, Queue* cola);
void move_to_tail(Process* proceso, Queue* cola);
void sumar_tick(Queue** colas, int Q, Queue* cola_running);
void print_de_prueba(Queue** colas, Queue* running, int Q);
Queue* find_parent_by_priority(Queue** colas, int Q, int p);
void time_up_check(Queue** colas, Queue* cola_running, Queue* cola_finished, int tick, int Q);
void waiting_to_ready(Queue** colas, int Q);
void special_time(Queue** colas, int Q, int tick, int S);
