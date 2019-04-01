/*
  Filename   : Matrix.hpp
  Author     : Brody
  Course     : CSCI 476-01
  Assignment : Assignment 4
  Description: Matrix Class for ez nice code. 
*/

/************************************************************/
// Macro guard to prevent multiple inclusions

#ifndef MATRIX_H
#define MATRIX_H

/************************************************************/
// System includes

#include <iostream>

/************************************************************/
// Local includes

/************************************************************/
// Using declarations

using std::ostream;
using std::copy;
using std::endl;

/************************************************************/
// Function prototypes/global vars/typedefs

/************************************************************/

template <typename T>
class Matrix
{
public:

	//Default Constructor
	Matrix ()
        : m_rows (0), m_cols (0), m_data (nullptr)
    {
    }

    // Initialize a matrix of size 'numRows' x 'numCols'
    Matrix (unsigned numRows, unsigned numCols)
        : m_rows (numRows), m_cols (numCols), m_data (new T[numRows * numCols])
    {
    }

    
    // Copy constructor
    Matrix (const Matrix& m)
        : m_rows (m.getNumRows ()), m_cols (m.getNumCols ()), m_data (new T[m_rows * m_cols])
    {
    }


    // Copy assignment
    // Test for self-assignment and return *this
    Matrix&
    operator= (const Matrix& m)
    {
        if (this != &m)
        {

            if (getNumElements () != m.getNumElements ())
            {
            	m_cols = m.getNumCols ();
            	m_rows = m.getNumRows ();
                delete[] m_data;
                m_data = nullptr;
                m_data = new T[m.size ()];
                copy (m.begin (), m.end (), begin ());
            }
        }
        return *this;
    }

    // Destructor
    ~Matrix ()
    {
        delete[] m_data;
    }

    // Subscript operator (non-const)
    // Do NOT do bounds checking
    T&
    operator () (unsigned row, unsigned col)
    {
        return m_data[getNumCols () * row + col];
    }

    // Subscript operator (const)
    // Do NOT do bounds checking
    const T&
    operator () (unsigned row, unsigned col) const
    {
       return m_data[getNumCols () * row + col];
    }

    T*
    begin ()
    {
        return m_data;
    }
    
    const T* 
    begin () const
    {
        return m_data;
    }

    T* 
    end ()
    {
        return m_data + (m_rows * m_cols);
    }

    const T* 
    end () const
    {
        return m_data + (m_rows * m_cols);
    }

    unsigned
    getNumRows () const
    {
        return m_rows;
    }

    unsigned
    getNumCols () const
    {
        return m_cols;
    }

    unsigned
    getNumElements () const
    {
        return m_rows * m_cols;
    }

private:
    unsigned m_rows;
    unsigned m_cols;
    T*       m_data;
};

/************************************************************/

// Free functions associated with the class

// Output operator
// Allows us to do "cout << M;", where "M" is a Matrix
// Testing purposes only.
template <typename T>
ostream&
operator<< (ostream& output, const Matrix<T>& m)
{
  
  for (unsigned i = 0; i < m.getNumRows (); ++i)                        
  {
  	output << "[ ";
    for (unsigned j = 0; j < m.getNumCols (); ++j)
    {
    	output << m(i,j) << " ";
    }

  	output << "]" << endl;
  }

  return output;
}

#endif

/************************************************************************/