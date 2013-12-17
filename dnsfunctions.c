#include"dnsquery.h"


//This function changes the format from www.google.com to 3www6google3com which is dns format
int ChangeNametoDnsFormat(unsigned char*dns_name,unsigned char*domainname)
{
    strcat((char*)domainname,".");
    int shot=0;
    int k=0;
    for( k=0;k<(int)strlen((char*)domainname);k++)
    {
	if(domainname[k]=='.')
	{
	    *dns_name++=k-shot;
	    for(;shot<k;shot++)
	    {
	        *dns_name++=domainname[shot];
	    }
	   shot++;
	}
    }
    *dns_name++='\0';
return 0;    
}


//Since dns uses compression , the following function reads the name from the dns format
u_char* DNS_Name_Read(unsigned char* reader,unsigned char* buffer,int* count)
{
    unsigned char *name;
    unsigned int p=0,jumped=0,offset;
    int i , j;
 
    *count = 1;
    name = (unsigned char*)malloc(256);
 
    name[0]='\0';
 
    //read the names in 3www6google3com format         256 128 64 32 16 8 4 2 1 
    while(*reader!=0)			//	  	1  1  0  0 0 0 0 0 =192
    {	

	/*

    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    | 1  1|                OFFSET                   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	if the first two bits 1 , then there is a offset
           128 64 32 16 8 4 2 1 
	     1  1  0  0 0 0 0 0 =192

	//see the example in RFC 1035 SECTION 4.3.1 to 							*/
        if(*reader>=192)
        {
            offset = (*reader)*256 + *(reader+1) - 49152; //49152 = 11000000 00000000 ;)
            reader = buffer + offset - 1;
            jumped = 1; //we have jumped to another location so counting wont go up!
        }
        else
        {
            name[p++]=*reader;
        }
 
        reader = reader+1;
 
        if(jumped==0)
        {
            *count = *count + 1; //if we havent jumped to another location then we can count up
        }
    }
 
    name[p]='\0'; //string complete
    if(jumped==1)
    {
        *count = *count + 1; //number of steps we actually moved forward in the packet
    }
 
    //now convert 3www6google3com0 to www.google.com
    for(i=0;i<(int)strlen((const char*)name);i++) 
    {
        p=name[i];
        for(j=0;j<(int)p;j++) 
        {
            name[i]=name[i+1];
            i=i+1;
        }
        name[i]='.';
    }
    name[i-1]='\0'; //remove the last dot
    return name;
}

//Following function sends the response of the name server to the client using tcp
int senddnsresponse(int newfd,char *ubuffer)
{
    char samibuffer[MAXBUF];
    printf("heres the ansbuffer output\n%s",ubuffer);    
    int ulen;
    ulen=strlen(ubuffer);
    printf("the length of the ubuffer%d\n",ulen);
    int total=0,y,rv;
    int bytes_write=1;
    y=sprintf(samibuffer, "HTTP/1.1 200 OK\r\nContent-Length:%d\r\nContent-Type: text/plain\r\n\r\n",ulen);	
	if(y<0)
	{				
		printf("sprintf error\n");
		exit(2);				
	}				
	rv= send(newfd,samibuffer, strlen(samibuffer), 0);
	if (rv !=(int) strlen(samibuffer))
		printf("Error @ send\n");
	else
	printf("OK\n");
    while (bytes_write!=0)
	{
		
		if ((bytes_write = write(newfd, ubuffer,strlen(ubuffer))) < 0) 
		{
	    		printf("Error during writing!\n");
	    		return -1;
	    	}
		total+=bytes_write;
		memset(ubuffer, '\0', MAXBUF);
	}//while for reading and writing

return 0;
}


