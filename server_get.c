#include"httpserver.h"
#define MAXBUF 5000
#define MAXBUFI 5000
#define SMALL 100

// pushes the file towards the client, until bytes sent equal to contentlength
int pushfile(int sockfd,char *buffer, FILE*fp,int h)
{
	int bytes_read=1;
	int bytes_write = 1;
	int total = 0;
//	FILE
while(bytes_write>0)
	 {
	    // Read data into buffer.  We may not have enough to fill up buffer, so we
	    // store how many bytes were actually read in bytes_read.
	    bytes_read = fread(buffer, sizeof(char), MAXBUF, fp);
	   // printf("%d\n", bytes_read);
	    //printf("%s\n", buffer);
	    total += bytes_read;
	    if (bytes_read == 0) // We're done reading from the file
	    	break;
 
	  if (bytes_read < 0) {
	        printf("Error in Byte_Read");// handle errors
		return 1;
	    }
	  if ((bytes_write = write(sockfd, buffer, bytes_read)) < 0) 
	    {
	    	printf("Error during writing!\n");
	    	return 1;
	   }
	   
	memset(buffer, '\0', MAXBUF);
	 if(h==bytes_write)
	 break;
	 }
close(sockfd);
fclose(fp);
return 0;

}



// if file is not found at the server following function sends 400 not ok response to client
int filenotfound(int newfd)
{	
	char sami[SMALL];
	int yoyo,rve;
	yoyo=sprintf(sami, "HTTP/1.1 404 Not Found\r\nContent-Length:\r\nContent-Type: text/plain\r\n\r\n");
	if(yoyo<0)
	{				
		printf("sprintf error\n");
		exit(2);				
	}
				
	rve= send(newfd,sami, strlen(sami), 0);
	if (rve !=(int) strlen(sami))
		printf("Error @ send\n");
	else
		printf("OK\n");

close(newfd);
return 0;
}

//responsible for the get request sent by client
int handle_get_request(char *buf , int newfd)
{//CHECKING FOR THE GET PART			
					
//extracting the name of the file				


char* temp=NULL;
char NOF[100] = { 0 };			
unsigned int rv;
int y,ready;
//int fd;
unsigned int i=0;

char sami[SMALL];
char buffer[MAXBUFI];
struct stat fileStat;
temp=strstr(buf,"GET /")+strlen("GET /");
for(i=0;i<strlen(temp);i++)
{
	if (*(temp+i)==' ' && *(temp+i+1)=='H')
		break;
	NOF[i]=*(temp+i);				
}
printf("this is file name=%s\n",NOF);				

/*Checking if the file exists, by the stat function*/
if(stat(NOF,&fileStat)<0)
{
printf("there is no file titled %s or there is no acess to read",NOF);
ready=0;
}
else
ready=1;
//-----------------------------------
if(ready==1)				
{				
	FILE* pfile;
	temp=NULL;
	pfile=fopen (NOF,"r");				
	if(pfile==NULL)
	{				
		printf("File error");				
		exit(1);				
	}				
	
	printf("File Size =%d\n",(int)fileStat.st_size);

	//putting header in buffer(sami)				
	y=sprintf(sami, "HTTP/1.1 200 OK\r\nContent-Length:%d\r\nContent-Type: text/plain\r\n\r\n",(int)fileStat.st_size);	
	if(y<0)
	{				
		printf("sprintf error\n");
		exit(2);				
	}				
	rv= send(newfd,sami, strlen(sami), 0);
	if (rv != strlen(sami))
		printf("Error @ send\n");
	else
	printf("OK\n");
	
	int h=0;
	h=(int)fileStat.st_size;
	pushfile(newfd,buffer, pfile, h);
				
}//end of if

else 
{
	int w=0;
	w=filenotfound(newfd);
	if(w==0)
	{
	printf("404 not ok response sent");
	}
	
}

return 1;		
}
