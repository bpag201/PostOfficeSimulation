#include "util_person.h"

/* ------------------- FUNCTIONS */
/* Create a new person */
extern PERSON* newPerson(int i, double chance, int maxServiceTime)
{
	PERSON *temp;

	if (!(temp = (PERSON*)malloc(sizeof(PERSON))))
	{
		fprintf(stderr, "Insufficient memory\n");
		exit(-2);
	}

	temp->id               = i;
	temp->next             = NULL;
	temp->previous         = NULL;
	temp->leaveChance      = chance;
	temp->timeAtPoint      = 0;
	temp->totalServiceTime = serviceTime(maxServiceTime);
	return temp;
}

/* Delete a person */
extern void deletePerson(PERSON *p)
{
	if(p)
	{	
		p->next    = NULL;
		p->previous = NULL;
		free(p);
		p = NULL;
	}
}

