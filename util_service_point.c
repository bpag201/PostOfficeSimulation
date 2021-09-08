#include "util_service_point.h"

/* Create a new service point */
extern SERVICE_POINT* createServicePoint(int x)
{
	SERVICE_POINT *temp;

	if(!(temp = (SERVICE_POINT*)malloc(sizeof(SERVICE_POINT))))
	{
		fprintf(stderr, "Insufficient memory\n");
		exit(-2);
	}

	temp->id              = x;
	temp->free            = TRUE;
	temp->personServicing = NULL;
}

/* Sets the person at the service point to the given person */
extern void attendPerson(SERVICE_POINT* s, PERSON* p)
{
	if (p)
	{
		s->personServicing = p;
		s->free            = FALSE;
	}
}

/* Sets the service point to free and returns the person that was at it */
extern PERSON* finishJob(SERVICE_POINT* s)
{
	s->free   = TRUE;
	PERSON *p = s->personServicing;

	s->personServicing = NULL;
	
	return p;
}

extern void deleteServicePoint(SERVICE_POINT* s)
{
	if (!s)
		return;

	s->personServicing = NULL;
	free(s);
}
