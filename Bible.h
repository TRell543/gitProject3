// Class Bible
// Computer Science, MVNU
//
// A Bible object represents a particular version of the Bible
// A Bible object is constructed by giving it a file reference containing 
// the entire text of the version.

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
enum LookupResult { SUCCESS, NO_BOOK, NO_CHAPTER, NO_VERSE, OTHER };

class Bible {	// A class to represent a version of the bible
 private:
   string infile;		// file path name
   ifstream instream;	// input stream, used when file is open
   bool isOpen = false;	// true if file is open
   string currentLine;  // reads currentLine from file
   map<Ref, int> bibleRefs;

 public:
   Bible();	// Default constructor
   Bible(const string s); // Constructor – pass name of bible file
   const int buildIndex(string file);

   // Find and return a verse in this Bible, given a reference
   const Verse lookup(const Ref ref, LookupResult& status);
   // Return the next reference
   const Ref next(const Ref ref);
   // Return an error message string to describe status
   const string error(LookupResult status);
};
#endif //Bible_H
