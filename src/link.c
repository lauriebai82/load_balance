#include "link.h"

void link_init(Link *link)
{
	link->head = malloc(sizeof(Node));
	if (link->head == NULL)
		err_quit("error : init pid_ipport_link");
	link->head->next = NULL;
}

Node *link_insert(Link *link, pid_t pid, char *ip, int port)
{
	Node *node = malloc(sizeof(Node));
	node->ip   = malloc(sizeof(MAX_IP_LEN));
	
	if (node == NULL || node->ip == NULL)
		err_quit("error : malloc new Node");
	
	strcpy(node->ip, ip);
	node->pid  = pid;
	node->port = port;
	node->next = link->head->next;
	link->head->next = node;

	return node;
}

Node *link_delete(Link *link, pid_t pid)
{
	Node *pre = link->head;
	Node *cur = link->head->next;
	
	while (cur != NULL)
	{
		if (cur->pid == pid)
		{
			pre->next = cur->next;
			return cur;
		}
		pre = cur;
		cur = cur->next;
	}
	
	return NULL;
}

void link_traverse(Link *link)
{
	Node *node = link->head->next;
	while (node != NULL)
	{
		printf("IP : %s , Port : %d , pid : %d\n", node->ip, node->port, node->pid);
		node = node->next;
	}
}
