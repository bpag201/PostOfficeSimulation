#include "util_linked_list.h"

/* Service Point Linked List */
extern SERVICE_POINT_NODE* newServicePointNode(SERVICE_POINT* s)
{
	SERVICE_POINT_NODE* temp;
	
	if (!(temp = (SERVICE_POINT_NODE*)malloc(sizeof(SERVICE_POINT_NODE))))
	{
		fprintf(stderr, "Insufficient memory\n");
		exit(-2);
	}

	temp->key      = s;
	temp->next     = NULL;
	temp->previous = NULL;

	return temp;
}

extern void deleteServicePointNode(SERVICE_POINT_NODE* n)
{
	if (!n)
		return;

	n->next     = NULL;
	n->previous = NULL;
	n->key      = NULL;
	free(n);
	n           = NULL;
}

extern SERVICE_POINT_LIST* createServicePointList()
{
	SERVICE_POINT_LIST *l;

	if (!(l = (SERVICE_POINT_LIST*)malloc(sizeof(SERVICE_POINT_LIST))))
	{
		fprintf(stderr, "Insufficient memory\n");
		exit(-2);
	}

	l->front = l->rear = NULL;
	l->count = 0;
	return l;
}

extern void addToServicePointList(SERVICE_POINT_LIST* l, SERVICE_POINT* s)
{
	SERVICE_POINT_NODE *temp = newServicePointNode(s);

	if (l->rear == NULL)
	{
		l->front = l->rear = temp;
		return;
	}

	SERVICE_POINT_NODE *n = l->rear;
	l->rear               = temp;
	temp->next            = n;
	temp->previous        = NULL;
	n->previous           = temp;
	l->count             += 1;
}

extern void removeServicePoint(SERVICE_POINT_LIST* l, SERVICE_POINT* s)
{
	SERVICE_POINT_NODE *temp = findServicePointNode(l, s);

	if(!temp)
	{
		fprintf(stderr, "Node not found\n");
		exit(-6);
	}

	SERVICE_POINT_NODE *next     = temp->next;
	SERVICE_POINT_NODE *previous = temp->previous;

	if (next)
		next->previous = previous;
	else
		next->previous = NULL;

	if (previous)
		previous->next = next;
	else
		previous->next = NULL;

	deleteServicePointNode(temp);
}

extern SERVICE_POINT_NODE* findServicePointNode(SERVICE_POINT_LIST* l, SERVICE_POINT* s)
{
	/* Check that the list exists */
	if (!l)
	{
		fprintf(stderr, "List doesn't exist\n");
		exit(-4);
	}

	/* Check that the list contains at least one item */
	if (!l->rear)
	{
		fprintf(stderr, "List doesn't contain any items\n");
		exit(-5);
	}

	SERVICE_POINT_NODE *n = l->rear;

	while(n)
	{
		if(n->key == s)
			return n;
	}

	return NULL;
}

/* Person Linked List */
extern PERSON_LIST* createPersonList()
{
	PERSON_LIST *l;

	if (!(l = (PERSON_LIST*)malloc(sizeof(PERSON_LIST))))
	{
		fprintf(stderr, "Insufficient memory\n");
		exit(-2);
	}

	l->front = l->rear = NULL;
	l->count = 0;
	return l;
}

extern void addToPersonList(PERSON_LIST* l, PERSON* p)
{
	if (p)
	{
		if (l->rear == NULL)
		{
			l->front = l->rear = p;
			return;
		}
	
		PERSON *temp   = l->rear;
		l->rear        = p;
		p->next        = temp;
		p->previous    = NULL;
		temp->previous = p;
		l->count      += 1;
	}
}

extern void removePerson(PERSON_LIST* l, PERSON* p)
{
	if (p)
	{
		PERSON *next     = p->next;
		PERSON *previous = p->previous;

		if (next)
			next->previous = previous;

		if (previous)
			previous->next = next;
		
		p->next     = NULL;
		p->previous = NULL;
	}
} 
