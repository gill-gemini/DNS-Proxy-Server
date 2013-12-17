#include"httpserver.h"
#define MAXBUF 5000


/*
Following function parses the header ,finds the content length 
and returns it
*/
int findcontentlength(char*muffer)
{
char *point;
char length[100] = { 0 };
point=strstr(muffer,"Content-Length:")+strlen("Content-Length:");
unsigned int i;
for (i=0;i<strlen(point);i++)
{
	if(*(point+i)=='\r' && *(point+i+1)=='\n')
	break;
	length[i]=*(point+i);
}
int len=0;				
len=atoi(length);
return len;
}


/* Pulls the file from the client and then save into the local directory*/
int pullfile(int k,const char *file,char* muffer, int sockfd)
{

int fd;
char *temp;
int bytes_read=1;
int bytes_write=1;
int total=0;

printf("Inside 200OK\n");		
//Parsing the Header	

//parsing and getting the length in payload of the file recieved by using the functin "content length"
int lenn=0;
lenn=findcontentlength(muffer);
		
printf("filesize...%d\n",lenn);	
fd = open(file, O_RDWR | O_CREAT | O_NONBLOCK, S_IRWXU);
if (fd < 0) 
	{                                
	printf("open local file error");
        return -1;
        }
//reaching till payload and temp contains the whole string						
temp=strstr(muffer,"\r\n\r\n")+strlen("\r\n\r\n");
if (write(fd, temp, strlen(temp)) !=(int) strlen(temp)) 
{
  	printf("partially write/Failed");
        return -1;
}
int u=0;
u=(int)strlen(temp);
printf("bytes_read.%d....total...%d..filesize.%d\n",bytes_read,total,lenn);

/******************************************************************
Recieving until there is not byte left from the server!		
Also matching the content length with the number of bytes
*****************************************************************/		
//while (( bytes_read > 0 ) && (total < (filesize)))
//sleep(8);

while (bytes_read!=0)
{
	//count++;
	printf("%s", muffer);			
		
		
	memset(muffer, '\0', MAXBUF);			
	bytes_read = recv(sockfd, muffer, sizeof(muffer), 0);
	printf("bytes_read %d....filesize%d....total...%d\n",bytes_read,lenn,total);	
	if(bytes_read==0)
	break;	

	bytes_write=write(fd,muffer,strlen(muffer));
//	printf("bytes_write....%d",bytes_write);
	total = bytes_read+total;
	memset(muffer, '\0', MAXBUF);
	if(total+u==lenn)
	break;		

}
		
printf("total bytes written%d and file size was %d.\n",total+u,lenn);
printf("strlen temp%d\n",u);

close(fd);
close(sockfd);
return 0;

}




//responsible for the put request sent by client 
int handle_put_request(char *buffer, int newfd)
{			
			
			
char NOF[100] = { 0 };			
int ready=0;

unsigned int i=0;

//char REQNO[20] = { 0 };		
char* temp=NULL;


//char  itemp[256]={0x0};
struct stat fileStat;
			
//parsing data and getting the name of file

temp=strstr(buffer,"PUT /")+strlen("PUT /");
for(i=0;i<strlen(temp);i++)
{
	if (*(temp+i)==' ' && *(temp+i+1)=='H')
	break;
	NOF[i]=*(temp+i);				
}

//ill=(parsing(buffer,"PUT /", 1));			
printf("Recieved Filename%s\n",NOF);

/*Checking if the file exist, if exist it is removed so nothing gets over write */				
if(stat(NOF,&fileStat)<0)
{
	printf("file doesn't exist add file to directory\n");
}
else 
{
	if(remove(NOF)!=0)
	perror("Error deleting file");
}

									
/*Adding the filename into the index*/
FILE* pf;				
if(ready==0)				
{				
	pf=fopen("index","a");				
	fprintf(pf,"%s\n",NOF);
}

int k;
int g=0;
k=pullfile(g,NOF,buffer, newfd);
if(k==0)
{
printf("pullfile success");
}


return 1;		
}//ending if buf[0]==P	

