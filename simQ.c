#include "simQ.h"

/* ------------------------------------------ MAIN FUNCTION */
int main(int argc, char **argv)
{
	/* Check if the number of arguments given to the main function
	 * is what is expected */
	if (argc != 4)
	{
		fprintf(stderr, "Expected 3 parameters, only given %d\n", (argc - 1));
		exit(-1);
	}

	/* Arguments taken in */
	char *fileIn  = argv[1];
	int   numSims = atoi(argv[2]);
	char *fileOut = argv[3];

	BOOLEAN finished = FALSE;
	int counter   = 0;

	/* Create my required sim variables */
	int    maxQueueLength;
	int    numServicePoints;
	int    closingTime;
	int    maxServiceTime;
	double mean;
	double leaveChance;
	double boredomChanceIncrease;

	readInParameters(fileIn, &maxQueueLength, &numServicePoints, &closingTime, &maxServiceTime, &mean, &leaveChance, &boredomChanceIncrease);

	int i;
	for (i = 1; i <= numSims; i++)
		runSimulation(fileOut, maxQueueLength, closingTime, numServicePoints, maxServiceTime, mean, leaveChance, boredomChanceIncrease, numSims, i);
}

/* ------------------------------------------ USER DEFINED FUNCTIONS */

/* Takes the parameters required for the simulation from the required
   file */
int readInParameters(char *fileIn, int *maxQueueLength, int *numServicePoints, int *closingTime, int *maxServiceTime, double *mean, double *leaveChance, double *boredomChanceIncrease)
{
	int numVariablesExpected = 7;
	FILE *file;

	/* Test to see if file exists */
	if ((file = fopen(fileIn, "r")) == NULL)
	{
		fprintf(stderr, "Couldn't find a file to open\n");
		return EXIT_FAILURE;
	}

	/* Checks if fscanf was successful */
	int varAssigned = 0;
	varAssigned    += fscanf(file, "maxQueueLength %d\n", maxQueueLength);
	varAssigned    += fscanf(file, "numServicePoints %d\n", numServicePoints);
	varAssigned    += fscanf(file, "closingTime %d\n", closingTime);
	varAssigned    += fscanf(file, "maxServiceTime %d\n", maxServiceTime);
	varAssigned    += fscanf(file, "mean %lf\n", mean);
	varAssigned    += fscanf(file, "leaveChance %lf\n", leaveChance);
	varAssigned    += fscanf(file, "boredomChanceIncrease %lf\n", boredomChanceIncrease);

	if (varAssigned != numVariablesExpected)
	{
		fprintf(stderr, "Only %d out of %d were successfully assigned\n", varAssigned, numVariablesExpected);
		exit(-3);
	}

	fclose(file);

	return EXIT_SUCCESS;
}

/* Runs all of the code required to run the simulation */
void runSimulation(char* fileOut, int maxQueueLength, int closingTime, int numServicePoints, int maxServiceTime, double mean, double leaveChance, double increase, int numSims, int simNum){
	static int totalPeople      = 0;
	static int currentNumPeople = 0;
	static int unfulfilled      = 0;
	static int timedOut         = 0;
	static int totalServiceTime = 0;
	int numFree                 = 0;
	static int fulfilled        = 0;

	QUEUE *q = createQueue(maxQueueLength);

	/* Create a list of Service Points */
	SERVICE_POINT_LIST *serviceList = createServicePointList();

	int i;
	for (i = 1; i <= numServicePoints; i++)
	{

		SERVICE_POINT *temp = createServicePoint(i);

		addToServicePointList(serviceList, temp);

	}

	int time;
	for (time = 1; time <= closingTime; time++)
	{

		int numPeopleToAdd = getNumPeople(mean);
		currentNumPeople  += numPeopleToAdd;
		totalPeople       += numPeopleToAdd;

		serviceCurrentCustomers(maxServiceTime, mean, leaveChance, increase, &totalServiceTime, serviceList, q, &timedOut, &numFree, &fulfilled);

		/* Adding customers to the store */
		int i;
		for (i = 1; i <= numPeopleToAdd; i++)
		{	
			if (q->count >= q->maxLength)
			{
				unfulfilled++;
				currentNumPeople--;
				continue;
			}
				
			PERSON *newP = newPerson((i + currentNumPeople), leaveChance, maxServiceTime);

			enQueue(q, newP);
		}

			if (numSims == 1)
				writeToFile(fileOut, numSims, time, unfulfilled, fulfilled, timedOut, q->count, (numServicePoints - numFree), (time == closingTime), FALSE, FALSE, 0, totalServiceTime, (simNum == numSims));
	
	numFree = 0;
	}

		writeToFile(fileOut, numSims, time, unfulfilled, fulfilled, timedOut, q->count, (numServicePoints - numFree), (time == closingTime), TRUE, FALSE, 0, totalServiceTime, FALSE);
	
	int timeAfterClosing = 0;
	while (q->count > 0)
	{

		serviceCurrentCustomers(maxServiceTime, mean, leaveChance, increase, &totalServiceTime, serviceList, q, &timedOut, &numFree, &fulfilled);
		timeAfterClosing += 1;
		numFree = 0;
	}

	writeToFile(fileOut, numSims, time, unfulfilled, fulfilled, timedOut, q->count, (numServicePoints - numFree), (time == closingTime), FALSE, TRUE, timeAfterClosing, totalServiceTime, (simNum == numSims));
	

}

/* Performs most of the simulation, minus the part where customers are added to the post
 * off */
