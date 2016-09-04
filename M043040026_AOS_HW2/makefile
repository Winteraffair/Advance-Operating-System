all : Server Client

Server : Server.c myftp.c
	gcc -o Server Server.c myftp.c -lpthread
Client : Client.c myftp.c 
	gcc -o Client Client.c myftp.c -lpthread

clean :
	rm -rf Server Client *~ *.o
