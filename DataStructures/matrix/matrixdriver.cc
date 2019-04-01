/*
  Filename   : matrixdriver.cc
  Author     : Brody Troutman
  Course     : CSCI 476-01
  Assignment : Assignment 4: Cache Me Inside, Howbowdah
  Description: Multiply two matricies using different 
  	algorithms to determine how optimal code optimizes cache.
  	Row vs Column vs Block traversing

*/   

/************************************************************/
// System includes

#include <iostream>
#include <algorithm>
#include <random>
#include <string>

/************************************************************/
// Local includes

#include "Matrix.hpp"
#include "Timer.hpp"

/************************************************************/
// Using declarations

using std::cout;
using std::cin;
using std::string;

/************************************************************/
// Function prototypes/global vars/typedefs

void
multiplyIjk (Matrix<int>& C, const Matrix<int>& A, const Matrix<int>& B);

void
multiplyJki (Matrix<int>& C, const Matrix<int>& A, const Matrix<int>& B);

void
multiplyKij (Matrix<int>& C, const Matrix<int>& A, const Matrix<int>& B);

void
multiplyBlock (Matrix<int>& C, const Matrix<int>& A, const Matrix<int>& B, unsigned blockSize);

unsigned
checkSum (const Matrix<int>& A);

/************************************************************/

int      
main (int argc, char* argv[]) 
{        
  cout << endl;

  unsigned n;
  string version;
  cout << "N       ==> ";
  cin >> n;

  cout << "Version ==> ";
  cin >> version;
  
  std::uniform_int_distribution<> dis (0,4);
  std::mt19937 mr (0);

  Matrix<int> A (n,n);
  std::generate_n (A.begin (), A.getNumElements (), [&]{return dis (mr);});

  Matrix<int> B (n,n);
  std::generate_n (B.begin (), B.getNumElements (), [&]{return dis (mr);});

  Matrix<int> C (n,n);

  Timer<> timer;

  if (version == "ijk")
  {
  		cout << endl;
  		timer.start ();
  		multiplyIjk (C, A, B);
  		timer.stop ();
  		cout << "Sum:   " << checkSum (C) << endl;
  		printf ("Time:  %.2f ms \n\n", timer.getElapsedMs ());
  }
  else if (version == "jki")
  {
  		cout << endl;
  		timer.start ();
  		multiplyJki (C, A, B);
  		timer.stop ();
  		cout << "Sum:   " << checkSum (C) << endl;
  		printf ("Time:  %.2f ms \n\n", timer.getElapsedMs ());
  }
  else if (version == "kij")
  {
  		cout << endl;
  		timer.start ();
  		multiplyKij (C, A, B);
  		timer.stop ();
  		cout << "Sum:   " << checkSum (C) << endl;
  		printf ("Time:  %.2f ms \n\n", timer.getElapsedMs ());
  }
  else if (version == "block")
  {
  		unsigned blockSize;
  		cout << "B       ==> ";
  		cin >> blockSize;
  		cout << endl;

  		timer.start ();
  		multiplyIjk (C, A, B);
  		timer.stop ();
  		cout << "Sum:   " << checkSum (C) << endl;
  		printf ("Time:  %.2f ms \n\n", timer.getElapsedMs ());
  }

  return EXIT_SUCCESS; 
}

/************************************************************/
//Function Implementation

//A Row-Wise, B Col-Wise, C Fixed
void
multiplyIjk (Matrix<int>& C, const Matrix<int>& A, const Matrix<int>& B)
{
	for (unsigned i = 0; i < A.getNumRows (); ++i)
	{
		for (unsigned j = 0; j < B.getNumCols (); ++j)
		{
			unsigned sum = 0;
			for (unsigned k = 0; k < A.getNumCols (); ++k)
			{
				sum += A (i,k) * B (k,j);
			}
			C (i,j) = sum;
		}
	}
}

//A Col Wise, B Fixed, C Col Wise
void
multiplyJki (Matrix<int>& C, const Matrix<int>& A, const Matrix<int>& B)
{
	for (unsigned j = 0; j < B.getNumCols (); ++j)
	{
		for (unsigned k = 0; k < A.getNumCols (); ++k)
		{
			unsigned r = B (k,j);
			for (unsigned i = 0; i < A.getNumRows (); ++i)
			{
				C (i,j) += A (i,k) * r;
			}
		}
	}
}

//A Fixed, B RowWise, C RowWise
void
multiplyKij (Matrix<int>& C, const Matrix<int>& A, const Matrix<int>& B)
{
	for (unsigned k = 0; k < A.getNumCols (); ++k)
	{
		for (unsigned i = 0; i < A.getNumRows (); ++i)
		{
			unsigned r = A (i,k);
			for (unsigned j = 0; j < B.getNumCols (); ++j)
			{
				C (i,j) += r * B (k,j);
			}
		}
	}
}
//ijk in ijk
void
multiplyBlock (Matrix<int>& C, const Matrix<int>& A, const Matrix<int>& B, unsigned blockSize)
{
	unsigned i, j, k;
	unsigned blsz = blockSize;
	//IJK per block
	for (i = 0; i < A.getNumRows (); i += blsz)
	{
		for (j = 0; j < B.getNumCols (); j += blsz)
		{
			for (k = 0; k < A.getNumCols (); k += blsz)
			{
				/* B x B mini matrix multiplcations */
				//IJK inside blocks
				for (unsigned i1 = i; i1 < i + blsz; ++i1)
				{
					for (unsigned j1 = j; j1 < j + blsz; ++j1)
					{
						for (unsigned k1 = k; k1 < k + blsz; ++k1)
						{
							C (i1,j1) += A (i1,k1) * B (k1,j1);
						}
					}
				}
			}
		}
	}
}

unsigned
checkSum (const Matrix<int>& A)
{
	if (A.getNumElements() < 10000)
	{
		return std::accumulate (A.begin (), A.end (), 0);
	}
	else 
	{
		return std::accumulate (A.begin (), A.begin () + 10000, 0);
	}
}

/************************************************************************/
/* RESULTS


  N      ijk        jki        kij      block 
  ====   ======     ======     ======   ======
  1024   1575.72    8342.82    1056.89  1806.77 (B = 08)
  1408   4043.10    23276.56   2702.11  3985.03 (B = 32)
  1792   21276.83   58754.91   5263.70  20961.4 (B = 04)

  Discussion
  ==========
  My results seem fairly consistent with what we discussed in class.
  The times rank in order from fastest to slowest, kij > ijk=block > jki.
  However, block algorithm did not outperform the other algorithms..
  This may be due to a smaller data set and having higher overhead. 

  However, changing the block size did not seem to affect the performance
  of that algorithm significantly. Maybe due to the size of the dataset 
  we are working on.

*/