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
#include <math.h>

#define MAXDATASIZE 100 

void severAprocess(void);
void severBprocess(void);
void severCprocess(void);

int main(int argc, char *argv[])
{
	pid_t  pid;
	pid=fork();
	if (pid > 0){
		severAprocess(); // start up the s
	}else if(pid == 0){
		pid=fork();
		if (pid>0){
			severBprocess(); // start up the severB
		}else if (pid == 0){
			severCprocess(); // start up the severC
		}
	}
	return 0;
}

void severAprocess(){

	//I use the naming of some variables from 6.3 Datagram Sockets of Beej's tutorial socket.
	int len;
	int sockfd;
	struct sockaddr_storage their_addr;
	socklen_t addr_len;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	char buf[MAXDATASIZE];
	
	memset(&hints, 0, sizeof hints);

	//This part I used are from 6.3 Datagram Sockets of Beej's tutorial socket.
	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = SOCK_DGRAM; // set the socket type as UDP Datagram sockets
	hints.ai_flags = AI_PASSIVE;

	//generate the address information
	getaddrinfo("127.0.0.1", "21846", &hints, &servinfo);
	// make a UDP Datagram sockets
	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if(sockfd==-1){
		printf("socket generation error!\n");
		exit(1);
	}

	// bind the port number to UDP socket
	int status = bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
	if(status==-1){
		printf("bind error!\n");
		exit(1);
    }

	freeaddrinfo(servinfo);

	printf("The Server A is up and running using UDP on port 21846.\n");

	while(1)
	{
		//receive the value from AWS
		addr_len = sizeof their_addr;
		float input;
		recvfrom(sockfd, &input, sizeof input , 0,(struct sockaddr *)&their_addr, &addr_len);
		printf("The Server A received <%g>\n",input);
		
		//compute the value's 2th pow
		input = pow(input,2);
		printf("The Server A calculated square: <%g>\n",input);

		//send the computed value back to the AWS
		sendto(sockfd, &input, sizeof input, 0,(struct sockaddr *)&their_addr, addr_len);
		printf("The Server A finished sending the output to AWS\n");
	}
	close(sockfd);
}

void severBprocess(){

	//I use the naming of some variables from 6.3 Datagram Sockets of Beej's tutorial socket.
	int len;
	int sockfd;
	struct sockaddr_storage their_addr;
	socklen_t addr_len;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	char buf[MAXDATASIZE];

	memset(&hints, 0, sizeof hints);

	//This part I used are from 6.3 Datagram Sockets of Beej's tutorial socket.
	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = SOCK_DGRAM; // set the socket type as UDP Datagram sockets
	hints.ai_flags = AI_PASSIVE;

	//generate the address information
	getaddrinfo("127.0.0.1", "22846", &hints, &servinfo);
	// make a UDP Datagram sockets
	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if(sockfd==-1){
		printf("socket generation error!\n");
		exit(1);
	}

	// bind the port number to UDP socket
	int status = bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
	if(status==-1){
		printf("bind v error!\n");
		exit(1);
    }

	freeaddrinfo(servinfo);

	printf("The Server B is up and running using UDP on port 22846.\n");

	while(1)
	{
		//receive the value from AWS
		addr_len = sizeof their_addr;
		float input;
		recvfrom(sockfd, &input, sizeof input , 0,(struct sockaddr *)&their_addr, &addr_len);
		printf("The Server B received <%g>\n",input);
		
		//compute the value's 3th pow
		input = pow(input,3);
		printf("The Server B calculated cube: <%g>\n",input);

		//send the computed value back to the AWS		
		sendto(sockfd, &input, sizeof input, 0,(struct sockaddr *)&their_addr, addr_len);
		printf("The Server B finished sending the output to AWS\n");
	}


	close(sockfd);
}

void severCprocess(){

	//I use the naming of some variables from 6.3 Datagram Sockets of Beej's tutorial socket.
	int len;
	int sockfd;
	struct sockaddr_storage their_addr;
	socklen_t addr_len;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	char buf[MAXDATASIZE];

	memset(&hints, 0, sizeof hints);

	//This part I used are from 6.3 Datagram Sockets of Beej's tutorial socket.
	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = SOCK_DGRAM; // set the socket type as UDP Datagram sockets
	hints.ai_flags = AI_PASSIVE;

	//generate the address information
	getaddrinfo("127.0.0.1", "23846", &hints, &servinfo);

	// make a UDP Datagram sockets
	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if(sockfd==-1){
		printf("socket generation error!\n");
		exit(1);
	}

	// bind the port number to UDP socket
	int status = bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
	if(status==-1){
		printf("bind error!\n");
		exit(1);
    }

	freeaddrinfo(servinfo);

	printf("The Server C is up and running using UDP on port 23846.\n");

	while(1)
	{
		//receive the value from AWS
		addr_len = sizeof their_addr;
		float input;
		recvfrom(sockfd, &input, sizeof input , 0,(struct sockaddr *)&their_addr, &addr_len);
		printf("The Server C received <%g>\n",input);
		
		//compute the value's 5th pow
		input = pow(input,5);
		printf("The Server C calculated 5th power: <%g>\n",input);

		//send the computed value back to the AWS
		sendto(sockfd, &input, sizeof input, 0,(struct sockaddr *)&their_addr, addr_len);
		printf("The Server C finished sending the output to AWS\n");
	}

	close(sockfd);

}


