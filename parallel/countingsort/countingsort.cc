/*
  Filename   : lab5.cc
  Author     : Brody Troutman
  Course     : CSCI 476-01
  Assignment : Assignemnt 5: A Problem of a Different Sort
  Description: Write a multithreaded, parallel countsort
  	and compare against a serial countsort and std::sort
*/   

/************************************************************/
// System includes

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <thread>
#include <boost/thread/barrier.hpp>

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
using std::ref;
using boost::barrier;

/************************************************************/
// Function prototypes/global vars/typedefs

// Serial counting sort of 'v'
void
countingSort (vector<unsigned>& v,unsigned k);

// Parallel counting sort of 'v'
void
parCountingSort (vector<unsigned>& v,unsigned cutoff, unsigned k, unsigned p);

string
convertVectorToString (const vector<unsigned>& data);

//Count subsection of v, store in count & prefix sum count
void
countLocal (const vector<unsigned>& v, unsigned vStart, unsigned vEnd, vector<unsigned>& count);

//Add prefix sum of an element of each count vector to master prefix sum (counts[0])
void
reduce (vector<vector<unsigned>>& counts, unsigned id, unsigned cStart, unsigned cEnd);

//Fill v with values based on master prefix vector. v is now sorted. 
void
populate (vector<unsigned>& v, const vector<unsigned>& count, unsigned cStart, unsigned cEnd);

//Main function for each thread, to break routine into logical parts. 
void
threadRoutine (vector<unsigned>& v, vector<vector<unsigned>>& counts,unsigned id, barrier& barrier);
/************************************************************/

int      
main (int argc, char* argv[]) 
{        
  cout << endl;

  unsigned N, k, p, cutoff;
  
  cout << "N      ==> ";
  cin >> N;

  cout << "k      ==> ";
  cin >> k;

  cout << "p      ==> ";
  cin >> p;

  cout << "Cutoff ==> ";
  cin >> cutoff;
  cout << endl;

  vector<unsigned> stdV (N);

  std::random_device rd;
  std::mt19937 mr (rd ());
  std::generate_n (stdV.begin (), stdV.size (), [&](){return (mr () % k);});

  vector<unsigned> parallelV (stdV);
  vector<unsigned> serialV (stdV);

  Timer<> timer;

  timer.start ();
  parCountingSort (parallelV, cutoff, k, p);
  timer.stop ();
  printf ("Parallel time:  %8.2f ms\n", timer.getElapsedMs ());
  
  timer.start ();
  countingSort (serialV, k);
  timer.stop ();
  printf ("Serial time:    %8.2f ms\n", timer.getElapsedMs ());

  timer.start ();
  std::sort (stdV.begin (), stdV.end ());
  timer.stop ();
  printf ("std::sort time: %8.2f ms\n\n", timer.getElapsedMs ());

  cout << std::boolalpha << "Parallel ok? " << (stdV == parallelV) << endl;
  cout << std::boolalpha << "Serial ok?   " << (serialV == stdV) << endl << endl;

  return EXIT_SUCCESS;  
}

/************************************************************/
//Function Implementation

string
convertVectorToString (const vector<unsigned>& data) {
  string value = "{ ";
  for (size_t i = 0; i < data.size () - 1; ++i) 
  {
      value += std::to_string (data[i]) + ", ";
  }
  value += std::to_string (data[data.size () - 1]) + " }";
  return value;
}

/************************************************************/

// Serial counting sort of 'v'
void
countingSort (vector<unsigned>& v,unsigned k){
  vector<unsigned> count (k,0);
  //Count(tally) each value found
	for (auto e : v)
	{
    ++count[e];
	}

  //Prefix sum
  for (unsigned i = 1; i < count.size (); ++i)
  {
    count[i] += count[i-1];
  }

  //Dump into original, special case on 0
  std::fill (v.begin (), v.begin () + count[0],0);
	for (unsigned i = 1; i < count.size (); ++i)
	{
		std::fill(v.begin () + count[i-1], v.begin () + count[i], i);
	}
}

/************************************************************/

// Parallel counting sort of 'v'
void
parCountingSort (vector<unsigned>& v,unsigned cutoff, unsigned k, unsigned p)
{
  if (v.size () <= cutoff)
  {
    countingSort (v,k);
  }
  else 
  {
    vector<thread> myThreads (p);
    vector<vector<unsigned>> counts (p, vector<unsigned> (k,0));
    barrier barrier (p);

	  for (unsigned i = 0; i < p; ++i)
    {
      myThreads[i] = thread (threadRoutine, ref (v), ref (counts), i, ref (barrier));
    }
    for (thread& t : myThreads)
    {
      t.join ();
    }
  }
}

/************************************************************/

//Count subsection of v, store in count & prefix sum count
void
countLocal (const vector<unsigned>& v, unsigned vStart, unsigned vEnd, vector<unsigned>& count)
{
	for (unsigned i = vStart; i < vEnd; ++i)
  {
    ++count[v[i]];
  }

  //Prefix sum own local count **not significant enough for own function?**
  for (unsigned i = 1; i < count.size (); ++i)
  {
    count[i] += count[i-1];
  }
}

/************************************************************/

//Add prefix sum of an element of each count vector to master prefix sum (counts[0])
void
reduce (vector<vector<unsigned>>& counts, unsigned id, unsigned cStart, unsigned cEnd)
{
  //Slice up each vector into elements per thread instead of each vector for each thread since data races. 
  //Temp var, partialPrefix, for temporal locality.
  unsigned partialPrefix = 0;
  for (unsigned i = cStart; i < cEnd; ++i)
  {
    for (vector<unsigned>& pCount : counts)
    {
      partialPrefix += pCount[i];
    }
    counts[0][i] = partialPrefix;
    partialPrefix = 0;
  }
  
  //Data race :(
  //std::transform (counts[0].begin (), counts[0].end (), counts[id].begin (), counts[0].begin (), std::plus<unsigned> ());
}

/************************************************************/

//Fill v with values based on master prefix vector. v is now sorted. 
void
populate (vector<unsigned>& v, const vector<unsigned>& count, unsigned cStart, unsigned cEnd)
{
  for (unsigned i = cStart; i < cEnd; ++i)
  {
    //0 Edgecase, since must start at vector.begin () instead of from where last fill ended. 
    if (i == 0)
    {
      std::fill (v.begin (), v.begin () + count[0], 0);
    }
    else 
    {
      std::fill (v.begin () + count[i-1], v.begin () + count[i], i);
    }
  }
}

/************************************************************/

//Main function for each thread, to break routine into logical parts. 
void
threadRoutine (vector<unsigned>& v, vector<vector<unsigned>>& counts,unsigned id, barrier& barrier)
{
  unsigned vStart = ( id * v.size () ) / counts.size ();
  unsigned vEnd = ( ( id + 1 ) * v.size () ) / counts.size ();
  countLocal (v, vStart, vEnd, counts[id]);
  barrier.wait ();
  unsigned cStart = (id * counts[0].size () ) / counts.size ();
  unsigned cEnd = ( ( id + 1 ) * counts[0].size () ) / counts.size ();
  reduce (counts,id, cStart, cEnd);
  barrier.wait ();
  populate (v, counts[0], cStart, cEnd);
}

