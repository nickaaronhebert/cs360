#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         100
#define HOST_NAME_SIZE      255

int  main(int argc, char* argv[])
{
	int Socket;                 /* handle to socket */
    struct hostent* server;   /* holds info about a machine */
    struct sockaddr_in Address;  /* Internet socket address stuct */
    long nHostAddress;
    char pBuffer[BUFFER_SIZE];
    unsigned nReadAmount;
    char strHostName[HOST_NAME_SIZE];
    int nHostPort;

    //Checking to see if we have enough arguments
    if(argc < 3)
    {
        printf("\nUsage: client host-name host-port\n");
        return 0;
    }
    else
    {
    	//Copying the arguments into the host name variable and passing  the host port number
    	strcpy(strHostName, argv[1]);
    	nHostPort=atoi(argv[2]);
    }
    //creating the socket.
    Socket = socket(AF_INET, SOCK_STREAM, 0);
    server = gethostbyname(strHostName);
    if (server == NULL)
    {
    	printf("\nCould not connect to host \n");
    	exit ( 0 );
    }
    //copying the address from the server struct into the HostAddress field
    //memcpy(&nHostAddress,server->h_addr,server->h_length);

    //setting Address to 0 
    memset(&Address, 0, sizeof(Address));
    memcpy((void*)&Address.sin_addr.s_addr, (const void*) server->h_addr, server->h_length);
	
	//where are these variables coming  from? They come from the netinet/in.h
	Address.sin_port=htons(nHostPort);
    Address.sin_addr.s_addr=nHostAddress;
    Address.sin_family=AF_INET;

    //calling connect
    //socket is the name of my socket, casting Address into a sockaddr struct, and passing in the size of address

    if (connect (Socket, (struct sockaddr*) &Address, sizeof(Address)))
    {
    	printf("\nCould not connect to host \n");
    	return 0;
    }
    string path_to_file = "";
    path_to_file = argv[3];
    string request = "GET " + path_to_file + " HTTP/1.0\r\nHost: " + strHostName + "\r\n\r\n"
    int n = write (Socket, request, strlen(request));
    int buffer = 0;

    string buffer = "";

    while (buffer.find("\r\n\r\n" == false)
    {
    	printf("\nCool \n");
    	n += read(socket, buffer, 255);
    	buffer = "";
    } 
  }
    





    
	
	//setting the port number



