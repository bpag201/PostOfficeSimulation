#ifndef __UTIL_LINKED_LIST_H
#define __UTIL_LINKED_LIST_H

#include "util_person.h"
#include "util_service_point.h"

/* ----------------------------- STRUCTURE */
typedef struct servicePointNode {
	SERVICE_POINT           *key;
	struct servicePointNode *next;
	struct servicePointNode *previous;
} SERVICE_POINT_NODE;

typedef struct servicePointLinkedList {
	int                 count;
	SERVICE_POINT_NODE *front;
	SERVICE_POINT_NODE *rear;
} SERVICE_POINT_LIST;

typedef struct personLinkedList {
	int     count;
	PERSON *front;
	PERSON *rear;
} PERSON_LIST;

/* ----------------------------- PROTOTYPES */
extern SERVICE_POINT_NODE* newServicePointNode(SERVICE_POINT*);
extern void                deleteServicePointNode(SERVICE_POINT_NODE*);
extern SERVICE_POINT_LIST* createServicePointList();
extern void                addToServicePointList(SERVICE_POINT_LIST*, SERVICE_POINT*);
extern void                removeServicePoint(SERVICE_POINT_LIST*, SERVICE_POINT*);
extern SERVICE_POINT_NODE* findServicePointNode(SERVICE_POINT_LIST*, SERVICE_POINT*);

extern PERSON_LIST* createPersonList();
extern void         addToPersonList(PERSON_LIST*, PERSON*);
extern void         removePerson(PERSON_LIST*, PERSON*);

#endif
