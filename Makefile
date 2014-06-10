SERVER: myserver.c
	g++ -o server -Wall -pedantic  myserver.c server_put.c server_get.c dnsquery.c dnsfunctions.c server_register.c signal_handler.c
clean:
	rm -rf server
