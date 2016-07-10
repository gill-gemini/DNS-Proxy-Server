/*****************************************************************************/
/*** HTTP DNS SERVER  MAIN FILE                                            ***/
/*** Bilal Shaukat Gill                                                    ***/
/*** Student Id:280244         						   ***/
/*****************************************************************************/

#include <stdio.h> //header for printf

#include "httpserver.h"

#define BACKLOG 10	 // how many pending connections queue will hold
#define MAXBUF 5000
#define SMALL 100
#define MAXBUFI 5000

/*Creates a daemon, which works on the background.(static , so cant be acess by other c files)*/
//static void daemonize(void)
static void daemonize(char* directory)
{
    /*pid data types represent process ids*/			
    pid_t pid, sid;

    // already a daemon getppid returns the parent id of the parent process
    if ( getppid() == 1 ) return;

    // Fork off the parent process 
    /* Clone ourselves to make a child */
    pid = fork();

    /* If the pid is less than zero,
   something went wrong when forking */
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    //If we got a good PID, then we can exit the parent process
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // At this point we are executing as the child process 

    // Change the file mode mask 
    /* Set the umask to zero */
    umask(0);


    // Create a new SID for the child process 
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    // Change the current working directory.  This prevents the current
       //directory from being locked; hence not being able to remove it. 
    //if ((chdir("/u/16/gillb1/unix/Desktop/server_13/server_280244_bilalgill")) < 0) {/home/gill/Desktop/dns
if ((chdir(directory)) < 0) {
        exit(EXIT_FAILURE);
    }
}




// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/*Start listening in this function.This function deals with all the networking calls and starts the server*/
int startserver(char **Strings)
{


	struct addrinfo hints,*ai,*p;	
	int get,sockfd;
	
	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_UNSPEC;
	//server can also have udp request
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_flags=AI_PASSIVE;
	
	get=(getaddrinfo(NULL,Strings[1],&hints,&ai));
	if(get!=0)
	{
	fprintf(stderr,"selectserver:%s\n",gai_strerror(get));
	exit(1);	
	}
	//Note that this is a linked list: ai_next points at the next element—there could be several results for you to choose from.	
	int yes=1;	
	for (p=ai;p!=NULL;p=p->ai_next)
	{
		//return socket descriptor		
		sockfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol);
		if (sockfd<0)
		{
		continue;
		}
		if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1)
		{
		perror("setsockopt");
		exit(1);
		}	
		//gives -1 on error(bind)		
		if (bind(sockfd,p->ai_addr,p->ai_addrlen)==0)		
		break;
		else{
		close(sockfd);
		continue;
		}	
}
	if (p==NULL)
		{
		fprintf(stderr,"selectserver:failed to bind\n");
		exit(2);		
		}	
		//you must free addrinfo boy!
		freeaddrinfo(ai);	
		
	if(listen(sockfd,BACKLOG)==-1)
		{
		perror("listen\n");
		exit(3);		
		}
	
return sockfd;
}
/*handle accept call and calls the required function for the put,get or post*/
int handleconnections(int sockfd,char **Strings)
{
	int newfd;
	char buf[MAXBUF];
	char s[INET6_ADDRSTRLEN];
	char sami[SMALL];
	char buffer[MAXBUFI];
	struct sockaddr_storage clientaddr;	
	socklen_t addrlen;
	addrlen=sizeof(clientaddr);
while(1)
		{//accepting connections
		newfd=accept(sockfd,(struct sockaddr *)&clientaddr,&addrlen);		
		//struct sockaddr_storage clientaddr;	
		//newfd = accept(listener,(struct sockaddr *)&remoteaddr,&addrlen);
		if(newfd==-1)
			{
			perror("accept");
			continue;
			}						
		
		inet_ntop(clientaddr.ss_family,get_in_addr((struct sockaddr *)&clientaddr),s,sizeof s);
		//inet_ntop(remoteaddr.ss_family,get_in_addr((struct sockaddr*)&remoteaddr),remoteIP, INET6_ADDRSTRLEN),newfd);
		printf("server: got connection from %s\n", s);
		//int byte_count;		
		
		if(!fork())
		{
			//this is a child process		
			close(sockfd);
			//sleep(20);					
			recv(newfd, buf, sizeof buf, 0);
			printf("buffer contains\n%s\n",buf);
			//Checking for PUT			
			if(buf[0]=='P' && buf[1]=='U' && buf[2]=='T')
			{
			handle_put_request(buf,newfd);
			}			

			else if (buf[0]=='G' && buf[1]=='E' && buf[2]=='T')
			{
			handle_get_request(buf,newfd);
			}

			else if (buf[0]=='P' && buf[1]=='O' && buf[2]=='S' && buf[3]=='T')
			{
			int timeout=atoi(Strings[5]);						
			handle_post_request(buf,newfd,Strings[4],timeout);
			}
			
			else
			printf("nor GET not PUT");					
			//creating index
			//close(fd);
			close(newfd);				
			exit(0);		
		
		}//closing the fork()
							
		close(newfd);				
		//Clearing Buffer	    
		memset(buf, '\0', MAXBUF);			
		memset(sami,'\0',SMALL);	
		memset(buffer,'\0',MAXBUFI);
	}//closing while(1)

return 1;
}

/*Main function call all the sub functions and handle signals*/
int main(int Count, char **Strings)
{
		
	int z=0;
	   z=atoi(Strings[2]);
	if(z==1)
	daemonize(Strings[3]);
	/*00000000000000000000000000*/
	struct sigaction sa;
	int sockfd;
	sockfd=startserver(Strings);
	//----->assign sig_child as our SigChild Handler	
	sa.sa_handler=sigchld_handler;
	//we dont want to block any other signals in this example	
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=SA_RESTART;//restart functions if interrupted handlers
	if(sigaction(SIGCHLD,&sa,NULL)==-1)
		{
		perror("sigaction");
		exit(1);		
		}
	printf("server: waiting for connections...\n");
	
	/*Registering the server at nwprog1.netlab.hut.fi--commented since server is not working  */
/*	int checkreg;	
	checkreg=serverregister(Strings[1],1);
	if(checkreg==0)
	{
	printf("Registeration/Degisteration done at nwprogserver\n");	
	}
	else
	printf("server not registered\n");	*/
	
	signal(SIGINT, signal_callback_handler);
        signal(SIGTSTP, signal_callback_handler);
	signal(SIGPIPE,sig_pipe);

	handleconnections(sockfd,Strings);	
	return 0;

}


