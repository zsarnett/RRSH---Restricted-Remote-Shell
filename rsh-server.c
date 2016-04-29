#include "csapp.h"
#include "rshell.h"

void login(int connfd, char *client_hostname, char *client_port, char *username, char *password);
void runcommands(int connfd, char *username);

void Execv(const char *filename, char *const argv[])
{
    if (execv(filename, argv) < 0)
        unix_error("Execv error");
}


int main(int argc, char **argv) 
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  
    char client_hostname[MAXLINE], client_port[MAXLINE], username[MAXLINE], password[MAXLINE];

    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }

    listenfd = Open_listenfd(argv[1]); //Listen for connections
    while (1) {
	clientlen = sizeof(struct sockaddr_storage); 
	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
	login(connfd, client_hostname, client_port, &username[0], &password[0]);
	runcommands(connfd, &username[0]);
	Close(connfd);
    }
    exit(0);
}

void login(int connfd, char *client_hostname, char *client_port, char *username, char *password)
{
	char buf[MAXLINE];
	rio_t rio;
	FILE* fp;
	int FLAG = 0;
	struct command *login;
	char *loginsucceed = "Login Approved\n";
	char *loginfail = "Login Failed\n";

	Rio_readinitb(&rio, connfd);
	fp = Fopen("rrshusers.txt", "rt");
	Rio_readlineb(&rio, username, MAXLINE);

	printf("User %s logging in from %s at TCP port %s.\n", strtok(username, "\n"), client_hostname, client_port);

	Rio_readlineb(&rio, password, MAXLINE);
	
	while((Fgets(buf, MAXLINE, fp) != NULL) && FLAG == 0)
	{
		login = parse_command(buf);

		if(strcmp(strtok(login->args[0], "\n"), strtok(username, "\n")) == 0)
                {

			if(strcmp(strtok(password,"\n"), strtok(login->args[1], "\n")) == 0)
                        {
				printf("Username %s logged in successfully!\n",  strtok(username, "\n"));
                        	Rio_writen(connfd, loginsucceed, strlen(loginsucceed));
                                FLAG = 1;
                        }
		}	
	}

	if(FLAG == 0)
	{
		printf("Username %s denied access.\n", strtok(username, "\n"));
		Rio_writen(connfd, loginfail, strlen(loginfail));
	}

	return;
}

void runcommands(int connfd, char *username)
{
	FILE* fp;
	char Command[MAXLINE], buf[MAXLINE];	
	int FLAG = 0;
	struct command *cmd, *file;
	rio_t rio;
	char *Complete = "RRSH COMMAND COMPLETED\n";
	char *quit = "quit";

	Rio_readinitb(&rio, connfd);
	fp = Fopen("rrshcommands.txt", "rt");

	while(Rio_readlineb(&rio, Command, MAXLINE) > 0)
	{
		FLAG = 0;
		cmd = parse_command(Command);

		if(strcmp(strtok(cmd->args[0], "\n"), quit) == 0)
		{
			printf("User %s disconnected.\n", username);
			break;
		}

		printf("User %s sent the command %s to be executed.\n", username,strtok(Command, "\n"));

		while((Fgets(buf, MAXLINE, fp) != NULL)  && FLAG == 0)
		{
			file = parse_command(buf);

			if(strcmp(strtok(file->args[0],"\n"), strtok(cmd->args[0],"\n")) == 0 )
			{
				printf("Executing the command %s on behalf of %s.\n", strtok(Command, "\n"), username);
				FLAG = 1;
			}
		}

		if(FLAG == 1)
		{
			pid_t pid = Fork();
			
			if(pid == 0)
			{
				int fd = Open("/dev/null", 0, O_RDONLY);

				dup2(fd, 0);
				dup2(connfd, 1);
				dup2(connfd, 2);
				close(fd);

				Execv(cmd->args[0], cmd->args);
			}
			else
			{
				int status;
				Waitpid(pid, &status, 0);

				Rio_writen(connfd, Complete, strlen(Complete));
			}
		}

		else
		{
			printf("The command %s is not allowed\n", strtok(Command, "\n"));

			char message[MAXLINE];
			sprintf(message, "Cannot execute %s on this server\n",cmd->args[0]);
			Rio_writen(connfd, message, strlen(message));

			Rio_writen(connfd, Complete, strlen(Complete));
		}
	}
	return;
}
