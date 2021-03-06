#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXDATASIZE 100 

int main(int argc, char *argv[2])
{
	//I use the naming of some variables from 6.1 A Simple Stream sever of Beej's tutorial socket.
	int len;
	int status;
	int sockfd;
	struct sockaddr_storage their_addr;
    socklen_t sin_size;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	char buf[MAXDATASIZE];

	memset(&hints, 0, sizeof hints);	//make sure the struct is empty

	//This part I used are from 6.1 A Simple Stream sever of Beej's tutorial socket.
	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = SOCK_STREAM; // set the socket type as TCP stream sockets
	hints.ai_flags = AI_PASSIVE;

	//generate the address information
	getaddrinfo("127.0.0.1", "25846", &hints, &servinfo);

	// make a TCP socket
	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	//connect the socket to the TCP socket generated by the AWS
	if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1){
		perror("client: connect");
	}
	freeaddrinfo(servinfo);

	printf("The client is up and running\n");

	len = strlen(argv[1]);
	send(sockfd, argv[1], len, 0);// send the function to AWS
	
	len = strlen(argv[2]);
	send(sockfd, argv[2], len, 0);// send the value to AWS

	printf("The client sent <%s> and %s to AWS\n",argv[2],argv[1]);

	int numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0);// receive the final result from AWS
	buf[numbytes] = '\0';
	printf("According to AWS, %s on <%s>: <%s>\n",argv[1],argv[2],buf);


	close(sockfd);
	return 0;
}