void serviceCurrentCustomers(int maxServiceTime, double mean, double leavechance, double increase, int* totalServiceTime, SERVICE_POINT_LIST* serviceList, QUEUE* q, int* timedOut, int* numFree, int* fulfilled)
{
	
	/* Checks if anyone at the service points - if there 
	 * are any - has finished */
	SERVICE_POINT_NODE *currentNode   = serviceList->front;
	SERVICE_POINT      *currentPoint  = currentNode->key;
	PERSON_LIST        *leavingPeople = createPersonList();

	while (currentNode)
	{
		if (!currentPoint->free)
		{
			PERSON* atService = currentPoint->personServicing;
			
			if (atService)
			{
				if (atService->timeAtPoint == atService->totalServiceTime)
				{
					addToPersonList(leavingPeople, atService);
					finishJob(currentPoint);
					*fulfilled += 1;
					*totalServiceTime += atService->totalServiceTime;
				}
				else
				{
					atService->timeAtPoint++;
				}
			}
		}

		currentNode = currentNode->previous;

		if (currentNode)
			currentPoint = currentNode->key;
		else
			currentPoint = NULL;
	}

	/* Checks if any service points are free */
	SERVICE_POINT_LIST *freePoints = createServicePointList();
	SERVICE_POINT_NODE *freeNode   = serviceList->front;
	SERVICE_POINT      *freePoint  = freeNode->key;
	while (freePoint)
	{
		if (freePoint->free)
		{
			*numFree += 1;
			addToServicePointList(freePoints, freePoint);
		}

		freeNode = freeNode->previous;

		if (freeNode)
			freePoint = freeNode->key;
		else
			freePoint = NULL;
	}

	/* Fill empty service points - so long as there are people available */
	if (freePoints->count > 0)
	{
		int j;
		freeNode  = freePoints->front;
		freePoint = freeNode->key;
		for (j = 0; j < *numFree; j++)
		{
			/* Checks if there are people in the queue */
			if (q->count == 0)
				break;

			PERSON *p = deQueue(q);

			attendPerson(freePoint, p);
			freeNode  = freeNode->previous;

			if (freeNode)
				freePoint = freeNode->key;
		}
	}

	/* Seeing if any customers want to leave the queue */
	PERSON* leaving = q->rear;

	if (leaving)
	{
		PERSON_LIST *timedOutPeople = createPersonList();

		while (leaving)
		{
			BOOLEAN leave = leaves(leaving->leaveChance);
	
			/* If the customer doesn't leave increase the chance that 
			*  they  will next time, if they do add them to the list 
			*  of people who will leave and remove them from the 
			*  queue */
			if (!leave)
				leaving->leaveChance += increase;
			else
			{
				addToPersonList(timedOutPeople, leaving);
				*timedOut += 1;
				removeFromQueue(q, leaving);
			}

			leaving = leaving->next;
		} 

		PERSON* tempLeaving = timedOutPeople->rear;
		while (tempLeaving)
		{
			PERSON* tempLeavingNext = tempLeaving->next;

			removePerson(timedOutPeople, tempLeaving);
			
			deletePerson(tempLeaving);
			tempLeaving = tempLeavingNext;
		}

		PERSON* satisfied = leavingPeople->rear;
		while (satisfied)
		{
			PERSON* tempSatisfied = satisfied->next;

			removePerson(leavingPeople, tempLeaving);

			deletePerson(satisfied);
			satisfied = tempSatisfied;
		}

	}
}

/* Takes in all of the vraibles that will be printed to a file for the simulation 
 * and printf them to the given file */
int writeToFile(char* fileOut, int numSims, int time, int unfulfilled, int fulfilled, int timedOut, int inQueue, int beingServed, BOOLEAN closingTime, BOOLEAN closed, BOOLEAN finished, int timeTakenToClose, int totalWaitTime, BOOLEAN finalSim)
{
	FILE *file;

	if (!(file = fopen(fileOut, "a+")))
	{
		fprintf(stderr, "Couldn't find a file to open\n");
		return EXIT_FAILURE;
	}

	if (numSims == 1)
	{	
		if (!closingTime)
			fprintf(file, "Current time interval: %d\n", time);
			fprintf(file, "There are %d customers currently being served\n", beingServed);
			fprintf(file, "There are %d customers currenlty in the queue\n", inQueue);
		fprintf(file, "There are %d unfulfilled customers, %d fulfilled customers and %d timed-out customers\n", unfulfilled, fulfilled, timedOut);
		
		if (closingTime && !closed)
			fprintf(file, "The post office has now closed\n");

		if (closed && !finished)
			fprintf(file, "It took %d intervals after closing to serve the remaining customers\n", timeTakenToClose);

		if (finished)
		{
			double averageWait = (double)totalWaitTime/(double)fulfilled;
			fprintf(file, "The average waiting time was %lf\n", averageWait);
		}
	}
	else
	{
		if (finalSim)
		{
			fprintf(file, "On average, there were %lf fulfilled customers\n", (double)fulfilled/(double)numSims);
			fprintf(file, "On average, there were %lf unfulfilled customers\n", (double)unfulfilled/(double)numSims);
			fprintf(file, "On average, there were %lf timed-out customers\n", (double)timedOut/(double)numSims);
			fprintf(file, "On average, the average wait time was %lf\n", (double)totalWaitTime/(double)fulfilled);
		}

	}
	
	fclose(file);

	return EXIT_SUCCESS;
}
