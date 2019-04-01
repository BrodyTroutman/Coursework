/*
  Filename   : OddEvenSort.cc
  Author     : Tyler Martin & Brody Troutman
  Course     : CSCI 476-01
  Assignment : Final Project
  Description: Explore a parallel programming topic in depth.       
*/

/************************************************************/

// System includes

#include <mpi.h>
#include <vector>
#include <algorithm>
#include <random>
#include <thread>

/************************************************************/

#include "Timer.hpp"

/************************************************************/

// Using declarations

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::swap;
using std::minstd_rand;
using std::uniform_int_distribution;
using std::generate_n;
using std::thread;

/************************************************************/

// Function prototypes/global vars/typedefs

void
fillNVector (vector<int>& A, size_t n);

void
serialOddEvenSort (vector<int>& v);

int
computePartner (int phase, int rank);

void
parallelOddEvenSort (vector<int>& v, int commSize, int rank);

void
testPrint (vector<int> &v);

void
doComparisons (vector<int>& v1, vector<int>& v2, bool lessThanComparison);

template <typename Iter>
void
mergeSortSerial (Iter first, Iter last, unsigned cutoff);

template <typename Iter>
void
insertionSort (Iter first, Iter last);

template <typename Iter>
void
mergeSortParallel (Iter first, Iter last, unsigned cutoff, unsigned depth, unsigned level = 0);

/************************************************************/

