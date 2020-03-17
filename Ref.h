// Class Ref
// Computer Science, MVNU
//
// Ref is a class for representing a Bible reference consisting of
//    * integer book   - the book from 1 (Genesis) to 66 (Revalation)
//    * integer chap   - the chapter number >1
//    * integer verse  - he verse numner >1
//

#ifndef Ref_H
#define Ref_H
#include <string> 
#include <stdlib.h>
#include <vector>
using namespace std;

// GetNextToken returns a token from str, which contains all characters
// up to the first character from delimiters
string GetNextToken(string& str, const string& delimiters);

class Ref {
private:
    // Reference information
   short book, chap, verse;
public:
   Ref();  	// Default constructor
   Ref(string s); 	// Parse constructor - example parameter "43:3:16"
   Ref(const int,const int,const int); 	// Construct from three integers;
   // Accessors
   int getBook() const;	// Access book number
   int getChap()const;	// Access chapter number
   int getVerse()const; // Access verse number
   // Comparison: determine if two references are equal
   bool operator==(const Ref);
   // Comparison: determine if one reference is less than another
   bool operator<(const Ref) const;
   // Retrieves book name from file and enters it into a vector
   string getBookName(int book);
   // Displays reference and verse appropriately
   void display();
};

#endif //Ref_H
