/*
  Filename   : scattergather.cc
  Author     : Brody Troutman
  Course     : CSCI 476-01
  Assignment : Scatter & Gather
  Description: Intro to MPI
*/   

/************************************************************/
// System includes

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <mpi.h>

/************************************************************/
// Local includes


/************************************************************/
// Using declarations

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

/************************************************************/
// Function prototypes/global vars/typedefs

void
checkMpiResult (int result, const string& message);

string
convertVectorToString (const vector<float>& data);

/************************************************************/


int
main (int argc, char* argv[])
{
  int result = MPI_Init (&argc, &argv);
  checkMpiResult (result, "MPI_Init");

  int numProcesses;
  MPI_Comm_size (MPI_COMM_WORLD, &numProcesses);
  int rank;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Request request;
  MPI_Status status;

  int arbitraryLocal = 2;

  // int lNeighbor = (rank + 1) % numProcesses;
  // int rNeighbor = (rank + numProcesses + 1) % numProcesses;
  int m;
  if (rank == 0) 
  { 
    cout << endl << "m ==> ";
    cin >> m;

    MPI_Isend(&m, 1, MPI_INT, 1, 1, comm, &request);
    MPI_Wait(&request, &status);

    int oneLocal = 0;
    MPI_Irecv(&oneLocal, 1, MPI_INT, 1, 0, comm, &request);
    int hasRecieved = 0;
    while (!hasRecieved)
    {
      arbitraryLocal += 2;
      arbitraryLocal %= 1000;
      MPI_Test (&request, &hasRecieved, &status);
    }
    
    cout << endl << "my local:    " << arbitraryLocal;
    cout << endl << "one's local: " << oneLocal;
    cout << endl << "Total:       " << arbitraryLocal + oneLocal << endl << endl;
  }

  else
  {
    MPI_Irecv(&m, 1, MPI_INT, 0, 1, comm, &request);
    int hasRecieved = 0;
    while (!hasRecieved)
    {
      arbitraryLocal += 5;
      arbitraryLocal %= 1000;
      MPI_Test (&request, &hasRecieved, &status);
    }

    int sendToZero = m + arbitraryLocal;
    MPI_Send(&sendToZero, 1, MPI_INT, 0, 0, comm);
  }

  MPI_Finalize ();

  return EXIT_SUCCESS;
}

/************************************************************/
//Function Implementation

void
checkMpiResult (int result, const string& message)
{
  if (result != MPI_SUCCESS)
  {
    printf ("Error %d: %s\n", result, message.c_str ());
    printf ("Aborting...\n");
    
    MPI_Abort (MPI_COMM_WORLD, result);
  }
}

/************************************************************/