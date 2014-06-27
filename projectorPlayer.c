#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<pthread.h>
#include <netdb.h>

void main()
{
	struct sockaddr_in server_addr,client_addr;    
      
      int sock; // socket descriptor

      // 1. socket: creates a socket descriptor that you later use to make other system calls
      if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket");
            exit(1);
      }

      int temp;
      if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&temp,sizeof(int)) == -1) {
        perror("Setsockopt");
        exit(1);
      }

      // configure the server
      server_addr.sin_port = htons(11002);// specify port numbe
      server_addr.sin_family = AF_INET;         
      server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
      bzero(&(server_addr.sin_zero),8); 
      
      // 2. bind: use the socket and associate it with the port number
      if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
            perror("Unable to bind");
            exit(1);
      }

      // 3. listen: indicates that we want to listn to the port to which we bound; second arg is number of allowed connections
      if (listen(sock, 5) == -1) {
            perror("Listen");
            exit(1);
      }
          
      printf("\nServer waiting for connection on port 1234\n");
        fflush(stdout);
     

      // 4. accbufept: wait until we get a connection on that port
      int sin_size = sizeof(struct sockaddr_in);
      int fd; 
      //integer to read string size-
      
      int pid;

      int sock1; // socket descriptor

      // 1. socket: creates a socket descriptor that you later use to make other system calls
      if ((sock1 = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket");
            exit(1);
      }

      int temp1;
      if (setsockopt(sock1,SOL_SOCKET,SO_REUSEADDR,&temp,sizeof(int)) == -1) {
        perror("Setsockopt");
        exit(1);
      }

      // configure the server
      server_addr.sin_port = htons(11003);// specify port numbe
      server_addr.sin_family = AF_INET;         
      server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
      bzero(&(server_addr.sin_zero),8); 
      
      // 2. bind: use the socket and associate it with the port number
      if (bind(sock1, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
            perror("Unable to bind");
            exit(1);
      }

      // 3. listen: indicates that we want to listn to the port to which we bound; second arg is number of allowed connections
      if (listen(sock1, 5) == -1) {
            perror("Listen");
            exit(1);
      }
          
      printf("\nServer waiting for connection on port 1234\n");
        fflush(stdout);
     

      // 4. accbufept: wait until we get a connection on that port
      int sin_size1 = sizeof(struct sockaddr_in);
      int fd1; 
      //integer to read string size-
      	
      
      
     
      while(1)
      {
	int client_req = 0;
	char andr_ip[256];
	int rxd;
	printf("Beginning of while\n");
	fd = accept(sock, (struct sockaddr *)&client_addr,(socklen_t *)&sin_size);
		
	printf("Server got a connection from (%s, %d)\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
	printf("Got\n");
	if(fd < 0){
	    printf("Error on new connection");
	}
	  
	rxd = recv(fd,&client_req,sizeof(client_req),0);
	printf("Data received :%d\n ", client_req);
	close(fd);
 	if(client_req == 1)
	{
		usleep(60000000);			
		system("mplayer nemo.mp4 -fs");
	}

	else if(client_req == 0)
	{
	  
	    //system("mplayer aa.mp4");
	
	}

	fd1 = accept(sock1, (struct sockaddr *)&client_addr,(socklen_t *)&sin_size);
		
	printf("Server got a connection from (%s, %d)\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
	printf("Got\n");
	if(fd < 0){
	    printf("Error on new connection");
	}
	  
	rxd = recv(fd,&client_req,sizeof(client_req),0);
	printf("Data received :%d\n ", client_req);
	close(fd);
 	if(client_req == 1)
	{
				
		system("mplayer nemo.mp4");
	}

	else if(client_req == 0)
	{
	  
	    system("mplayer aa.mp4");
	
	}


	printf("Nearing end of while\n");

     }
     
	

      close(fd);
      close(fd1);     
      printf("Server closed connection\n");    
  	
}
