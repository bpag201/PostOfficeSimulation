#ifndef __UTIL_SERVICE_POINT_H
#define __UTIL_SERVICE_POINT_H

#include "util_boolean.h"
#include "util_person.h"
#include <stdio.h>

/* ------------------------- STRUCTURES */
typedef struct Point {
	int     id;
	PERSON *personServicing;
	BOOLEAN free;
} SERVICE_POINT;

/* ------------------------- STRUCTURES */
extern SERVICE_POINT* createServicePoint(int);
extern void           attendPerson(SERVICE_POINT*, PERSON*);
extern PERSON*        finishJob(SERVICE_POINT*);
extern void           deleteServicePoint(SERVICE_POINT*);

#endif
