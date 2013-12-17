SERVER: myserver.c
	g++ -o server -Wall -pedantic  myserver.c server_put.c server_get.c dnsquery.c dnsfunctions.c
clean:
	rm -rf server
