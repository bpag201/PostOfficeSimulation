#include "util_generator.h"

/* Creates a new random number generator */
extern GENERATOR* createGenerator()
{
	GENERATOR *g;
	
	if (!(g = (GENERATOR*)malloc(sizeof(GENERATOR))))
	{
		printf("Out of memory\n");
		exit(1);
	}
	
	gsl_rng_env_setup();

	g->rng_type = gsl_rng_default;
	g->rng      = gsl_rng_alloc(g->rng_type);

	gsl_rng_set(g->rng, time(0));

	return g;
}

/* Returns the number of people that arrive at the store in a 
  given tick using the Poisson distribution - with the mean 
  as the given int */   
extern int getNumPeople(double mean)
{
	GENERATOR *g = createGenerator();
	
	int numPeople = gsl_ran_poisson(g->rng, mean);

	gsl_rng_free(g->rng);
	free(g);

	return numPeople;
}

/* Calculates whether or not a customer leaves using the Bernoulli
   distibution - with probability of leaving being a given double 
   chance */
extern BOOLEAN leaves(double chance)
{
	GENERATOR *g = createGenerator();

	BOOLEAN b = gsl_ran_bernoulli(g->rng, chance);

	gsl_rng_free(g->rng);
	free(g);

	return b;
}

/* Calculates the amount of time it will take for the customer to be served
   using the Gaussian distribution - will be somewhere between 1 and a 
   given int max using */
extern int serviceTime(int max)
{
	GENERATOR *g = createGenerator();

	int x = gsl_ran_flat(g->rng, 1, max);

	gsl_rng_free(g->rng);
	free(g);

	return x;
}
