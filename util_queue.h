#ifndef __UTIL_QUEUE_H
#define __UTIL_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include "util_person.h"

/* ----------------------- STRUCTURES */
typedef struct queue {
	int     count;
	int     maxLength;
	PERSON *front;
	PERSON *rear;
} QUEUE;

/* ----------------------- PROTOTYPES */
extern QUEUE*      createQueue(int);
extern void        enQueue(QUEUE*, PERSON*);
extern PERSON*     deQueue(QUEUE*);
extern void        removeFromQueue(QUEUE*, PERSON*);
#endif
