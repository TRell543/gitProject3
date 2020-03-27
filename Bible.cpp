// Bible class function definitions
// Computer Science, MVNU

#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

// Default constructor
Bible::Bible() {
	infile = "/home/class/csc3004/Bibles/web-complete";
}

// Constructor – pass bible filename
Bible::Bible(const string s) { 
	infile = s; 
	buildIndex(infile);
}

// Builds an index from the Bible file using a map
const int Bible::buildIndex(string file){
	instream.open(file.c_str());
	if(!instream){
		cerr << "Error: could not open the file " << file << endl;
		return 0;
	}
	// Adds the reference of the currentLine and position of that reference to the map
	while(!instream.fail()){
		int pos = instream.tellg();
		getline(instream, currentLine);
		if(instream.fail())
			break;
		Ref ref(currentLine);
		bibleRefs[ref] = pos;
	}
	instream.close();
	cout << "References: " << bibleRefs.size() << endl;
	return 1;
}

// Lookup finds a given verse reference in the Bible file
const Verse Bible::lookup(Ref ref, LookupResult& status) {
	instream.open(infile);
	if(!instream){
		cerr << "Error: could not open the file " << infile << endl;
		exit(0);
	}
 	if (bibleRefs.find(ref) != bibleRefs.end()) {
     	instream.seekg(bibleRefs[ref]);
     	getline(instream, currentLine);
		Ref currentRef(currentLine);
		if(currentRef.operator==(ref))
			status = SUCCESS;
 	} else {
		Ref testRef(ref.getBook(), ref.getChap(), 1);
		if(bibleRefs.find(testRef) != bibleRefs.end())
			status = NO_VERSE;
		else
			status = NO_CHAPTER;
	}
	instream.close();
	Verse aVerse(currentLine);
	return aVerse;
}

// Return the next reference after the given reference
const Ref Bible::next(Ref ref, LookupResult& status) {
	instream.open(infile);
	if(!instream){
		cerr << "Error: could not open the file " << infile << endl;
		exit(0);
	}
	instream.seekg(bibleRefs[ref]);
	getline(instream, currentLine);
	getline(instream, currentLine);
	instream.close();
	Ref nextRef(currentLine);
	Ref testRef(nextRef.getBook(), nextRef.getChap(), 1);
	if(bibleRefs.find(testRef) != bibleRefs.end())
		status = NO_VERSE;
	else if(bibleRefs.find(testRef) == bibleRefs.end())
		status = NO_CHAPTER;
	else
		status = SUCCESS;
	return nextRef;
}

// Return an error message string to describe status
const string Bible::error(LookupResult status) {
	if (status == NO_CHAPTER)
		return "Error: no such chapter ";
	if (status == NO_VERSE)
		return "Error: no such verse ";
}