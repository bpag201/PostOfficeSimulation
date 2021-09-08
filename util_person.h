#ifndef __UTIL_PERSON_H
#define __UTIL_PERSON_H

#include <stdio.h>
#include <stdlib.h>

#include "util_generator.h"

/* ----------------------- STRUCTURES */
typedef struct Person {
	int            id;
	struct Person *next;
	struct Person *previous;
	double         leaveChance;
	int            timeAtPoint;
	int            totalServiceTime;
} PERSON;

/* ----------------------- PROTOTYPES */
extern PERSON* newPerson(int, double, int);
extern void    deletePerson(PERSON*);

#endif
