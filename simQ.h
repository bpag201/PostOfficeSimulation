#ifndef __SIMQ_H
#define __SIMQ_H

#include <stdio.h>
#include <stdlib.h>

/* -------------------------------- USER DEFINED INCLUDES */
#include "util_queue.h"
#include "util_person.h"
#include "util_boolean.h"
#include "util_generator.h"
#include "util_linked_list.h"
#include "util_service_point.h"

/* -------------------------------- PROTOTYPES */
int  readInParameters(char*, int*, int*, int*, int*, double*, double*, double*);
void runSimulation(char*, int, int, int, int, double, double, double, int, int);
void serviceCurrentCustomers(int, double, double, double, int*, SERVICE_POINT_LIST*, QUEUE*, int*, int*, int*);
int writeToFile(char*, int, int, int, int, int, int, int , BOOLEAN, BOOLEAN, BOOLEAN, int, int, BOOLEAN);

#endif
