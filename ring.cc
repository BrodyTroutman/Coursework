/*
  Filename   : ring.cc
  Author     : Brody Troutman
  Course     : CSCI 476-01
  Assignment : Lab8 - Ring-a-ring o' processes
  Description: Learn MPI
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

  int rNeighbor = (rank + 1) % numProcesses;
  int lNeighbor = (rank + numProcesses - 1) % numProcesses;
  //printf ("\nI am rank %d I send to %d and recieve from %d\n",rank, rNeighbor, lNeighbor);
  if (rank == 0)
  {
    string input;
    printf("\nMessage ==> ");
    getline (cin,input);

    MPI_Send (input.c_str (), input.length (), MPI_CHAR, rNeighbor, 0, comm);
    printf("\n%d sent to %d!\n",rank,rNeighbor);
    MPI_Probe (lNeighbor, MPI_ANY_TAG, comm, &status);
    int count;
    MPI_Get_count (&status, MPI_CHAR, &count);
    string msg (count, 0);
    MPI_Recv (&msg[0], msg.size (), MPI_CHAR, lNeighbor, MPI_ANY_TAG, comm, &status);
    printf ("\nMessage = %s\n", msg.data ());
  }
  else 
  {
    MPI_Probe (lNeighbor, MPI_ANY_TAG, comm, &status);
    int count;
    MPI_Get_count (&status, MPI_CHAR, &count);
    string msg(count, 0);
    MPI_Recv (&msg[0], msg.size (), MPI_CHAR, lNeighbor, MPI_ANY_TAG, comm, &status);
    //printf("\ndid i make it here %d\n", rank);
    MPI_Send (msg.c_str(), msg.size () + 1, MPI_CHAR, rNeighbor, 0, comm);
    printf("\n%d sent to %d!\n",rank,rNeighbor);
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