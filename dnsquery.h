#ifndef DNSQUERY_H //Guard from double inclusion
#define DNSQUERY_H

/*Header files */
#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>// sockets defined
#include<netinet/in.h>//AF_INET
#include<string.h>//strcat
#include<arpa/inet.h>
#include"httpserver.h"
//#include"dnsquery.h"//function defined for dns query
#define MAXBUF 5000
#define DNS_V4 1

/**********************************************
                                   1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      ID                       |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    QDCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ANCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    NSCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ARCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    DNS header contain the above mentioned fields 
****************************************************/
struct HEADER_DNS
{
	unsigned short id;//16 bit identifier
	unsigned char rd :1;
	unsigned char tc :1;//Truncation
	unsigned char aa :1;//specifies if name server is authoritative
	unsigned char opcode :4;//specifies kind of query
	unsigned char qr :1;//specifies whether it is query or response
	
	
	
	unsigned char rcode :4;//Response code

	unsigned char cd :1;//check is disabled NA IN HEADER
	unsigned char ad :1;//data is authenticated NA IN HEADER
	unsigned char z :1;// Reserved for the future use
	unsigned char ra :1;// Recursion Available
	
	
	
	unsigned short qdcount;//no of entries in question section
	unsigned short ancount;// no of entries in answer section
	unsigned short nscount;// no of entries in authority records section
	unsigned short arcount;//no of entries in additional records section

};

/*********************************************
                                1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                     QNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QTYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QCLASS                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    QUESTION SECTION FORMAT
**************************************************/
//typedef struct
/*struct QUESTION
{
	unsigned char *qname;
	unsigned short *qtype;
	unsigned short *qclass;
};*/

struct QUESTION
{
    unsigned short qtype;
    unsigned short qclass;
};

typedef struct
{
    unsigned char *name;
    struct QUESTION *ques;
} QUERY;

/********************************************
                                1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                                               /
    /                      NAME                     /
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     CLASS                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TTL                      |
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                   RDLENGTH                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
    /                     RDATA                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
Resourse record format
****************************************************/

#pragma pack(push, 1)
struct R_FIELD
{
    unsigned short r_type;
    unsigned short r_class;
    unsigned int r_ttl;
    unsigned short r_data_len;
};
#pragma pack(pop)
 
//Pointers to resource record contents
struct RESOURCE_RECORD
{
    unsigned char *r_name;
    struct R_FIELD *resource;
    unsigned char *r_data;
};



/********************************
changes www.google.com to 3www6google3com

*******************************/

int ChangeNametoDnsFormat(unsigned char*,unsigned char*);


/**********************************
changes 3www6google3com to www.google.com
********************************/

int ChangeDnsNametoNormalFormat(unsigned char*,unsigned char*);



u_char* DNS_Name_Read(unsigned char* ,unsigned char* ,int*);


/*Send the response back to the client for the lookup*/
int senddnsresponse(int newfd,char *ubuffer);


#endif
