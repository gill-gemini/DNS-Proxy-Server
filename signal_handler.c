
#include "httpserver.h"



void sigchld_handler(int s)
{
	/*Wait for all dead processes.
	 * We use a non-blocking call to be sure this signal handler will not
	 * block if a child was cleaned up in another part of the program.*/ 
	while(waitpid(-1, NULL, WNOHANG) > 0);
}


// Define the function to be called when ctrl-c (SIGINT) signal is sent to process
void signal_callback_handler(int signum)
{

   //char* geo;
   printf("Caught signal %d\n",signum);
   // Cleanup and close up stuff here 
   int checkreg;	
   checkreg=serverregister(NULL,0);
   if(checkreg==0)
   {
   printf("Degistered  at nwprogserver\n");	
   }
   else
   printf("server not deregistered\n");

   // Terminate program
   exit(signum);
}

/*SIGPIPE Handler*/
void sig_pipe(int signum)
{
	printf("got sigpipe");
	signal(SIGPIPE,sig_pipe);
}


