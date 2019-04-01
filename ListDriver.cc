/*
  Filename   : ListDriver.cc
  Author     : Gary M. Zoppetti / Brody Troutman
  Course     : CSCI 362-01
  Assignment : Program 3 : List Class
  Description: Test methods of the List class
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

#include "List.hpp"

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

/************************************************************/

int
main (int argc, char *argv[])
{
  //**************************************************************
  //Zoppetti Tests
  //**************************************************************
  List<int> A;

  // For holding the actual result
  ostringstream output;
  // Put the actual result into the output stream
  output << A;
  printTestResult ("no-arg ctor", "[ ]", output);

  // Must clear the output stream each time
  output.str ("");
  output << A.size ();
  printTestResult ("size", "0", output);
  
  //Test Size Constructor
  List<int> B (3, 8);
  output.str ("");
  output << B;
  printTestResult ("size ctor", "[ 8 8 8 ]", output);
  
  //*************************************************************

  //Test Initializer List Constructor
  List<int> C { 1, 2, 3, 4, 5, 6};
  output.str ("");
  output << C;
  printTestResult ("initializerList ctor", "[ 1 2 3 4 5 6 ]", output);
  
  //Test Copy Constructor
  List<int> D (C);
  output.str ("");
  output << D;
  printTestResult ("copy ctor", "[ 1 2 3 4 5 6 ]", output);
  
  //Test Operator=
  List<int> E;
  E = D;
  output.str ("");
  output << E;
  printTestResult ("Operator= bigger", "[ 1 2 3 4 5 6 ]", output);
  
  //Test Operator smaller
  E = B;
  output.str ("");
  output << E;
  printTestResult ("Operator= smaller", "[ 8 8 8 ]", output);
  
  //Test empty()
  output.str ("");
  string aEmpty = (A.empty () == 1 ? "true" : "false");
  string bEmpty = (B.empty () == 1 ? "true" : "false");
  output << aEmpty << " " << bEmpty;
  printTestResult ("empty()", "true false", output);
  
  //Test size()
  output.str ("");
  output << A.size () << " " << B.size ();
  printTestResult ("size()", "0 3", output);
  
  //Test front
  output.str ("");
  output << C.front ();
  printTestResult ("front()", "1", output);
  
  //Test back
  output.str ("");
  output << C.back ();
  printTestResult ("back()", "6", output);
  
  //push_front
  C.push_front (10);
  output.str ("");
  output << C;
  printTestResult ("push_front()", "[ 10 1 2 3 4 5 6 ]", output);
  
  //push_front Empty
  A.push_front (4);
  output.str ("");
  output << A;
  printTestResult ("push_front_empty()", "[ 4 ]", output);
  
  //pop_front
  C.pop_front ();
  output.str ("");
  output << C;
  printTestResult ("pop_front()", "[ 1 2 3 4 5 6 ]", output);
  
  //pop_front() 1 Element
  A.pop_front ();
  output.str ("");
  output << A;
  printTestResult ("pop_front 1 Element", "[ ]", output);
  
  //push_back
  C.push_back (20);
  output.str ("");
  output << C;
  printTestResult ("push_back", "[ 1 2 3 4 5 6 20 ]", output);
  
  //Test size()
  output.str ("");
  output << C.size ();
  printTestResult ("size()", "7", output);

  //push_back Empty
  A.push_back (6);
  output.str ("");
  output << A;
  printTestResult ("push_back_empty", "[ 6 ]", output);
  
  //pop_back 1 Element
  A.pop_back();
  output.str ("");
  output << A;
  printTestResult ("pop_back 1 Element", "[ ]", output);
  
  //Test size()
  output.str ("");
  output << A.size ();
  printTestResult ("size()", "0", output);
  
  //begin()
  output.str ("");
  output << *C.begin ();
  printTestResult ("begin()", "1", output);
  
  //end()
  output.str ("");
  output << (*--C.end ());
  printTestResult ("end()", "20", output);
  
  //insert middle
  C.insert (++C.begin (), 50);
  output.str ("");
  output << C;
  printTestResult ("insertMiddle", "[ 1 50 2 3 4 5 6 20 ]", output);
  
  //erase middle
  ListIterator<int> cEnd = --C.end ();
  C.erase (--cEnd);
  output.str ("");
  output << C;
  printTestResult ("eraseMiddle", "[ 1 50 2 3 4 5 20 ]", output);

  //erase range
  C.erase (++C.begin (), --C.end ());
  output.str ("");
  output << C;
  printTestResult ("eraseRange", "[ 1 20 ]", output);

  //increment iterators
  ListIterator<int> cBegin = C.begin ();
  ++cBegin; 
  output.str ("");
  output << *cBegin;
  printTestResult ("preincrement", "20", output);

  ListIterator<int> cBegin2 = cBegin++;
  output.str ("");
  output << *cBegin2;
  printTestResult ("postincrement", "20", output);

  //decrement iterators
  ListIterator<int> cEndd = C.end ();
  --cEndd;
  output.str ("");
  output << *cEndd;
  printTestResult ("preDecrement", "20", output);

  ListIterator<int> cEnd2 = cEndd++;
  output.str ("");
  output << *cEnd2;
  printTestResult ("postDecrement", "20", output);

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