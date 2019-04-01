/*
  Filename   : List.hpp
  Author     : Gary Zoppetti / Brody Troutman
  Course     : CSCI 362-01
  Assignment : Program 3
  Description: List class, our implementation of a list ADT 
                 using a doubly-linked list data structure.
*/   
/************************************************************/
// Macro guard

#ifndef LIST_HPP
#define LIST_HPP

/************************************************************/
// System includes

#include <iostream>
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <initializer_list>

/************************************************************/
// Local includes

/************************************************************/
// Using declarations

using std::ostream;
using std::ostream_iterator;
using std::initializer_list;

/************************************************************/

template <typename T>
struct ListNode 
{
  ListNode ()
    : data ()
  {
    next = this;
    prev = this;
  }
  
  ListNode (const T& d, ListNode* n = nullptr,
	    ListNode* p = nullptr)
		
    : data (d), next (n), prev (p)
  {
  }

  T          data;
  ListNode*  next;
  ListNode*  prev;
};

/************************************************************/

template<typename T>
struct ListIterator
{
  typedef ListIterator<T>                  Self;
  typedef ListNode<T>                      Node;

  typedef ptrdiff_t                        difference_type;
  typedef std::bidirectional_iterator_tag  iterator_category;

  typedef T                                value_type;
  typedef T*                               pointer;
  typedef T&                               reference;

  ListIterator ()
    : m_nodePtr ()
  { 
  }

  explicit
  ListIterator (Node* n)
    : m_nodePtr (n) 
  { 
  }
  
  reference
  operator* () const
  { 
    return m_nodePtr->data;
  }
  
  pointer
  operator-> () const
  { 
    return &m_nodePtr->data;
  }
  
  Self&
  operator++ ()
  {
	m_nodePtr = m_nodePtr->next;
	return *this;
  }
  
  // Post-increment
  Self
  operator++ (int)
  {
    Self temp (*this);
    m_nodePtr = m_nodePtr->next;
    return temp;
  }

  Self&
  operator-- ()
  {
	m_nodePtr = m_nodePtr->prev;
    return *this;
  }
  
  // Post-decrement
  Self
  operator-- (int)
  {
    Self temp (*this);
    m_nodePtr = m_nodePtr->prev;
    return temp;
  }
  
  bool
  operator== (const Self& i) const
  {
    return m_nodePtr == i.m_nodePtr;
  }
  
  bool
  operator!= (const Self& i) const
  {
    return m_nodePtr != i.m_nodePtr;
  }
  
  /************************************************************/

  Node* m_nodePtr;

};

/************************************************************/

template<typename T>
struct ListConstIterator
{
  typedef ListConstIterator<T>     Self;
  typedef const ListNode<T>        Node;
  typedef ListIterator<T>          iterator;
  
  typedef ptrdiff_t                        difference_type;
  typedef std::bidirectional_iterator_tag  iterator_category;

  typedef       T                   value_type;
  typedef const T*                  pointer;
  typedef const T&                  reference;
  
  ListConstIterator ()
    : m_nodePtr ()
  {
  }
  
  explicit
  ListConstIterator (Node* n)
    : m_nodePtr (n)
  { 
  }
  
  ListConstIterator (const iterator& i)
    : m_nodePtr (i.m_nodePtr)
  { 
  }
  
  reference
  operator* () const
  { 
    return m_nodePtr->data;
  }
  
  pointer
  operator-> () const
  { 
    return m_nodePtr->data;
  }

  Self&
  operator++ ()
  {
    m_nodePtr = m_nodePtr->next;
    return *this;
  }
  
  //post-increment
  Self
  operator++ (int)
  { 
    Self temp (*this);
	m_nodePtr = m_nodePtr->next;
	return temp;
  }
  
  Self&
  operator-- ()
  {
    m_nodePtr = m_nodePtr->prev;
    return *this;
  }
  
  //post-decrement
  Self
  operator-- (int)
  {
    Self temp (*this);
	m_nodePtr = m_nodePtr->prev;
	return temp;
  }
  
  bool
  operator== (const Self& i) const
  { 
    return m_nodePtr == i.m_nodePtr;
  }
  
  bool
  operator!= (const Self& i) const
  { 
    return m_nodePtr != i.m_nodePtr;
  }
  
  /************************************************************/

  Node* m_nodePtr;

};

template<typename T>
inline bool
operator== (const ListIterator<T>& i1,
	    const ListConstIterator<T>& i2)
{ 
  return i1.m_nodePtr == i2.m_nodePtr;
}

