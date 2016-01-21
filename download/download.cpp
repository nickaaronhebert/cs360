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
#include <ostream>
#include <vector>
#include "sample.cpp"

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         512
#define HOST_NAME_SIZE      255

using namespace std;

int download_header_html(int socket)
{
    int amount_read = 0;
    unsigned nReadAmount;
    int final_num = 0;
    vector<char *> header_lines;
    char pBuffer[BUFFER_SIZE];

    GetHeaderLines(header_lines, socket, 0);
    for (int i = 0; i < header_lines.size(); ++i)
    {
     cout <<endl<<header_lines[i];
     if (strstr(header_lines[i],"Content-Length")!=NULL)
     {
        string content_string = string(header_lines[i]);
        int content_location = content_string.find(":") + 2;
        string num_string = content_string.substr(content_location, content_string.length());
        cout << endl << num_string;
        final_num = atoi(num_string.c_str());
     }
    }
    while (amount_read < final_num)
    {
      nReadAmount=read(socket,pBuffer,1);
      amount_read += nReadAmount;  
      cout << pBuffer[0];
    }
}

int  main(int argc, char* argv[])
{
	int hSocket;
    vector<char *> header_lines;                 /* handle to socket */
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address;  /* Internet socket address stuct */
    long nHostAddress;
    char pBuffer[BUFFER_SIZE];
    unsigned int nReadAmount;
    char strHostName[HOST_NAME_SIZE];
    int nHostPort;
    extern char * optarg;
    int c, times_to_download = 1, err = 0;
    bool debug = false;
    bool html = true;
    string page = "";
    int final_num = 0;
    int amount_read = 0;
    

 //    //Checking to see if we have enough arguments
    if(argc > 6)
    {
        printf("\nUsage: download host-name host-port -c or -d\n");
        return 0;
    }
    else
    {
        html = true;
        debug = false;
        while ( ( c = getopt(argc, argv, "c:d") ) != -1)
        {
            switch(c)
            {
              case 'c':
                times_to_download = atoi(optarg);
                cout << times_to_download;
                html = false;
                break;
              case 'd':
                debug = true;
                html = true;
                break;
              case '?':
                err = 1;
                break;
            }
        }
        strcpy(strHostName, argv[optind]);
        page = argv[optind + 2];
        nHostPort=atoi(argv[optind+1]);
        

    }
    /* make a socket */
    hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(hSocket == SOCKET_ERROR)
    {
        printf("\nCould not make a socket\n");
        return 0;
    }

    /* get IP address from name */
    pHostInfo=gethostbyname(strHostName);
    /* copy address into long */
    memcpy(&nHostAddress,pHostInfo->h_addr,pHostInfo->h_length);

    /* fill address struct */
    Address.sin_addr.s_addr=nHostAddress;
    Address.sin_port=htons(nHostPort);
    Address.sin_family=AF_INET;

    

    /* connect to host */
    if(connect(hSocket,(struct sockaddr*)&Address,sizeof(Address)) 
       == SOCKET_ERROR)
    {
        printf("\nCould not connect to host\n");
        return 0;
    }

    #define MAXGET 1000
    char * message = (char *)malloc(MAXGET);
    sprintf(message, "GET %s HTTP/1.1\r\nHost:%s%u\r\n\r\n", page.c_str(), strHostName, nHostPort);

    int successes = 0;
    if (debug == true)
    {
        write(hSocket, message, strlen(message));
        download_header_html(hSocket);
    }
    else if (debug == false && html == true)
    {
        write(hSocket, message, strlen(message));
        GetHeaderLines(header_lines, hSocket, 0);
        for (int i = 0; i < header_lines.size(); ++i)
        {
         if (strstr(header_lines[i],"Content-Length")!=NULL)
         {
            string content_string = string(header_lines[i]);
            int content_location = content_string.find(":") + 2;
            string num_string = content_string.substr(content_location, content_string.length());
            final_num = atoi(num_string.c_str());
         }
        }
        while (amount_read < final_num)
        {
          nReadAmount=read(hSocket,pBuffer,1);
          amount_read += nReadAmount;  
          cout << pBuffer[0];
        }
    }
    else 
    {
        write(hSocket, message, strlen(message));
        GetHeaderLines(header_lines, hSocket, 0); 
        for (int i = 0; i < header_lines.size(); ++i)
        {
            cout <<endl<<header_lines[i];
        }
        int download_tracker = 0;
        cout << endl;
        while (download_tracker < times_to_download)
        {
            //write(hSocket, message, strlen(message));
            nReadAmount=read(hSocket,pBuffer,1);
            if (nReadAmount > 0)
            {
                download_tracker++;
            }
            if(close(hSocket) == SOCKET_ERROR)
            {
                printf("\nCould not close socket\n");
                return 0;
            }
            hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
            if(hSocket == SOCKET_ERROR)
            {
                printf("\nCould not make a socket\n");
                return 0;
            }
        }
        cout << "The number of successfull downloads is: " << download_tracker;
       
    }
            
    printf("\nClosing socket\n");
    /* close socket */                       
    if(close(hSocket) == SOCKET_ERROR)
    {
        printf("\nCould not close socket\n");
        return 0;
    }
    free(message); 
  }



