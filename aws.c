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


int main(int argc, char *argv[])
{

//I use the naming of some variables from 6.3 A Simple Stream sever of Beej's tutorial socket.
struct sockaddr_storage their_addr;
struct sockaddr_in severs;
socklen_t sin_size,addr_len,len;
struct addrinfo hints, *res, *resA, *resB, *resC, *resAWS;
int sockfd, new_fd, sock,flag;
char buf[MAXDATASIZE],function[10];
float middleResults[7],finalResult;

memset(&hints, 0, sizeof hints); //make sure the struct is empty

//This part I used are from 6.1 A Simple Stream sever of Beej's tutorial socket.
hints.ai_family = AF_UNSPEC; 
hints.ai_socktype = SOCK_STREAM; // set the socket type as TCP stream sockets
hints.ai_flags = AI_PASSIVE;

//generate the address information
getaddrinfo(NULL, "25846", &hints, &res);

// make a TCP socket
sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
if(sockfd==-1){
	printf("socket generation error!\n");
	exit(1);
}

int yes=1;
setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes);

// bind the port number to TCP socket
int status = bind(sockfd, res->ai_addr, res->ai_addrlen);
if(status==-1){
	printf("bind TCP error!\n");
	exit(1);
}
// listen on the TCP socket
listen(sockfd, 10);

memset(&hints, 0, sizeof hints);

hints.ai_family = AF_UNSPEC; 
hints.ai_socktype = SOCK_DGRAM;  // set the socket type as UDP Datagram sockets
hints.ai_flags = AI_PASSIVE;

//generate the address information
getaddrinfo("127.0.0.1", "21846", &hints, &resA);
getaddrinfo("127.0.0.1", "22846", &hints, &resB);
getaddrinfo("127.0.0.1", "23846", &hints, &resC);
getaddrinfo("127.0.0.1", "24846", &hints, &resAWS);


// make a UDP socket
sock = socket(resA->ai_family, resA->ai_socktype, resA->ai_protocol);
if(sock==-1){
	printf("socket generation error!\n");
	exit(1);
}

// bind the port number to UDP socket
status = bind(sock, resAWS->ai_addr, resAWS->ai_addrlen);
	if(status==-1){
		printf("bind UDP error!\n");
		exit(1);
    }