template<typename T>
inline bool
operator!= (const ListIterator<T>& i1,
	    const ListConstIterator<T>& i2)
{ 
  return i1.m_nodePtr != i2.m_nodePtr;
}

/************************************************************/

template<typename T>
class List 
{
  typedef ListNode<T>            Node;

public:

  typedef ListIterator<T>        iterator;
  typedef ListConstIterator<T>   const_iterator;
  typedef T                      value_type;
  typedef T*                     pointer;
  typedef T&                     reference;
  typedef const T&               const_reference;

  List ()
  : m_header(), m_size(0)
  {
  }

  List (initializer_list<T> values)
    : m_header (), m_size (0)
  {
    for ( auto& e : values )
	{
		push_back (e);
	}
  }  

  // Initialize a list of size "n", with each value set to "defValue"
  explicit List (size_t n, const T& defValue = T ())
  : m_header(), m_size(0)
  {
	 for (size_t i = 0; i < n; ++i)
	{
		push_back (defValue);
	}
  }

  List (const List& otherList)
  : m_header(), m_size(0)
  {
	  for (const auto& elem : otherList)
	  {
		  push_back (elem);
	  }
  }
  
  ~List ()
  {
	while (!empty ())
	{
		pop_front ();
	}
  }

    // Assign "rhs" to this list
	// Watch for self-assignment
  List&
  operator= (const List& rhs)
  {
	  if (this != &rhs)
	  {
		  const_iterator rhsTrace = rhs.begin ();
		  iterator thisTrace = this->begin ();
		  
		  //copy data
		  while (rhsTrace != rhs.end () && thisTrace != this->end ())
	      {
			  thisTrace.m_nodePtr->data = rhsTrace.m_nodePtr->data;
			  ++rhsTrace;
			  ++thisTrace;
		  }
		  
		  //add if necessary
		  while (rhsTrace != rhs.end ())
		  {
			  this->push_back (rhsTrace.m_nodePtr->data);
			  ++rhsTrace;
		  }
		  
		  //delete if necessary
		  while (thisTrace != this->end ())
		  {
			 thisTrace = this->erase (thisTrace);
		  }  
	  }
    return *this;
  }
  
  bool    
  empty () const
  {
	  return m_size == 0;
  }
  
  size_t
  size ()  const
  {
	  return m_size;
  }

  reference
  front ()
  {
    return m_header.next->data;
  }

  const_reference
  front () const
  {
    return m_header.next->data;
  }

  reference
  back ()
  {
    return m_header.prev->data;
  }

  const_reference
  back () const
  {
    return m_header.prev->data;
  }

  void 
  push_front (const_reference item)
  {
    insert (begin (), item);
  }

  void
  pop_front ()
  {
    erase (begin ());
  }

  void 
  push_back  (const_reference item)
  {
    insert (end (), item);
  }

  void 
  pop_back ()
  {
    erase (--end ());
  }

  iterator       
  begin ()
  {
    return iterator (m_header.next);
  }

  const_iterator 
  begin () const
  {
    return const_iterator (m_header.next);
  }

  iterator       
  end   ()
  {
    return iterator (&m_header);
  }

  const_iterator 
  end   () const
  {
    return const_iterator (&m_header);
  }  

  // Insert "item" at position "i"
  // Return an iterator referencing the inserted element
  iterator 
  insert (iterator i, const T& item)
  {
    Node* curr = i.m_nodePtr;
    Node* n = new Node (item, curr, curr->prev);
    n->next->prev = n;
    n->prev->next = n;
    ++m_size;
    return iterator (n);
  }

  // Erase element at position "i"
  // Return an iterator referencing the element beyond the one erased
  iterator 
  erase  (iterator i)
  {
    Node* curr = i.m_nodePtr;
    Node* succNode = curr->next;
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    delete curr;
    --m_size;
    return iterator(succNode);
  } 

  // Erase elements in the range [i1, i2)
  // Return iterator "i2"
  iterator 
  erase  (iterator i1, iterator i2)
  {
    size_t numToDelete = std::distance (i1, i2);
    for (size_t i = 0; i < numToDelete; ++i)
    {
      i1 = erase (i1);
    }
    return i2;
  }
  
private:
  // Dummy header
  Node   m_header;
  size_t m_size;

};

/************************************************************/

// output "list" as [ e1 e2 e3 ... en ]
template<typename T>
ostream&
operator<< (ostream& out, const List<T>& list)
{
  out << "[ ";
  // Print the elements ...
  for (const auto& elem : list)
  {
    out << elem << " ";
  }  
  out << ']';
  return out;
}

/************************************************************/

#endif

/************************************************************/