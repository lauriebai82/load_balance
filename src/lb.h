#ifndef __LB_H__
#define __LB_H__

#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERV_IP		"127.0.0.1"
#define PROXY_IP	"127.0.0.1"
#define PROXY_PORT	8282
#define SERVER_PORT 8283
#define MAX_LINE	1024
#define LISTENQ		100
#define MAX_SERVER	10
#define MAX_IP_LEN	100
#define SERVLIST_CONF "/home/littlewhite/Code/load_blance/src/serverlist.conf"
#define SA struct sockaddr

typedef void Sigfunc(int);

struct server
{
	int  port;
	int  conn;
	char *ip;
};

int  servlist(struct server *);
int  getresponse(char *, int, char *);
void err_quit(char *msg);
Sigfunc *Signal(int, Sigfunc *);

#endif
