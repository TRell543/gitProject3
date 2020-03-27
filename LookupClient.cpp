// LookupClient.cpp
// CGI program to process an AJAX request from a web interface
// Author: Terrell Gingerich

#define logging
#define LOG_FILENAME "/tmp/tergingerich-LookupClient.log"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "logfile.h"
#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include "fifo.h"
#include <stdio.h>                                                              
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sstream>

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"
using namespace cgicc;
                                                   
string receive_pipe = "bibleReply";
string send_pipe = "bibleRequest";

int main(){
#ifdef logging 
	logFile.open(logFilename.c_str(), ios::out);
#endif
	cout << "Content-Type: text/plain\n\n";
	Cgicc cgi;
	// retrieves the data enter by the user n the web interface
	form_iterator st = cgi.getElement("search_type");
	form_iterator book = cgi.getElement("book");
	form_iterator chapter = cgi.getElement("chapter");
	form_iterator verse = cgi.getElement("verse");
	form_iterator nv = cgi.getElement("num_verse");
	//	get integer value from nv iterator to determine number of verses
	int numOfVerses = nv->getIntegerValue();
	// create the message in format "book:chapter:verse,numOfVerses"
	string message = **book + ":" + **chapter + ":" + **verse + "," + **nv;
	// create pipes
	Fifo recfifo(receive_pipe);
	Fifo sendfifo(send_pipe);
	// open request pipe
	sendfifo.openwrite();
	log("open request pipe");
	// send the requested reference to server
	sendfifo.send(message);
	log("Request: " + message);
	// open the reply pipe
	recfifo.openread();
	log("open reply pipe");
	
	message = recfifo.recv();	// retrieve the status code from server
	int code = atoi(message.c_str());	// convert from string to integer
	LookupResult result = static_cast<LookupResult>(code);	// convert from integer to LookupResult
	// code for successful lookup
	if(result == SUCCESS){
		cout << "Search Type: <b>" << **st << "</b>" << endl;	// prints search type
		cout << "<p><b>RESULT:</p></b>";	// prints RESULT: in bold
		message = recfifo.recv();	// retrieve verse and reference from server
		log("Reply: " + message);
		Verse verse(message);	// create verse from the message received
		Ref ref = verse.getRef();	// create Ref object from verse
		// print first reference and verse to web interface
		cout << "<p><b>" << ref.getBookName(ref.getBook()) << " " << ref.getChap() << ":</b></p>";
		cout << "<p>" << "<em> " << ref.getVerse() << "</em>" << " " << verse.getVerse();
		numOfVerses--;	// decrease number of verses left by 1
		// loop for multiple verse retrieval
		while(numOfVerses > 0){
			message = recfifo.recv();	// receive next reference and verse
			log("Reply: " + message);
			Verse nextVerse(message);	// create next verse from the message
			ref = nextVerse.getRef();	// update the reference object
			if(ref.getVerse() == 1)	// if the next verse is 1, print book and chapter
				cout << "</p><p><b>" << ref.getBookName(ref.getBook()) << " " << ref.getChap() << ":" << "</b></p>";
			cout << "<em> " << ref.getVerse() << "</em>" << " " << nextVerse.getVerse();	// prints verse number and next verse text
			numOfVerses--;	// decrease number of verses left by 1
		}
	}
	// code for NO_CHAPTER or NO_VERSE status code received
	if(result == NO_CHAPTER || result == NO_VERSE){
		message = recfifo.recv();	// retrieve the appropriate status code error message
		log("Reply: " + message);
		cout << "<p><b>" << message << "</b></p>" << endl;	// print the error message
	}
	// close reply pipe
	recfifo.fifoclose();
	log("close reply fifo");
	// close request pipe
	sendfifo.fifoclose();
	log("close request fifo");
	return 1;
}