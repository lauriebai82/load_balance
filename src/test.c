#include "lb.h"

int main()
{
	int    i, servern, curport;
	char   cmd[MAX_LINE];
	struct server serverlist[MAX_SERVER];
	
	servern = servlist(serverlist);
	for (i = 0; i < servern; i++)
	{
		sprintf(cmd, "./server %d &", serverlist[i].port);
		if (system(cmd) < 0)
			err_quit("error : start server");
	}

	//start proxy server
	sprintf(cmd, "./proxy > conn_num.txt &");
	if (system(cmd) < 0)
		err_quit("error : start proxy server");

	//start a lot of clients
	for (i = 0; i < 100; i++)
	{
		if (system("./client >> result &") < 0)
			err_quit("error : start client");
	}

	return 0;
}
