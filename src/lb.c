#include "lb.h"

void err_quit(char *str)
{
	printf("%s\n", str);
	exit(0);
}

int servlist(struct server *servlist)
{
	FILE *fp;
	int  port, index;
	char servline[MAX_LINE];
	char ip[MAX_LINE], ports[MAX_LINE];
	
	if ((fp = fopen(SERVLIST_CONF, "r")) == NULL)
		err_quit("error : open servlist.conf");
	index = 0;
	while (fgets(servline, MAX_LINE, fp) != NULL)
	{
		int i, j;
		struct server serv;

		i = j = 0;
		while (servline[i] != ':')
			ip[i++] = servline[i];
		ip[i++] = '\0';
		while (servline[i] != '\0' && servline[i] != '\n')
			ports[j++] = servline[i++];
		ports[j] = '\0';
		serv.port = atoi(ports);
		serv.conn = 0;
		serv.ip = malloc(sizeof(char) * MAX_IP_LEN);
		strcpy(serv.ip, ip);
		servlist[index++] = serv;
	}

	return index;
}

int getresponse(char *ip, int port, char *response)
{
	int    sockfd, n;
	struct sockaddr_in servaddr;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &servaddr.sin_addr);
	
	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_quit("error : client connect");
	n = read(sockfd, response, MAX_LINE);
	close(sockfd);
	
	return n;
}

Sigfunc *Signal(int signo, Sigfunc *func)
{
	struct sigaction act, oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (signo == SIGALRM)
	{
#ifdef SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;
#endif
	}
	else 
	{
#ifdef SA_RESTART
		act.sa_flags |= SA_RESTART;
#endif
	}
	if (sigaction(signo, &act, &oact) < 0)
		return SIG_ERR;

	return oact.sa_handler;
}
