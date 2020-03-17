// Ref class function definitions
// Computer Science, MVNU

#include "Ref.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// GetNextToken - general routine for getting next token from a string
// Parameters
//    str - string to search. search is destructive, i.e., token is removed.
//    delimiters - string, a list of characters to use a delimiters between tokens
// Returns: string token (removed from str)

string GetNextToken(string& str, const string& delimiters = " ") {
  // Skip delimiters at beginning                                          
  int startPos = str.find_first_not_of(delimiters, 0);
  // Find position of delimiter at end of token                                     
  int endPos = str.find_first_of(delimiters, startPos);

  // Found a token, remove it from string, and return it                       
  string next = str.substr(startPos, endPos - startPos);
  string rest = str.substr(endPos - startPos + 1, string::npos);
  str = rest;
  return(next);
}

// Ref member functions

// Default constructor
Ref::Ref() {book = 0; chap = 0; verse = 0;}

Ref::Ref(const string s) { // Parse constructor - receives a line "34:5:7 text"
    string rtext = s; // make local copy of string to avoid modifying parameter
    // parse the reference - notice, currently there is no error checking!
    // Get book number
    string strbook = GetNextToken(rtext,":");
    book = atoi(strbook.c_str());
    // Get the chapter number
    string strchap = GetNextToken(rtext,":");
    chap = atoi(strchap.c_str());
    // Get the verse number
    string strverse = GetNextToken(rtext," ");
    verse = atoi(strverse.c_str());
}

// Construct Ref from three integers
Ref::Ref(const int b, const int c, const int v) {
	book = b;
	chap = c;
	verse = v;
}

// Accessors
int Ref::getBook() const {return book;}	    // Access book number
int Ref::getChap() const {return chap;}	    // Access chapter number
int Ref::getVerse() const {return verse;}   // Access verse number

// Comparison function to check if given reference is equal to desired reference
bool Ref::operator==(const Ref r) {
	if(r.getBook() == getBook()){   // check if given book is equal to desired book
        if (r.getChap() == getChap()) {   // check if given chapter is equal to desired chapter
            if (r.getVerse() == getVerse()) { // check if given verse is equal to desired verse
                return true;    // return true if all conditions are true
            }
        }
	}
	return false;   // return false otherwise
}

// Comparison function to check if given reference is less than desired reference
bool Ref::operator<(const Ref r) const{
	if(getBook() < r.getBook())	// returns true if desired book is less than given book
		return true;
	if(getBook() == r.getBook() && getChap() < r.getChap())	// returns true if desired chapter is less than given chapter
		return true;
	if(getBook() == r.getBook() && getChap() == r.getChap() && getVerse() < r.getVerse())	// returns true if desired verse is less than given verse
		return true;
	return false;	// return false otherwise
}

// Displays name of book and chapter number on one line and the verse number on the next
void Ref::display() {
    cout << getBookName(book) << " " << chap << endl;
    cout << verse;
}

string Ref::getBookName(int book) {
    // Create vector, string object, and ifstream object to read the file
    vector<string> chapNamesVector;
    string line;
    ifstream chapNamesFile("NumberedBibleBooks.txt");
    // Check if file is open
    if (chapNamesFile.is_open()) { 
        for (int i = 0; i < 67; i++) {  // loop to read in each line of file into an array
            getline(chapNamesFile, line);
            int n = line.find_first_of('\r');
            line = line.substr(0, n);
            chapNamesVector.push_back(line);
        }
        chapNamesFile.close();
    }
    // Error message dsiplayed if file was not opened
    else
        cout << "Unable to open the file to convert from book number to book name" << endl;
    // Return specific book name
    return chapNamesVector.at(book);
}
