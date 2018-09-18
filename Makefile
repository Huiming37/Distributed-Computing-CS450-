all: client.c aws.c servers.c
	gcc -o client client.c
	gcc -o aws aws.c
	gcc -o servers servers.c -lm

servers:
	./servers
aws:
	./aws
client: 
	./client
.PHONY: servers aws client
