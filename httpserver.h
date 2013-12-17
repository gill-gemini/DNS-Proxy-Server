
#ifndef HTTPSERVER_H //Guard from double inclusion
#define HTTPSERVER_H

#include <stdio.h> //header for printf
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  //header for fork
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/wait.h>   /* header for waitpid() and various macros */
#include <signal.h>     /* header for signal functions */
#include <sys/stat.h>    //header for mode bits for acess permissions in mkdir
//#include <io.h>	//for open ocreate blahblah
#include <errno.h>//errnovalues
#include <fcntl.h>


//responsible for the put request sent by client 
int handle_put_request(char *buffer, int newfd);

//responsible for the get request sent by client
int handle_get_request(char *buffer , int newfd);

/*Server Post implementation.Contacts the DNS server for the result.
Send backs the result to the client*/
int handle_post_request(char *buffer,int newfd);
#endif