printf("The AWS is up and running \n");

	while(1) { 

		sin_size = sizeof their_addr;

		// accept the connection with the client and generate the child packet discriptor	
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
		perror("accept");	
        }

        //receive the function type from the client
		int numbytes = recv(new_fd, buf, 3, 0);
		buf[numbytes] = '\0';
		strcpy(function,buf);
		
		//receive the input value from the client
		int num2bytes = recv(new_fd, buf, MAXDATASIZE-1, 0);
		buf[num2bytes] = '\0';
		float input= atof(buf);
		middleResults[1]=input;
		printf("The AWS received <%g> and function=<%s> from the client using TCP over port <25846> \n",input,function);

		//send the input value to the severA
		sendto(sock, &input, sizeof input, 0,
		resA->ai_addr, resA->ai_addrlen);
		printf("The AWS sent < %g > to Backend-Server A\n",input);

		//send the input value to the severB
		sendto(sock, &input, sizeof input, 0,
		resB->ai_addr, resB->ai_addrlen);
		printf("The AWS sent < %g > to Backend-Server B\n",input);

		//send the input value to the severC
		sendto(sock, &input, sizeof input, 0,
		resC->ai_addr, resC->ai_addrlen);
		printf("The AWS sent < %g > to Backend-Server C\n",input);


		//receive the calculated value and judge which sever the value comes from by port number
		float result;
		addr_len = sizeof their_addr;
		recvfrom(sock, &result, sizeof result, 0,(struct sockaddr *)&severs, &addr_len);
		int port = ntohs(severs.sin_port);
		switch(port) {
	      	case 21846 : middleResults[2]=result; printf("The AWS received < %g > Backend-Server <A> using UDP over port < 24846 >\n",result); break;
	      	case 22846 : middleResults[3]=result; printf("The AWS received < %g > Backend-Server <B> using UDP over port < 24846 >\n",result); break;
	     	case 23846 : middleResults[5]=result; printf("The AWS received < %g > Backend-Server <C> using UDP over port < 24846 >\n",result); break;      
   		}

   		//receive the calculated value and judge which sever the value comes from by port number
		addr_len = sizeof their_addr;
		recvfrom(sock, &result, sizeof result, 0,(struct sockaddr *)&severs, &addr_len);
		port = ntohs(severs.sin_port);
		switch(port) {
	      	case 21846 : middleResults[2]=result; printf("The AWS received < %g > Backend-Server <A> using UDP over port < 24846 >\n",result); break;
	      	case 22846 : middleResults[3]=result; printf("The AWS received < %g > Backend-Server <B> using UDP over port < 24846 >\n",result); break;
	     	case 23846 : middleResults[5]=result; printf("The AWS received < %g > Backend-Server <C> using UDP over port < 24846 >\n",result); break;      
   		}

   		//receive the calculated value and judge which sever the value comes from by port number
		addr_len = sizeof their_addr;
		recvfrom(sock, &result, sizeof result, 0,(struct sockaddr *)&severs, &addr_len);
		port = ntohs(severs.sin_port);
		switch(port) {
	      	case 21846 : middleResults[2]=result; printf("The AWS received < %g > Backend-Server <A> using UDP over port < 24846 >\n",result); break;
	      	case 22846 : middleResults[3]=result; printf("The AWS received < %g > Backend-Server <B> using UDP over port < 24846 >\n",result); break;
	     	case 23846 : middleResults[5]=result; printf("The AWS received < %g > Backend-Server <C> using UDP over port < 24846 >\n",result); break;      
   		}

   		//send the the value of input^2 to the severA
		sendto(sock, &middleResults[2], sizeof middleResults[2], 0,
		resA->ai_addr, resA->ai_addrlen);
		printf("The AWS sent < %g > to Backend-Server A\n",middleResults[2]);

		//send the the value of input^2 to the severB
		sendto(sock, &middleResults[2], sizeof middleResults[2], 0,
		resB->ai_addr, resB->ai_addrlen);
		printf("The AWS sent < %g > to Backend-Server B\n",middleResults[2]);

		//receive the calculated value and judge which sever the value comes from by port number
		addr_len = sizeof their_addr;
		recvfrom(sock, &result, sizeof result, 0,(struct sockaddr *)&severs, &addr_len);
		port = ntohs(severs.sin_port);
		switch(port) {
	      	case 21846 : middleResults[4]=result; printf("The AWS received < %g > Backend-Server <A> using UDP over port < 24846 >\n",result); break;
	      	case 22846 : middleResults[6]=result; printf("The AWS received < %g > Backend-Server <B> using UDP over port < 24846 >\n",result); break;
	    }

	    //receive the calculated value and judge which sever the value comes from by port number
		addr_len = sizeof their_addr;
		recvfrom(sock, &result, sizeof result, 0,(struct sockaddr *)&severs, &addr_len);
		port = ntohs(severs.sin_port);
		switch(port) {
	      	case 21846 : middleResults[4]=result; printf("The AWS received < %g > Backend-Server <A> using UDP over port < 24846 >\n",result); break;
	      	case 22846 : middleResults[6]=result; printf("The AWS received < %g > Backend-Server <B> using UDP over port < 24846 >\n",result); break;
	   }
	   printf("values of powers received by AWS: [%g,%g,%g,%g,%g,%g]\n",middleResults[1],middleResults[2],middleResults[3],middleResults[4],middleResults[5],middleResults[6]);
	   // judge which funciton we are going to use and compute the final result
	   if (strcmp(function,"DIV")==0){
	   		finalResult = 1 + middleResults[1]+middleResults[2]+middleResults[3]+middleResults[4]+middleResults[5]+middleResults[6];	
	   } else {
	   		for(int i=1;i<7;i++){
	   			finalResult = finalResult + middleResults[i]/(-i);
	   		}
	   }
	    printf("AWS calculated %s on <%g>: <%g>\n",function, input, finalResult);
		sprintf(buf,"%g",finalResult);
		len = strlen(buf);
		send(new_fd, &buf, len, 0); //send the final result to the client
		printf("The AWS sent <%g> to client\n",finalResult);
		finalResult = 0;
		close(new_fd);
	}

	freeaddrinfo(res);
	freeaddrinfo(resA);
	freeaddrinfo(resB);
	freeaddrinfo(resC);
	freeaddrinfo(resAWS);
	close(sockfd);
	close(sock);
	return 0;
}









