#ifndef __LINK_H__
#define __LINK_H__

#include "lb.h"

typedef struct childconn
{
	char  *ip;
	int   port;
	pid_t pid;
	struct childconn *next;
} Node;

typedef struct connlist
{
	int  total;
	Node *head;
} Link;

void link_init  (Link *);
Node *link_insert(Link *, pid_t, char *, int);
Node *link_delete(Link *, pid_t);
void link_traverse(Link *);

#endif
