# DNS-Proxy Server

HTTP DNS Proxy server serves the request of client asking for lookup for the domain names.The dns proxy server connects to the name server using UDP.The name server replies with answers to dns proxy server.Ultimately dns proxy server replies to the client using tcp.

 **What did i Learned:** 
* 1.Signal Handling
* 2.Semaphores
* 3.Processes(Forking)
* 4.Buffer Managment
* 5.Server side (Socket Calls)


**Build & Installation Instructions:**

* Download the code.Just write “make” in the terminal(Tested in lucid linux).


**User Instructions:**

* At the moment ,only query type:A is supported in the implementation.If an alias name is found in the
answer records, it is also sent to the client.

**To start the server**
 
* `./server (desired port) (1=daemon /2=not daemon) (directory of server) (ip address of name server) (timeout for the name server)`


* For example:

For daemon(background process)
* `./server 2400 1 /Desktop/Dns 8.8.8.8`

For starting server without daemon for debugging purposes.
* `./server 2400 2 /Desktop/Dns 8.8.8.8 4`

The server will send the query for the name server to the address mentioned in the terminal.In this case
it will send it to 8.8.8.8.If there is no response from the name server , it will wait for 4 seconds and then
respond to the client with no answer.

The response from the dns proxy server is saved in the “results.txt” file in the local directory of the httpclient implementation.You can see the results by using “nano results.txt”.Some of the results may also be seen in the terminal(first recv is printed on terminal)

PSY: 8.8.8.8 is a google name server , you can change it to some other name server as well

-![alt text] (http://giphy.com/gifs/oirLISmToyoeI)
