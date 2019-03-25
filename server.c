#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>

void error(const char *error)
{
	perror(error);
	exit(1);
}

int main(int ac, char **av)
{
	int sockfd, newsockfd, portno, n, i;
	char buff[255];
	FILE *fp;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;//32 bit

	if(ac < 2)
	{
		fprintf(stderr,"provide port plox");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		error("error opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(av[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("binding failed");

	listen(sockfd, 2);
	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if(newsockfd < 0)
		error("not accepted");

	while(1)
	{
		bzero(buff, 25);
		n = read(newsockfd, buff, 255);
		printf("client: %s\n", buff);
		bzero(buff, 25);
		fgets(buff, 255, stdin);

		n = write(newsockfd, buff, strlen(buff));
		if(n < 0)
			error("did not write");
		i = strncmp("bye", buff, 3);
		if(i == 0)
			break;
	}

	int ch = 0;
	fp = fopen("hello recived", "a");
	int words;

	read(newsockfd, &words, sizeof(int));

	while(ch != words)
	{
		read(newsockfd, buff, 255);
		fprintf(fp, "%s ", buff);
		ch++;
	}

	printf("files good man");
	close(newsockfd);
	close(sockfd);
	return(0);
}
