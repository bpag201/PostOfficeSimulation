#ifndef __UTIL_GENERATOR_H
#define __UTIL_GENERATOR_H

#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "util_boolean.h"

/* ---------------------------------- STRUCTURES */
typedef struct generator {
	const gsl_rng_type *rng_type;
	gsl_rng            *rng;
} GENERATOR;

/* ---------------------------------- PROTOTYPES */
extern int getNumPeople(double);
extern BOOLEAN leaves(double);
extern int serviceTime(int);
extern GENERATOR* createGenerator();

#endif