int 
main (int argc, char* argv[])
{
    int numProcesses;
    int rank;
    int localVSize;
    const int root = 0;
    MPI_Init (&argc, &argv);
    MPI_Comm_size (MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
     double pAvg = 0;
        double sAvg = 0;
        double stdAvg = 0;
  for (int i = 0; i < 5; ++i)
  {
    if (rank == root)
    {
        // size_t n;
        // cout << endl << "Input the number of elements (n) ==> ";
        // cin >> n;

   
        size_t n = 10'000;

        //Pad if n not divisible by p, as it is requirement for scatter
        localVSize = (n % numProcesses) ? (n / numProcesses + 1) : (n / numProcesses);
        size_t vSize = numProcesses * localVSize;

        vector<int> stdV (vSize, 0);
        fillNVector (stdV, n);
        vector<int> serialV (stdV);
        vector<int> parallelV (stdV);

        vector<int> localV (localVSize);

        Timer<> timer;

        //parallel sort
        timer.start ();
        MPI_Bcast (&localVSize, 1, MPI_INT, root, MPI_COMM_WORLD);

        MPI_Scatter (&parallelV[0], localVSize, MPI_INT,
            &localV[0], localVSize, MPI_INT, root, MPI_COMM_WORLD);

        parallelOddEvenSort (localV, numProcesses, rank);

        MPI_Gather (&localV[0], localVSize, MPI_INT, 
             &parallelV[0], localVSize, MPI_INT, root, MPI_COMM_WORLD);

        timer.stop ();
        //printf ("\nParallel time:  %8.2f ms\n", timer.getElapsedMs ());
        pAvg += timer.getElapsedMs ();


        //serial sort
        timer.start ();
        serialOddEvenSort (serialV);
        timer.stop ();
        //printf ("Serial time:    %8.2f ms\n", timer.getElapsedMs ());
        sAvg += timer.getElapsedMs ();

        //std::sort
        timer.start ();
        std::sort (stdV.begin (), stdV.end ());
        timer.stop ();
        //printf ("std::sort time: %8.2f ms\n\n", timer.getElapsedMs ());
        stdAvg += timer.getElapsedMs ();
 

        //cout << std::boolalpha << "Parallel ok? " << (parallelV == stdV) << endl;
        //cout << std::boolalpha << "Serial ok?   " << (serialV == stdV) << endl << endl;
    }
    else
    {
        // Recieve number of elements for each process' vector
        MPI_Bcast (&localVSize, 1, MPI_INT, root, MPI_COMM_WORLD);

        vector<int> localV (localVSize);

        MPI_Scatter (nullptr, localVSize, MPI_INT,
             &localV[0], localVSize, MPI_INT, root, MPI_COMM_WORLD);

        //std::sort (localV.begin (), localV.end ());
        parallelOddEvenSort (localV, numProcesses, rank);

        MPI_Gather (&localV[0], localVSize, MPI_INT, 
             nullptr, localVSize, MPI_INT, root, MPI_COMM_WORLD);

    }

 }
  cout << endl << "pavg " << pAvg / 5 << endl;
  cout << "savg " << sAvg / 5 << endl;
  cout << "stdAvg " << stdAvg / 5 << endl;

    MPI_Finalize ();

    return EXIT_SUCCESS;
}


/************************************************************/

// Fills initial vector to be operated on

void
fillNVector (vector<int>& A, size_t n)
{
    size_t padding = A.size () - n;
    std::random_device rd;
    std::mt19937 mr (rd ());
    generate_n (A.begin () + padding, n, [&](){return (mr () % 20) + 1;});
}

/************************************************************/

void
serialOddEvenSort (vector<int>& v)
{
    size_t N = v.size ();

    for (size_t phase = 0; phase < N; ++phase) 
    {
        if (phase % 2 == 0) 
        {
            for (size_t i = 0; i < N - 1; i += 2)
            {
                if (v[i] > v[i + 1])
                {
                    swap (v[i], v[i + 1]);
                }
            }
        }
        else 
        { // Odd
            for (size_t i = 1; i < N - 1; i += 2)
            {
                if (v[i] > v[i + 1])
                {
                    swap (v[i], v[i + 1]);
                }
            }
        }
    }
}

/************************************************************/

int
computePartner (int phase, int rank, int commSize)
{
    int partner;

    if (phase % 2 == 0)
    {
        if (rank % 2 != 0)
        {
            partner = rank - 1;
        }
        else
        {
            partner = rank + 1;
        }
    }
    else
    {
        if (rank % 2 != 0)
        {
            partner = rank + 1;
        }
        else
        {
            partner = rank - 1;
        }
    }
    if (partner == -1 || partner == commSize)
    {
        partner = MPI_PROC_NULL;
    }

    return partner;
}

/************************************************************/

void
parallelOddEvenSort (vector<int>& localV, int commSize, int rank)
{

    //Test with std sort????
    std::sort (localV.begin (), localV.end ());
    //Local sort
    //Cutoff 10, Depth 3
    //mergeSortParallel (localV.begin (), localV.end (), 10, 3, 0);

    for (int phase = 0; phase < commSize; ++phase)
    {
        int partner = computePartner (phase, rank, commSize);
        vector<int> recvKeys (localV.size ());

        if (partner != MPI_PROC_NULL)
        {
            MPI_Status status;

            //Trade keys
            MPI_Sendrecv (&localV[0], localV.size (), MPI_INT, partner, 0, 
            &recvKeys[0], recvKeys.size (), MPI_INT, partner, 0, MPI_COMM_WORLD, &status);

            //(rank < partner) if true, take lesser keys, otherwise take higher keys
            doComparisons (localV, recvKeys, (rank < partner));
        }
    }
}

/************************************************************/

// Testing purposes only

void
testPrint (vector<int> &v)
{
    cout << endl << "{ ";
    for (int x : v)
    {
        cout << x << " ";
    }
    cout << "}" << endl;
}

/************************************************************/

void
doComparisons (vector<int>& v1, vector<int>& v2, bool takeLesserKeys)
{
   //This works ONLY if presorted
    vector<int> myKeys (v1.size ());
    if (takeLesserKeys)
    {
        
        size_t v1Sub = 0;
        size_t v2Sub = 0;
        size_t myKeySub = 0;

        //Until determining all keys to take
        while (myKeySub != myKeys.size ())
        {
            //Take lesser
            if (v1[v1Sub] < v2[v2Sub])
            {
                myKeys[myKeySub] = v1[v1Sub];
                ++v1Sub;
            }
            else 
            {
                myKeys[myKeySub] = v2[v2Sub];
                ++v2Sub;
            }
            ++myKeySub;
        }
    }

    else
    {
        size_t v1Sub = v1.size () - 1;
        size_t v2Sub = v2.size () - 1 ;
        long int myKeySub = myKeys.size () - 1;

        //Until determining all keys to take
        while (myKeySub != -1)
        {
            //Take greater keys
            if (v1[v1Sub] > v2[v2Sub])
            {
                myKeys[myKeySub] = v1[v1Sub];
                --v1Sub;
            }
            else 
            {
                myKeys[myKeySub] = v2[v2Sub];
                --v2Sub;
            }
            --myKeySub;
        }
    }
    v1 = myKeys;
}

/************************************************************/

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
    Iter mid = std::next (first, std::distance (first, last) / 2);
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

/************************************************************/

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

/************************************************************/

template <typename Iter>
void
mergeSortSerial (Iter first, Iter last, unsigned cutoff)
{
	if (std::distance (first, last) > cutoff)
	{
    Iter mid = std::next (first, std::distance (first, last) / 2);
		mergeSortSerial (first, mid, cutoff);
		mergeSortSerial (mid, last, cutoff);
		std::inplace_merge (first, mid, last);
	}
	else 
	{
		insertionSort (first, last);
	}
}