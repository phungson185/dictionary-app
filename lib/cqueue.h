#ifndef CQUEUE_H_
#define CQUEUE_H_

#include <stddef.h>
#include "dllist.h"
#include "jval.h"

typedef Dllist Queue;

Queue queue_create();
void queue_push(Queue, Jval);
char* queue_pop(Queue);
void queue_free(Queue);

#endif  