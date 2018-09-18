# Distributed-Computing-Socked-Programming

- Used C language to fulfill the function of computation of two taylor series, and the information transimission is based on UDP and TCP. And the whole project consists of aws, client and severs. And the project also uses multiple threads.<br >
 
- The whole project consists of three C files which are client.c, aws.c and servers.c. The client.c is responsible for transimitting the function and the value to the aws. The aws is responsible for transimitting the input value to the servers and computing the final results usig the formula of the taylor series. The servers are responsible for computing the pow of the input value. The transmission between client and aws is based on TCP, and the transmission between aws and servers is based on UDP.<br >

- The client transmits string data to aws, and aws transmit string datat to float data. and the transmission between aws and servers are using float data. At last, the aws transform the float final results to string final results and then transmit the final results to client. <br >
