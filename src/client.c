#include "lb.h"

void str_echo(int sockfd);

int main (int argc, char *argv[])
{
	char response[MAX_LINE];

	if (getresponse(PROXY_IP, PROXY_PORT, response) < 0)
		err_quit("error : client get server response");
	printf("%s", response);
	
	return 0;
}
