// testreader.cpp
// command line test program for project3
// Author: Terrell Gingerich
// Date: 2-20-20

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char** argv) {
	// Create Bible object to process the raw text file
	Bible webBible("/home/class/csc3004/Bibles/web-complete");
	int numOfVerses = 1;

	// Error if no book, chapter, and verse numbers are entered
	if (argc == 1) {
		cerr << "Error: book, chapter, and verse numbers are missing" << endl;
		exit(1);
	}
	// Book variable initialized to 2nd parameter entered
	int b = atoi(argv[1]);

	// Error if no chapter and verse number are entered
	if (argc == 2) {
		cerr << "Error: chapter and verse numbers are missing" << endl;
		exit(2);
	}
	// Chapter variable initialized to 3rd parameter entered
	int c = atoi(argv[2]);

	// Error if no verse number is entered
	if (argc == 3) {
		cerr << "Error: verse number is missing" << endl;
		exit(3);
	}
	// Verse variable intialized to 4th parameter entered
	int v = atoi(argv[3]);

	// Input for number of verses to search after first verse reference
	if (argc != 4) 
		numOfVerses = atoi(argv[4]);

	// Displays error message if book is not in range 1 to 66
	if (b < 1 || b > 66) {
		cerr << "Error: no such book " << b << endl;
		exit(1);
	}
	
	// Create a reference from input
	Ref ref(b, c, v);
	LookupResult result = OTHER;
	
	// Look up first verse, display it, and decrease numOfVerses by 1
	Verse verse = webBible.lookup(ref, result);
	if (result == SUCCESS) {
		verse.display();
		cout << endl;
		numOfVerses--;
	}
	
	// Check if result is set to NO_CHAPTER and if so, display error message
	if (result == NO_CHAPTER) {
		cerr << webBible.error(result) << c << " in " << ref.getBookName(b) 
			 << " or no such verse " << v << " in " << ref.getBookName(b) << " " << c << endl;
		exit(0);
	}

	// Check if result is set to NO_VERSE and if so, display error message
	if (result == NO_VERSE) {
		cerr << webBible.error(result) << v << " in " << ref.getBookName(b) << " " << c << endl;
		exit(0);
	}
	
	// If fourth parameter is greater than 0, get the next verse, look it up, and display it with only verse number
	while (numOfVerses > 0) {
		result = OTHER;
		ref = webBible.next(ref);
		Verse nextVerse = webBible.lookup(ref, result);
		cout << ref.getVerse() << " " << nextVerse.getVerse() << endl;
		numOfVerses--;
	}
}