DNS-Proxy-Server
================

HTTP DNS Proxy server serves the request of client asking for lookup for the domain names.The dns proxy server connects to the name server using UDP.The name server replies with answers to dns proxy server.Ultimately dns proxy server replies to the client using tcp.

Build & Installation Instructions:

The name server which the dns proxy will contact is hard coded at the moment.
You can change from the file “dnsquery.c”
servaddr.sin_addr.s_addr=inet_addr("8.8.8.8");
just change the name server ip address to any of the name server.
After that just write “make” in the terminal(Linux Terminal).

Build & Installation Instructions:
Just write “make” in the terminal.


User Instructions:

At the moment ,only query type:A is supported in the implementation.If an alias name is found in the
answer records, it is also sent to the client.
To start the server.
./server <desired port> <1=daemon /2=not daemon > <directory of server> <ip address of name server
> <timeout for the name server>

For example:

For daemon
./server 2400 1 /Desktop/Dns 8.8.8.8 4

For starting server without daemon for debugging purposes.
./server 2400 2 /Desktop/Dns 8.8.8.8 4

The server will send the query for the name server to the address mentioned in the terminal.In this case
it will send it to 8.8.8.8.If there is no response from the name server , it will wait for 4 seconds and then
respond to the client with no answer.
The response from the dns proxy server is saved in the “results.txt” file in the local directory of the
httpclient implementation.You can see the results by using “nano results.txt”.
Some of the results may also be seen in the terminal(first recv is printed on terminal)

