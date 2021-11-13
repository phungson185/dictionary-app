#include "cqueue.h"
#include <stdlib.h>
#include <stdio.h>

Queue queue_create(){
  Queue new_q;
  new_q = new_dllist();
  return new_q;
}
void queue_push(Queue q, Jval val){
  dll_insert_b(q->flink,val);
}
char* queue_pop(Queue q) {
    char* a;
    a =  dll_val(q->blink).s;
  dll_delete_node(q->blink);
  return a;
}
void queue_free(Queue q){
    free_dllist(q);
    free(q);
}