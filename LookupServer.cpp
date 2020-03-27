// LookupServer.cpp
// background server process to create the map index and handles requests from LookupClient.cpp
// Author: Terrell Gingerich

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include "fifo.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include <list>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

// names of pipes
string receive_pipe = "bibleRequest";
string send_pipe = "bibleReply";

string convertInt(int number){
	stringstream ss;	//create a stringstream
	ss << number;		//add number to the stream
	return ss.str();	//return a string with the contents of the stream
}

int main() {
	// create the pipes
	Fifo recfifo(receive_pipe);
	Fifo sendfifo(send_pipe);
	// creates a bibleFile object with specified Bible file
	Bible bibleFile("/home/class/csc3004/Bibles/web-complete");
	// open request pipe
	recfifo.openread();
	// infinite loop for lookup of references
	while(true){
		string message = recfifo.recv();	// receive reference from client
		int numOfVerses = 1;	// create variable to hold number of verses to lookup; initialized to 1
		size_t found = message.find(',');	// find ',' in message received
		if(found != string::npos){	// if found...
			string nv = message.substr(message.find(',') + 1);	// retrieve the number of verses to lookup
			numOfVerses = atoi(nv.c_str());	// change from string to integer
			message.erase(message.find(","));	// erase everything after the ',' including the ','
		}
		Ref ref(message);	// create a Ref object from message
		LookupResult result = OTHER;	// set status variable to OTHER
		Verse verse = bibleFile.lookup(ref, result);	// lookup the verse with requested reference
		// open reply pipe
		sendfifo.openwrite();
		// code for SUCCESS lookup
		if(result == SUCCESS){
			message = "0";	// integer value for SUCCESS
			sendfifo.send(message);	// send SUCCESS status message to client
			string b = convertInt(ref.getBook());	// convert book int to string
			string c = convertInt(ref.getChap());	// convert chapter int to string
			string v = convertInt(ref.getVerse());	// convert verse int to string
			message = b + ":" + c + ":" + v + " " + verse.getVerse();
			sendfifo.send(message);	// send reference and verse to client
			numOfVerses--;	// subtract one from numberOfVerses variable to lookup
			// loop for multiple verse lookup
			while(numOfVerses > 0){
				result = OTHER;	// set result back to OTHER
				ref = bibleFile.next(ref, result);	// retrieve next reference
				verse = bibleFile.lookup(ref, result);	// lookup the reference
				string b = convertInt(ref.getBook());	// convert book int to string
				string c = convertInt(ref.getChap());	// convert chapter int to string
				string v = convertInt(ref.getVerse());	// convert verse int to string
				message = b + ":" + c + ":" + v + " " + verse.getVerse();
				sendfifo.send(message);	// send reference and verse to client
				numOfVerses--;	// subtract one from numberOfVerses variable to lookup
			}
		}
		// code for NO_CHAPTER lookup
		if(result == NO_CHAPTER){
			message = "1";	// integer value for NO_CHAPTER
			sendfifo.send(message);	// send NO_CHAPTER to client
			string c = convertInt(ref.getChap());	// convert chapter int to string
			message = bibleFile.error(result) + c + " in " + ref.getBookName(ref.getBook());
			sendfifo.send(message);	// send error message to client
		}
		// code for NO_VERSE lookup
		if(result == NO_VERSE){
			message = "2";	// integer value for NO_VERSE
			sendfifo.send(message);	// send NO_VERSE to client
			string c = convertInt(ref.getChap());	// convert chapter int to string
			string v = convertInt(ref.getVerse());	// convert verse int to string
			message = bibleFile.error(result) + v + " in " + ref.getBookName(ref.getBook()) + " " + c;
			sendfifo.send(message);	// send error message to client
		}
		// close the reply pipe
		sendfifo.fifoclose();
	}
}