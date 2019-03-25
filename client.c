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
    int sockfd, portno, n, i;
    char buffer[255];
	FILE *f;
	int words;
	char c;

    struct sockaddr_in serv_addr;
    struct hostent *server;
	words = 0;
    if(ac < 3)
    {
        fprintf(stderr,"useage %s hostname port\n", av[0]);
        exit(1);
    }
    
	portno = atoi(av[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        error("error opening socket");

	server = gethostbyname(av[1]);
	if(server == NULL)
		fprintf(stderr, " error no host yo");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if(connect(sockfd, ( struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
		error("connection failed");
	while(1)
	{
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		n = write(sockfd, buffer, strlen(buffer));
		if(n < 0)
			error("error ono writing");

		bzero(buffer, 255);
		n = read(sockfd, buffer, 255);
		if(n < 0)
			error("error on read");
		printf("server: %s", buffer);

		i = strncmp("bye", buffer, 3);
		if(i == 0)
			break;
	}

    f = fopen("hello.txt" , "r");
    while((c = getc(f)) != EOF)
{
	fscanf(f,"%s", buffer);
	if(isspace(c) || c == '\t')
		words++;
}

write(sockfd, &words, sizeof(int));
rewind(f);

char ch;
while(ch != EOF)
{
    fscanf(f, "%s", buffer);
    write(sockfd, buffer, 255);
    ch = fgetc(f);
}
printf("files ok");
	close(sockfd);
	return(0);
}
