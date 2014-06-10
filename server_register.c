#include"httpserver.h"

#define MAXBUF 5000
/*This function registers the server on nwprog1.netlab.hut.fi and also deregisters*/
int serverregister(char* port,int regis)
{
unsigned int rv;
int rockfd;
int n=1;
const char* file="servers-gill.txt";
//const char* dns="nwprog1.netlab.hut.fi";
const char* cport;
cport="3000";
char buffer[MAXBUF];
struct addrinfo hints;
struct addrinfo *res;
const char* host_name;
host_name="nwprog1.netlab.hut.fi";
/*---Initialize server address/port struct---*/
//size_t out_size=sizeof(buffer);
bzero(&hints, sizeof(struct addrinfo));
hints.ai_family = AF_UNSPEC;
hints.ai_socktype=SOCK_STREAM;
//Name Resolution Address and port 
printf("host_name%s....port%s...\n",host_name,port);	
n=getaddrinfo(host_name,cport, &hints, &res);
if (n!=0)
{
	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(n));
        exit(EXIT_FAILURE);
}
else
{
	printf("getaddrinfo OK\n");
}
//ressave =res;

do{

	rockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if (rockfd==-1)
	{
		printf("socket error\n");
	}
	else
	{
		printf("socket OK\n");
	}
/*---Connect to server---*/
rv = connect(rockfd,res->ai_addr,res->ai_addrlen);
if (rv == 0)
	printf("Connect Ok\n");
else
	printf("Connect error registering\n");
	break ; 

}while ((res=res->ai_next)!=NULL);

if (res==NULL)
	{
	printf("res=null error");
	}

/*will send gillb1 port:_____  and empty file when want to deregister*/
if(regis==1)
{
sprintf(buffer, "PUT /%s HTTP/1.1\r\nHost: %s\r\nContent-Type: text/plain\r\nIam:gill\r\nContent-Length: 27\r\n\r\n nwprog1.netlab.hut.fi:%s", file,host_name,port);
//printf("buffer contains..%s\n",buffer);
}
else
sprintf(buffer, "PUT /%s HTTP/1.1\r\nHost: %s\r\nContent-Type: text/plain\r\nIam:gill\r\nContent-Length: 2\r\n\r\n\r\n", file,host_name);
	//nwprog1.netlab.hut.fi:8934
	rv = 0;
		rv = send(rockfd, buffer, strlen(buffer), 0);
		if (rv != strlen(buffer))
			printf("Error @ send\n");
		else
			printf("OK at send\n");

		 memset(buffer, '\0', MAXBUF);

close(rockfd);
return 0;
}
