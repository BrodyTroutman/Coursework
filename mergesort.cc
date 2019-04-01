/*
  Filename   : lab5.cc
  Author     : Brody Troutman
  Course     : CSCI 476-01
  Assignment : Assignemnt 5: A Problem of a Different Sort
  Description: Write a multithreaded, parallel mergesort
  	and compare against a serial mergesort and std::sort
*/   

/************************************************************/
// System includes

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <thread>

/************************************************************/
// Local includes

#include "Timer.hpp"

/************************************************************/
// Using declarations

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::thread;

/************************************************************/
// Function prototypes/global vars/typedefs

template <typename Iter>
void
insertionSort (Iter first, Iter last);

template <typename Iter>
void
mergeSortSerial (Iter first, Iter last, unsigned cutoff);

template <typename Iter>
void
mergeSortParallel (Iter first, Iter last, unsigned cutoff, unsigned depth, unsigned level = 0);

string
convertVectorToString (vector<int> data);


/************************************************************/

int      
main (int argc, char* argv[]) 
{        
  cout << endl;

  unsigned n, cutoff, depth;

  cout << "N      ==> ";
  cin >> n;

  cout << "Cutoff ==> ";
  cin >> cutoff;

  cout << "Depth  ==> ";
  cin >> depth;
  cout << endl;

  vector<int> stdV (n);

  std::random_device rd;
  std::mt19937 mr (rd ());
  std::generate (stdV.begin (), stdV.end (), [&](){return (mr () % 1000);});

  vector<int> parallelV (stdV);
  vector<int> serialV (stdV);

  Timer<> tParallel;
  tParallel.start ();
  mergeSortParallel (parallelV.begin (), parallelV.end (), cutoff, depth, 0);
  tParallel.stop ();

  Timer<> tSerial;
  tSerial.start ();
  mergeSortSerial (serialV.begin (), serialV.end (), cutoff);
  tSerial.stop ();

  Timer<> tStd;
  tStd.start ();
  std::sort (stdV.begin (), stdV.end ());
  tStd.stop ();

  printf ("Parallel time:  %.2f ms\n", tParallel.getElapsedMs ());
  printf ("Serial time:    %.2f ms\n", tSerial.getElapsedMs ());
  printf ("std::sort time: %.2f ms\n", tStd.getElapsedMs ());
  cout << endl;
  cout << "Parallel ok? " << (stdV == parallelV ? "true" : "false") << endl;
  cout << "Serial ok?   " << (stdV == serialV ? "true" : "false") << endl << endl;

  return EXIT_SUCCESS; 
}

/************************************************************/
//Function Implementation

// Perform an insertion sort on the range [ first, last )
template <typename Iter>
void
insertionSort (Iter first, Iter last)
{
  for (Iter i = first + 1; i < last; ++i)
  {
    auto v = *i;
    auto j = i;
    while (j >= first + 1 && v < *(j - 1))
    {
      *j = *(j - 1);
      --j;
    }
    *j = v;
  }
}

// Perform a serial mergesort on the range [ first, last )
// Use insertion sort if the range size is LESS or EQUAL to 'cutoff'
template <typename Iter>
void
mergeSortSerial (Iter first, Iter last, unsigned cutoff)
{
	if (std::distance (first, last) > cutoff)
	{
    Iter mid = std::next(first, std::distance (first, last) / 2);
		mergeSortSerial (first, mid, cutoff);
		mergeSortSerial (mid, last, cutoff);
		std::inplace_merge (first, mid, last);
	}
	else 
	{
		insertionSort (first, last);
	}
}

// Perform a parallel mergesort on the range [ first, last )
// Use insertion sort if the range size is LESS or EQUAL to 'cutoff'
// If 'level' is LESS than 'depth' spawn a new thread for the left subrange,
//   o/w perform a SERIAL mergesort
template <typename Iter>
void
mergeSortParallel (Iter first, Iter last, unsigned cutoff, unsigned depth, unsigned level)
{
  if (std::distance (first, last) > cutoff)
	{
    Iter mid = std::next(first, std::distance (first, last) / 2);
    if (level < depth)
    { 
      thread t (mergeSortParallel<Iter>, first, mid, cutoff, depth, level + 1);
      mergeSortParallel (mid, last, cutoff, depth, level + 1);
      t.join ();
    } 
    else 
    {
      mergeSortSerial (first, mid, cutoff);
      mergeSortSerial (mid, last, cutoff);
    }
    std::inplace_merge (first, mid, last);
	}
	else 
	{
		insertionSort (first, last);
	}
}

string
convertVectorToString (vector<int> data) {
  string value = "{ ";
  for (size_t i = 0; i < data.size () - 1; ++i) 
  {
      value += std::to_string (data[i]) + ", ";
  }
  value += std::to_string (data[data.size () - 1]) + " }";
  return value;
}