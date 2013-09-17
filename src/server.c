#include "lb.h"

void serv_echo(int, char *, int);
void sig_child(int);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("usage : %s Port\n", argv[0]);
		return -1;
	}

	int listenfd, connfd, port;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	port = atoi(argv[1]);
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_quit("error : server bind");
	listen(listenfd, LISTENQ);
	Signal(SIGCHLD, sig_child);

	for (; ;) 
	{
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (SA *) &cliaddr, &clilen);
		
		if ((childpid = fork()) == 0) 
		{
			close(listenfd);
			sleep(1);
			serv_echo(connfd, SERV_IP, port);
			close(connfd);
			exit(0);
		}
		close(connfd);
	}
	close(listenfd);
}

void serv_echo(int fd, char *ip, int port)
{
	time_t timer;
	char line[MAX_LINE];

	timer = time(NULL);
	snprintf(line, MAX_LINE, "IP   : %s\nPort : %d\nTime : %s\r\n", 
			ip, port, ctime(&timer));
	write(fd, line, strlen(line));
}

void sig_child(int signo)
{
	pid_t pid;
	int   stat;

	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child process %d terminated\n", pid);
}
