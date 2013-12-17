/************************8
DNS QUERY
************************/


#include"dnsquery.h"


int handle_post_request(char *buff,int newfd)
{

unsigned char lname[100]={0};
char query[100]={0};
char ansbuffer[MAXBUF];
char ubuffer[MAXBUF];
int i;
char *temp=NULL;

/*Extracting the domain name for the lookup*/
temp=strstr(buff,"Name=")+strlen("Name=");
for (i=0;i<(int)strlen(temp);i++)
{
	if(*(temp+i)=='&' && *(temp+i+1)=='T')
	break;
	lname[i]=*(temp+i);
}
printf("\nName=%s",lname);

/*Extracting the Type from the buffer*/
temp=strstr(buff,"Type=")+strlen("Type=");
for (i=0;i<(int)strlen(temp);i++)
{
	if(*(temp+i)=='\r' && *(temp+i+1)=='\n')
	break;
	query[i]=*(temp+i);
}

printf("\nType=%s",query);
int type;
type=atoi(query);

int sockfd;
unsigned char buffer[65536];
struct sockaddr_in servaddr;
unsigned char *dns_name,*dns_read;
//unsigned char domain_name[50];
struct HEADER_DNS *hdr=NULL;
struct QUESTION *qfield=NULL;
struct sockaddr_in dns_addr;
struct RESOURCE_RECORD answer[20];
//struct RESOURCE_RECORD authority[20],additional[20];


unsigned char *domainname=lname;
/*Creating a UDP socket*/
sockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
if (sockfd<0)
{
printf("Error in Socket Creation");
return 1;
}
/* Filling the fields of struct sockaddr_in*/
bzero(&servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;//ipv4 used
servaddr.sin_addr.s_addr=inet_addr("130.233.224.141");//selected famous google dns server130.233.224.141
servaddr.sin_port=htons(53);//dns listens on port 53

/*Filling the DNS header with appropriate values for the standard query*/
hdr = (struct HEADER_DNS *)&buffer;

hdr->id = (unsigned short) htons(getpid());
hdr->qr=0;
hdr->opcode=0;
hdr->aa=0;
hdr->tc=0;
hdr->rd=1;
hdr->ra=0;
hdr->z=0;
hdr->rcode=0;
hdr->ad=0;
hdr->cd=0;

hdr->qdcount = htons(1);
hdr->ancount = 0;
hdr->nscount = 0;
hdr->arcount = 0;
   
/* Point to the question section format for getting the name*/
dns_name=(unsigned char*)&buffer[sizeof(struct HEADER_DNS)];

/*changing domain name to dns format e.g www.google.com -> 3www6google3com*/
int k=1;
k=ChangeNametoDnsFormat(dns_name,domainname);
if (k==0)
{
printf("\nchanged to dns format");
}

/*Filling the types of the Question Section*/
qfield=(struct QUESTION*)&buffer[sizeof(struct HEADER_DNS)+(strlen((const char*)dns_name)+1)];
    qfield->qtype=htons(1);//Query type 1  given for 
    qfield->qclass=htons(1);
    int m=0;
    /* Sending data to the DNS server*/
    //ssize_t sendto(int socket, const void *message, size_t length,int flags, const struct sockaddr *dest_addr,socklen_t dest_len);
    printf("\nsending header");
    m=sendto(sockfd,(char*)buffer,sizeof(struct HEADER_DNS)+(strlen((const char*)domainname)+1)+ sizeof(struct QUESTION),0,(struct sockaddr*)&servaddr,sizeof(servaddr));
    if(m<0)
    {
    printf("send to failed");
    }
    printf("recieving data");
    int u=0;
    int h;
    h=sizeof servaddr;
    u=recvfrom(sockfd,(char*)buffer,sizeof(buffer),0,(struct sockaddr*)&servaddr,(socklen_t*)&h);
    if(u<0)
    {
    printf("\nrecfrom failed");
    }

    /*Giving offset to reach the answers section recieved*/
    dns_read=&buffer[sizeof(struct HEADER_DNS) +(strlen((const char*)domainname)+1)+ sizeof(struct QUESTION)];

    printf("\nThe response contains : ");
    printf("\n %d Questions.",ntohs(hdr->qdcount));
    printf("\n %d Answers.",ntohs(hdr->ancount));
    printf("\n %d Authoritative Servers.",ntohs(hdr->nscount));
    printf("\n %d Additional records.\n\n",ntohs(hdr->arcount));

    int block=0;
    int l=0; 
   memset(ubuffer, '\0', MAXBUF);
    memset(ansbuffer, '\0', MAXBUF);


     /* Loops to all the answer records recieved*/
   

   for(l=0;l<ntohs(hdr->ancount);l++)
    {
        //Using DNS_Name_Read to read the name and then storing into answer records
        answer[l].r_name=DNS_Name_Read(dns_read,buffer,&block);
        dns_read = dns_read + block;

        
        answer[l].resource = (struct R_FIELD*)(dns_read);
        dns_read = dns_read + sizeof(struct R_FIELD);
 
        if(ntohs(answer[l].resource->r_type) == 1) //if its an ipv4 address
        {
            answer[l].r_data = (unsigned char*)malloc(ntohs(answer[l].resource->r_data_len));
 	    int e;
            for(e=0 ; e<ntohs(answer[l].resource->r_data_len) ; e++)
            {
                answer[l].r_data[e]=dns_read[e];
            }
 
            answer[l].r_data[ntohs(answer[l].resource->r_data_len)] = '\0';
 
            dns_read = dns_read + ntohs(answer[l].resource->r_data_len);
        }
        else
        {
            answer[l].r_data = DNS_Name_Read(dns_read,buffer,&block);
            dns_read = dns_read + block;
        }
    }

    printf("\nAnswer Records : %d \n" , ntohs(hdr->ancount) );
//Print and Saves all the records in the ubuffer so that they can be sent to the client    
for(l=0 ; l < ntohs(hdr->ancount) ; l++)
    {
        printf("Name : %s ",answer[l].r_name);
        sprintf(ansbuffer,"Name : %s ",answer[l].r_name);
        strcat(ubuffer,ansbuffer);
 	
        if( ntohs(answer[l].resource->r_type) == 1) //IPv4 address
        {
            long *p;
            p=(long*)answer[l].r_data;
            dns_addr.sin_addr.s_addr=(*p); //working without ntohl
            printf("has IPv4 address : %s",inet_ntoa(dns_addr.sin_addr));
	    sprintf(ansbuffer,"has IPv4 address : %s\n",inet_ntoa(dns_addr.sin_addr));
	    strcat(ubuffer,ansbuffer);

        }
         
        if(ntohs(answer[l].resource->r_type)==5) 
        {
            //Canonical name for an alias
            printf("has alias name : %s",answer[l].r_data);
            sprintf(ansbuffer,"has alias name : %s\n",answer[l].r_data);
	    strcat(ubuffer,ansbuffer);
        }
      printf("\n----------type=%d----\n",answer[l].resource->r_type); 
        printf("\n");
    }
   
    int n=1;
//Sending the answers to the client using following function
    n=senddnsresponse(newfd,ubuffer);
    if(n==0)
    {
	printf("send response to client successful");
    }
  
close(sockfd);
close(newfd);   
return 0;
}
 



