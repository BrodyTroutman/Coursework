/*
  Filename   : ArrayDriver.cc
  Author     : Gary M. Zoppetti / Brody Troutman
  Course     : CSCI 362-01
  Assignment : Program2 : Array Class
  Description: Test methods of the Array class
*/   

/************************************************************/
// System includes

#include <cstdlib>
#include <iostream>
#include <string>
#include <iterator>
#include <sstream>
#include <cassert>

/************************************************************/
// Local includes

#include "Array.hpp"

/************************************************************/
// Using declarations

using std::cin;
using std::cout;
using std::endl;
using std::ostream_iterator;
using std::string;
using std::ostringstream;

/************************************************************/
// Function prototypes/global vars/typedefs

void
printTestResult (const string& test,
		 const string& expected,
		 const ostringstream& actual);

void
print (Array<int> A);

/************************************************************/

int      
main (int argc, char* argv[]) 
{        
  Array<int> A (3);

  // For holding the actual result
  ostringstream output;
  // Put the actual result into the output stream
  output << A;
  printTestResult ("size ctor", "[ 0 0 0 ]", output);

  A.push_back (5);
  A.push_back (10);
  A.push_back (15);

  // Must clear the output stream each time
  output.str ("");
  output << A;
  printTestResult ("push_back", "[ 0 0 0 5 10 15 ]", output);

  output.str ("");
  output << A.size ();
  printTestResult ("size", "6", output);
  
  A.pop_back ();
  A.pop_back ();

  output.str ("");
  output << A;
  printTestResult ("pop_back", "[ 0 0 0 5 ]", output);
  //--------------------------------------------
  // Convert rest of code to use printTestResult
  //   and add your own tests
  //--------------------------------------------
  
  //Test Insert beginning 10x
  output.str ("");
  for (int i = 0; i < 10; ++i)
  {
    A.insert (A.begin (), i);
  }
  cout << "Inserted 9 8 ... 0 at beginning\n";
  output << A;
  printTestResult ("10x insert begin", "[ 9 8 7 6 5 4 3 2 1 0 0 0 0 5 ]", output);

  //Test Delete all (for loop)
  output.str ("");
  for (Array<int>::iterator i = A.begin (); i != A.end (); )
  {
    i = A.erase (i);
  }
  cout << "Erased all elements of A\n";
  output << A;
  printTestResult ("EraseAll", "[ ]", output);
  
  //Test Fill Ctor
  output.str ("");
  Array<int> B (3, 9);
  cout << "B (3, 9): 3 9's\n";
  output << B;
  printTestResult ("FillCtor", "[ 9 9 9 ]", output);

  //Test Range Ctor
  output.str ("");
  Array<int> C (B.begin (), B.begin () + 2);
  cout << "C (B.begin (), B.begin () + 2): 9 9\n";
  output << C;
  printTestResult ("RangeCtor", "[ 9 9 ]", output);
  
  //Test Operator=
  output.str ("");
  B = A;
  cout << "B = A: empty\n";
  output << B;
  printTestResult( "=EmptyArray", "[ ]", output);
  
  //Insert beginning empty
  output.str ("");
  cout << "Inserting 0, 1, 2, 3, 4 at beginning of B\n";
  for (int i = 0; i < 5; ++i)
    B.insert (B.begin (), i);
  output << B;
  printTestResult ("insertBeginningEmpty", "[ 4 3 2 1 0 ]", output);

  cout << "Printing B using the output operator\n";
  cout << B << endl;
  cout << endl;
  
  //*********************************************************/
  //Added Tests
  //*********************************************************/
  
  //Test noArg Ctor
  output.str ("");
  Array<int> D;
  output << D;
  printTestResult ("emptyCtor", "[ ]", output);
  
  //Test Range Ctor
  output.str ("");
  Array<int> E (B.begin () + 1, B.begin () + 4);
  output << E;
  printTestResult ("RangeCtor", "[ 3 2 1 ]", output);
  
  //destructor call
  cout << "Dtor test\n";
  for (size_t i = 0; i < 1; ++i)
  {
	  Array<int> F (5, 5);
  }
  
  //Capacity Check
  output.str ("");
  
  Array<int> G;
  output << G.capacity();
  printTestResult ("0 capacity", "0", output);
  
  G.push_back(1);
  output << G.capacity();
  printTestResult ("1 capacity", "01", output);
  
  G.push_back(2);
  output << G.capacity();
  printTestResult ("2 capacity", "012", output);
  
  G.push_back(4);
  output << G.capacity();
  printTestResult ("4 capacity", "0124", output);
  
  G.push_back(4);
  G.push_back(8);
  output << G.capacity();
  printTestResult ("8 capacity", "01248", output); 
  
  //Size Check
  G.push_back(8);
  output.str("");
  output << G.size();
  printTestResult ("6 size", "6", output); 
  
  output.str("");
  G.push_back(8);    
  G.push_back(8);
  output << G.size();
  printTestResult ("8 size", "8", output);
  
  //Reserve smaller
  output.str("");
  G.reserve(5);
  output << G.capacity();
  printTestResult ("reserveLess", "8", output);
  
  //Reserve Bigger
  output.str("");
  G.reserve(20);
  output << G.capacity();
  printTestResult ("reserveMore", "20", output);
  
  //Resize Smaller
  output.str("");
  G.resize(2);
  output << "Size: " << G.size() << " Array: " << G;
  printTestResult ("resizeSmaller", "Size: 2 Array: [ 1 2 ]", output);
  
  //Resize Bigger
  output.str("");
  G.resize(5);
  output << "Size: " << G.size() << " Array: " << G;
  printTestResult ("resizeBigger", "Size: 5 Array: [ 1 2 0 0 0 ]", output);
  
  //Dereference Const
  output.str("");
  Array<int>::const_iterator constBegin = G.begin();
  Array<int>::const_iterator constBack = G.end() - 1;
  output << *constBegin << " " << *constBack;
  printTestResult ("consIters", "1 0", output);
  
  //Dereference
  output.str("");
  Array<int>::iterator iterBegin = G.begin();
  Array<int>::iterator iterBack = G.end()-1;
  output << *iterBegin << " " << *iterBack;
  printTestResult ("deref", "1 0" , output);

  
  return EXIT_SUCCESS;
}

/************************************************************/

void
printTestResult (const string& test,
		 const string& expected,
		 const ostringstream& actual)
{
  cout << "Test: " << test << endl;
  cout << "==========================" << endl;
  cout << "Expected: " << expected << endl;
  cout << "Actual  : " << actual.str () << endl;
  cout << "==========================" << endl << endl;

  // Ensure the two results are the same
  assert (expected == actual.str ());
}

/************************************************************/

// Copy ctor will get called as A is passed by value
void
print (Array<int> A) 
{
  cout << "[ ";
  // Using a for-each loop, which employs iterators
  for (const auto& e : A)
  {
    cout << e << " ";
  }
  cout << "]";
}

/************************************************************/