#define __STDC_FORMAT_MACROS

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
#include <arpa/inet.h>
#include<pthread.h>
#include"sqlite3.h"


char buf[100];
char *data;
void *server(void *ptr);
void readdata(int fd);
char buffer[100];
int flag = 0;
char *val,*val1;
void readDb(int no,int fd,int stop);
char sendstr[100];
int port;


/***************DB variables************************/

//function to open the port
int open_port(void)
{	
	int fd; // file description for the serial port
    fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);

	if(fd == -1) // if open is unsucessful
	{
		//perror("open_port: Unable to open /dev/ttyS0 - ");
		printf("open_port: Unable to open /dev/ttyS0. \n");
	}
	else
	{
		fcntl(fd, F_SETFL, 0);
		printf("port is open.\n");
	}

	return(fd);
} //open_port

//function to configure the port
void configure_port(int fd)      // configure the port
{
	struct termios port_settings;      // structure to store the port settings in

	cfsetispeed(&port_settings, B9600);    // set baud rates
    cfsetospeed(&port_settings, B115200);

	port_settings.c_cflag &= ~PARENB;    // set no parity, stop bits, data bits
	port_settings.c_cflag &= ~CSTOPB;
	port_settings.c_cflag &= ~CSIZE;
	port_settings.c_cflag |= CS8;
	port_settings.c_cflag |= CREAD;

	port_settings.c_cc[VMIN]=1;		//read atleast 1 character before returning

	tcsetattr(fd, TCSANOW, &port_settings);    // apply the settings to the port

}


int main(){
  pthread_t thread1, thread2;
  port = open_port();
  configure_port(port);

  /*int n;
  n = write(port, "action BoondockSaints nil nil right-top\r\n", strlen("action BoondockSaints nil nil right-top\r\n"));*/

  /*char op[256] = "action BoondockSaints nil int ";
  char str[256] = "/v/5";
  int i=0;
  char *token;
  token = strtok(str, "/");
  while(token != NULL)
  {
      printf("%s:%d\n",token,i);
      if(i == 1)
      {
        strcat(op,token);
        strcat(op,"\r\n");
      }

      i++;

      token = strtok(NULL,"/");
  }

  printf("sending to slave %s\n",op);*/

  char *message1 = "Thread 1";
  char *message2 = "Thread 2";	
  pthread_create( &thread2, NULL, server, (void*) message2);
  pthread_join( thread2, NULL);
  }

//server 
void *server(void *ptr)
{
  // structs to represent the server and client
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
      server_addr.sin_port = htons(10055); // specify port number
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
      printf("Server got a connection from (%s, %d)\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
      //integer to read string size-
      
      int pid;
      int client_req=0;
      char andr_ip[256];
      int rxd;
      while(1)
      {
        fd = accept(sock, (struct sockaddr *)&client_addr,(socklen_t *)&sin_size);
        printf("Got\n");
        if(fd < 0)
            printf("Error on new connection");
        pid = fork();
        if (pid < 0)
             error("ERROR on fork");
        if (pid == 0)  {
                 close(sock);
                 readdata(fd);
                 exit(0);
        }
        else{
            close(fd);
        }

        //rxd = recv(fd,&client_req,1,0);
        //printf("Number received %d:\n ", client_req);

     }

      close(fd);
      
      printf("Server closed connection\n");
  }

//function to read data
void readdata(int fd)
{
	int client_req = 0;
    char str[256];
	int received;
    int n;
    received = recv(fd,&str,256,0);
    printf("Number received: %s\n ", str);

      char op[256] = "action BoondockSaints nil int ";
      int i=0;
      char *token;
      token = strtok(str, "/");
      while(token != NULL)
      {
          printf("%s:%d\n",token,i);
          if(i == 1)
          {
            strcat(op,token);
            strcat(op,"\r\n");
          }

          i++;

          token = strtok(NULL,"/");
      }

      printf("sending to slave %s\n",op);
      n = write(port, op, strlen(op));


}


