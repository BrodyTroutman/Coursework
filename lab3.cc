/*
  Filename   : lab2.cc
  Author     : Brody Troutman
  Course     : CSCI 476-01
  Assignment : Assignment 2 : May the Forks be with You
  Description: Write parallel program to compute sum
                of a vector of integers.
                Skills: Process parallelism, Fork, Wait

    Threads Average:   25.976 ms
    Processes Average: 43.544 ms

    I believe threads are faster than processes for multiple reasons.
    Threads are inherently less resource intensive than processess since
    processes are comprised of one or more threads. 
    Threads are faster to create and destroy since the address space is not
    copied, unlike in processes where each process gets a copy.
    Threads communicate faster to one another since they do not use expensive
    pipes, and instead share memory.


*/

/************************************************************/
// System includes

#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <string>
#include <thread>

/************************************************************/
// Local includes

#include "Timer.hpp"

/************************************************************/
// Using declarations

using std::cout;
using std::endl;
using std::vector;
using std::cin;
using std::thread;

/************************************************************/
// Function prototypes/global vars/type definitions

int
computeParallelSum (const vector<int>& data, unsigned numProcessors);

void
computeMySum (const vector<int>& data, size_t myId, unsigned numProcessors, int* myPartialSum);

/************************************************************/
//Main

int
main (int argc, char* argv[])
{
    unsigned processors;
    cout << endl << "p ==> ";
    cin >> processors;
    cout << endl;
    

    size_t numElements;
    cout << "N ==> ";
    cin >> numElements;
    cout << endl;

    std::minstd_rand mr (0);
    std::uniform_int_distribution<> dis (0,4);
    vector<int> data (numElements);

    std::generate_n (data.begin (), numElements, [&]{return dis (mr);});
    Timer<> timerParallel;
    timerParallel.start ();
    int parallelSum = computeParallelSum (data, processors);
 	timerParallel.stop ();
    cout << "Parallel sum:  " << parallelSum << endl;
    printf ( "Parallel time: %.2f ms \n\n", timerParallel.getElapsedMs ());

    Timer<> timerSerial;
    timerSerial.start ();
    int serialSum = std::accumulate (data.begin (), data.end (), 0);
    timerSerial.stop ();
    cout << "Serial sum:    " << serialSum << endl;
    printf ("Serial time:   %.2f ms\n\n", timerSerial.getElapsedMs ());
}

/************************************************************/
//Function Implementation

int
computeParallelSum (const vector<int>& data, unsigned numProcessors)
{
    vector<thread> myThreads (numProcessors);
    vector<int> partialSums (numProcessors);

    for (size_t i = 0; i < numProcessors; ++i)
    {
        int* myPartialSum = &partialSums[i];
        thread t (computeMySum, std::cref (data), i, numProcessors, myPartialSum);
        myThreads[i] = std::move (t);
    }

	for (thread& t : myThreads)
    {
        t.join ();
    }

    return std::accumulate (partialSums.begin (), partialSums.end (), 0);
}

void
computeMySum (const vector<int>& data, size_t myId, unsigned numProcessors, int* myPartialSum)
{
	
    size_t start = (myId * data.size ()) / numProcessors;
    size_t end = (( myId + 1 ) * data.size ()) / numProcessors;
    *myPartialSum = std::accumulate (data.begin () + start, data.begin () + end, 0);

}
