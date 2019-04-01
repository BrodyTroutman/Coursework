/*
  Filename   : searchTreeDriver.cc
  Author     : Brody Troutman
  Course     : CSCI 362-01
  Assignment : Lab6 : BST
  Description: Fail at writing a BST class. 
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

#include "searchtree.hpp"

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
	
	
  ostringstream output;
  SearchTree<int> treeA;
  output << treeA;
  printTestResult ("NoArg Ctor", "[ ]", output);
  
  output.str ("");
  treeA.insert (100);
  treeA.insert (200);
  treeA.insert (50);
  output << treeA;
  printTestResult ("insert", "[ 50 100 200 ]", output);

  //output.str ("");
  //SearchTree<int> treeB (treeA);
  //output << treeB;
  //printTestResult ("copyCtor", "[ 50 100 200]", output);
  
  output.str ("");
  output << treeA.size ();
  printTestResult ("size ()", "3", output);
  
  output.str ("");
  output << treeA.empty ();
  printTestResult ("empty ()", "0", output);
	
  output.str ("");
  output << *treeA.begin ();
  printTestResult ("begin ()", "50", output);
  
  output.str ("");
  auto postIncre = treeA.begin ()++;
  output << *postIncre;
  printTestResult ("PostIncrement", "50", output);
  
  //output.str("");
  //auto temp = ++treeA.begin();
  //auto postDecre = temp--;
  //output << *postDecre;
  //printTestResult ("PostDecrement", "100", output);
  
  //output.str ("");
  //output << *++treeA.end();
  //printTestResult ("end ()", "50", output);
  
  //output.str("");
  //auto finder = treeA.find(200);
  //output << *finder;
  //printTestResult ("find ()", "200", output);
  
  
  output.str ("");
  treeA.erase(50);
  output << treeA;
  printTestResult ("erase leaf", "[ 100 200 ]", output);
  
  output.str ("");
  treeA.erase (100);
  output << treeA;
  printTestResult ("erase w/ right child", "[ 200 ]", output);
  
  output.str ("");
  treeA.insert(100);
  treeA.erase (100);
  output << treeA;
  printTestResult ("erase w/ left child", "[ 200 ]", output);
  




  
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