#include "util_queue.h"

/* ------------------ FUNCTIONS */
/* Creates a new queue */
extern QUEUE* createQueue(int max)
{
	QUEUE *q;

	if (!(q = (QUEUE*)malloc(sizeof(QUEUE))))
	{
		fprintf(stderr, "Insufficient memory/n");
		exit(-2);
	}

	q->front     = q->rear = NULL;
	q->count     = 0;
	q->maxLength = max;
	return q;
}

/* Adds the person p the rear of the queue q */
extern void enQueue(QUEUE* q, PERSON* p)
{
	/* Check if the queue has already reached its
	   max length */
	if (q->count == q->maxLength)
		return;

	if (q->rear == NULL && q->front == NULL)
	{
		q->front = q->rear = p;
		return;
	}

	PERSON* temp   = q->rear;
	q->rear        = p;
	p->next        = temp;
	p->previous    = NULL;
	temp->previous = p;
	q->count++;
}

/* Removes the first person from the queue q and returns them */
extern PERSON* deQueue(QUEUE *q)
{
	if (!q->front)
		return;

	PERSON *p = q->front;
	q->front  = q->front->previous;

	if (!q->front)
		q->rear = NULL;
	else
		q->front->next = NULL;

	q->count--;
	return p;
}

/* Remove a given person from the queue */
extern void removeFromQueue(QUEUE* q, PERSON* p)
{
	if (!(q->front))
		return;
	
	if (p)
	{
		PERSON *next = p->next;
		PERSON *prev = p->previous;

		if (next)
			next->previous = prev;
		else
			q->front = prev;
	
		if (prev)
			prev->next = next;
		else
			q->rear = next;

		p->next     = NULL;
		p->previous = NULL;
		q->count--;
	}
}
