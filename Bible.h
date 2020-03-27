// Bible.h
// header file to Bible.cpp
// Edited by Terrell Gingerich

#ifndef Bible_H
#define Bible_H

#include "Ref.h"
#include "Verse.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <map>
using namespace std;

// status codes to be returned when looking up a reference
enum LookupResult { SUCCESS, NO_CHAPTER, NO_VERSE, OTHER };

// A class to represent a version of the bible
class Bible {
 private:
   string infile;			// file path name
   ifstream instream;		// input stream, used when file is open
   bool isOpen;				// true if file is open
   string currentLine;  	// reads currentLine from file
   map<Ref, int> bibleRefs;	// map index with all references of Bible

 public:
   Bible();													// Default constructor
   Bible(const string s); 									// Constructor – pass name of bible file
   const int buildIndex(string file);						// builds an index of Bible references
   const Verse lookup(const Ref ref, LookupResult& status);	// Find and return a verse in this Bible, given a reference
   const Ref next(const Ref ref, LookupResult& status);							// Return the next reference
   const string error(LookupResult status);				// returns an error message for invalid reference
};
#endif //Bible_H
