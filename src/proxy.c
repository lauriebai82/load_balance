#include "lb.h"
#include "link.h"

static int    servern;
static struct server serverlist[MAX_SERVER];
static Link   connlink;

void sig_child(int signo)
{
	pid_t pid;
	int   i, stat;

	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
	{
		Node *node = link_delete(&connlink, pid);
		for (i = 0; i < servern; i++)
		{
			if (serverlist[i].port == node->port && strcmp(serverlist[i].ip, node->ip) == 0)
			{
				serverlist[i].conn -= 1;
				break;
			}
		}
	}
	return;
}

void printconnlist()
{
	int i;

	for (i = 0; i < servern; i++)
		printf("%d\t", serverlist[i].conn);
	printf("\n");
}

int main(int argc, char *argv[])
{
	int i;
	
	servern = servlist(serverlist); //get server list
	link_init(&connlink);			//init conn link, record (childpid, server)
	
	//listen client and sent request to least balance's server
	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PROXY_PORT);
	
	if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_quit("error : server bind");
	listen(listenfd, LISTENQ);
	Signal(SIGCHLD, sig_child);

	for (; ;)
	{
		int  min, n;
		char response[MAX_LINE];

		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (SA *) &cliaddr, &clilen);
		
		for (i = 0, min = 0; i < servern; i++)
		{
			if (serverlist[i].conn < serverlist[min].conn)
				min = i;
		}

		if ((childpid = fork()) < 0)
			err_quit("error : fork chile process");
		else if (childpid == 0)
		{
			close(listenfd);
			if (getresponse(serverlist[min].ip, serverlist[min].port, response) < 0)
				sprintf(response, "error : get response from %s:%d", serverlist[min].ip, serverlist[min].port);
			write(connfd, response, strlen(response));
			close(connfd);
			exit(0);
		}
		//record childpid
		else
		{
			//add server conn
			serverlist[min].conn += 1;
			link_insert(&connlink, childpid, serverlist[min].ip, serverlist[min].port);
			printconnlist();
			close(connfd);
		}
	}
	close(listenfd);

	return 0;
}
