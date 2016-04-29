#include "csapp.h"
#include "rshell.h"


int main(int argc, char **argv) 
{
    int clientfd;
    char *host, *port, buf[MAXLINE], receive[MAXLINE];
    rio_t rio;
    char *login = "Login Approved";
    char *Complete = "RRSH COMMAND COMPLETED\n";
    char *quit = "quit\n";
    if (argc != 3) {
	fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
	exit(0);
    }
    host = argv[1];
    port = argv[2];

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    Fputs("Username: ", stdout);
    Fgets(buf, MAXLINE, stdin);
    Rio_writen(clientfd, buf, strlen(buf));
    
    Fputs("Password: ", stdout);
    Fgets(buf, MAXLINE, stdin);
    Rio_writen(clientfd, buf, strlen(buf));
    
    Rio_readlineb(&rio, receive, MAXLINE);
    Fputs(receive, stdout); 
    if(strcmp(login, strtok(receive,"\n")))
    {
	Close(clientfd);
	exit(125);
    }
    while(1)
    {
    	Fputs("rrsh> ", stdout);
	Fgets(buf, MAXLINE, stdin);
	
	if(strcmp(buf, quit) == 0)
	{
		Rio_writen(clientfd, quit, strlen(quit));
		break;
	}
		
	if(strcmp(buf, "\n") == 0)
	{
		continue;
	}
	
	Rio_writen(clientfd, buf, strlen(buf));
	Rio_readlineb(&rio, buf, MAXLINE);

	while(strcmp(buf, Complete))
	{
		Fputs(buf,stdout);
		Rio_readlineb(&rio, buf, MAXLINE);
	}
    }	 
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
