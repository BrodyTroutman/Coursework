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
  int lN = 5;
  vector<float> lA (lN);

  //Could factor out code and make a lot cleaner..
  if (rank == 0) 
  {
    //Make vector to distribute
    vector<float> A (numProcesses * lN);
    std::random_device rd;
    std::mt19937 mr (rd ());
    std::generate_n (A.begin (), A.size (), [&](){return (mr () % 100);});

    MPI_Scatter (&A[0], lN, MPI_FLOAT, &lA[0], lN, MPI_FLOAT, 0, comm);

    //Do Own Part
    float lS = std::accumulate (lA.begin (), lA.end (), 0);

    //Gather and sum
    vector<float> subResults (numProcesses);
    MPI_Gather (&lS, 1, MPI_FLOAT, &subResults[0], 1, MPI_FLOAT, 0, comm);
    float result = std::accumulate (subResults.begin (), subResults.end (), 0);
    
    cout << endl << "MY RESULT IS:        " << result << endl;
    cout << "MY RESULT SHOULD BE: " << std::accumulate(A.begin (), A.end (), 0) << endl << endl;
  }
  else
  { 
    MPI_Scatter (nullptr, lN, MPI_FLOAT, &lA[0], lN, MPI_FLOAT, 0, comm);
    float lS = std::accumulate (lA.begin (), lA.end (), 0);
    MPI_Gather (&lS, 1, MPI_FLOAT, nullptr, 1, MPI_FLOAT, 0, comm); 
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

string
convertVectorToString (const vector<float>& data) {
  string value = "{ ";
  for (size_t i = 0; i < data.size () - 1; ++i) 
  {
      value += std::to_string (data[i]) + ", ";
  }
  value += std::to_string (data[data.size () - 1]) + " }";
  return value;
}