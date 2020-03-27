# Makefile for Bible reader program using classes
# Use GNU c++ compiler with C++11 standard
CC= g++
CFLAGS= -g -std=c++11

all: lookupserver lookupclient PutCGI PutHTML

Ref.o : Ref.h Ref.cpp
	$(CC) $(CFLAGS) -c Ref.cpp

Verse.o : Ref.h Verse.h Verse.cpp
	$(CC) $(CFLAGS) -c Verse.cpp

Bible.o : Ref.h Verse.h Bible.h Bible.cpp
	$(CC) $(CFLAGS) -c Bible.cpp
	
fifo.o: fifo.cpp fifo.h
	$(CC) $(CFLAGS) -c fifo.cpp

lookupserver.o: LookupServer.cpp fifo.h Bible.h
	$(CC) $(CFLAGS) -c LookupServer.cpp

lookupclient.o: LookupClient.cpp fifo.h
	$(CC) $(CFLAGS) -c LookupClient.cpp

lookupserver: LookupServer.o fifo.o Bible.o Ref.o Verse.o
	$(CC) $(CFLAGS) -o lookupserver LookupServer.o fifo.o Bible.o Ref.o Verse.o

lookupclient: LookupClient.o fifo.o Bible.o Ref.o Verse.o
	$(CC) $(CFLAGS) -o lookupclient LookupClient.o fifo.o Bible.o Ref.o Verse.o -lcgicc
	
PutCGI: lookupclient
		chmod 757 lookupclient
		cp lookupclient /var/www/html/class/csc3004/tergingerich/cgi-bin/
		echo "Current contents of your cgi-bin directory: "
		ls -l /var/www/html/class/csc3004/tergingerich/cgi-bin/

PutHTML: bibleajax.html
		cp bibleajax.html  /var/www/html/class/csc3004/tergingerich
		echo "Current contents of your HTML directory: "
		ls -l  /var/www/html/class/csc3004/tergingerich

clean:
	rm *.o *# *~ lookupclient lookupserver