// Verse class function definitions
// Computer Science, MVNU

#include "Verse.h"
#include <iostream>
using namespace std;

// Default constructor
Verse::Verse() {
    verseText = "Uninitialized Verse!";
    verseRef = Ref();
} 

// Parse constructor - pass verse string from file
Verse::Verse(const string s) {
	verseRef = Ref(s);
	verseText = s.substr(s.find(" ") + 1);
}  	

// Accessors
string Verse::getVerse() {
	return verseText;
}
Ref Verse::getRef() {
	return verseRef;
}

// display reference and verse
void Verse::display() {
    verseRef.display();
    cout << " " << getVerse();
 }
