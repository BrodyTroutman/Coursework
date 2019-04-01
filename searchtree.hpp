/*
  Filename   : SearchTree.hpp
  Author     : Gary M. Zoppetti
  Course     : CSCI 362-01
  Description: Binary search tree class, 
  a basis for implementing associative containers. 
*/

/************************************************************/
// Macro guard

#ifndef SEARCHTREE_HPP
#define SEARCHTREE_HPP

/************************************************************/
// System includes

#include <iostream>
#include <algorithm>
#include <iterator>
#include <queue>

/************************************************************/
// Local includes

/************************************************************/
// Using declarations

using std::ostream;
using std::ostream_iterator;
using std::queue;

/************************************************************/

template<typename T>
struct Node
{
  typedef Node* NodePtr;

  Node (const T& d = T ())
  : data (d),left (nullptr), right (nullptr), parent (nullptr)
  {
    // Initialize data, left, right, and parent in
    //   the member initialization list
    // left, right, and parent should be set to nullptr
    // The body of this constructor should be empty
  }

  Node (const T& d, NodePtr l, NodePtr r, NodePtr p)
  : data (d),left (l), right (r), parent (p)
  {
    // Initialize data, left, right, and parent in
    //   the member initialization list
    // The body of this constructor should be empty
  }


  T        data;
  NodePtr  left;
  NodePtr  right;
  NodePtr  parent;
};

/************************************************************/

// Forward declaration
template<typename T>
class SearchTree;

/************************************************************/

template<typename T>
struct TreeIterator
{
  friend class SearchTree<T>;

  typedef TreeIterator<T>  Self;
  typedef Node<T>*         NodePtr;
  typedef const Node<T>*   ConstNodePtr;

  typedef T                value_type;
  typedef const T*         pointer;
  typedef const T&         reference;

  TreeIterator ()
    : m_nodePtr ()
  { }

  explicit
  TreeIterator (ConstNodePtr n)
    : m_nodePtr (n)
  { }

  reference
  operator* () const
  {
    // Return the data associated with the node
    // COMPLETE
	return m_nodePtr->data;
  }

  // Return address of node's data member
  pointer
  operator-> () const
  {
    return &*this;
  }

  // Pre
  Self&
  operator++ ()
  {
    m_nodePtr = increment (m_nodePtr);

    return *this;
  }

  // Post
  Self
  operator++ (int)
  {
    // Increment iterator so it points to in-order successor
    // COMPLETE
	Self temp (*this);
	m_nodePtr = increment (m_nodePtr);
	return temp;
  }

  // Pre
  Self&
  operator-- ()
  {
    m_nodePtr = decrement (m_nodePtr);

    return *this;
  }

  // Post
  Self
  operator-- (int)
  {
    // Decrement iterator so it points to in-order predecessor
    // COMPLETE
	Self temp (*this);
	m_nodePtr = decrement (m_nodePtr);
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

private:

  static ConstNodePtr
  increment (ConstNodePtr n)
  {
    // Find in-order successor of "n"
    // COMPLETE
	if (n->right != nullptr)
	{
		while (n -> left != nullptr)
		{
			n = n -> left;
		}
		return n;
	}
	while (n->parent->data < n->data )
	{
		n = n->parent;
	}
	return n->parent;
  }

  static ConstNodePtr
  decrement (ConstNodePtr n)
  {
    // Find in-order predecessor of "n"
    // COMPLETE
	if (n->left != nullptr)
	{
		while (n -> right != nullptr)
		{
			n = n -> right;
		}
		return n;
	}
	while (n->parent->data < n->data)
	{
		n = n->parent;
	}
	return n->parent;
  }

private:

  ConstNodePtr m_nodePtr;
};

/************************************************************/

template<typename T>
class SearchTree
{
  friend class TreeIterator<T>;

public:

  // DO NOT MODIFY typedef's
  typedef T             value_type;
  typedef T*            pointer;
  typedef const T*      const_pointer;
  typedef T&            reference;
  typedef const T&      const_reference;

  typedef TreeIterator<T> iterator;
  typedef TreeIterator<T> const_iterator;

  // Header parent points to root of tree or is nullptr
  //   if the tree is empty
  // Header left points to LARGEST node or is nullptr
  //   if the tree is empty  
  // Header right points to SMALLEST node or is nullptr
  //   if the tree is empty
  // size represents the number of elements in the tree
  SearchTree ()
    : m_header (), m_size (0)
  {
  }

  // Copy constructor
  //Another thing that doesn't work... 
  SearchTree (const SearchTree& t) // COMPLETE
  : m_header (), m_size (0)
  {
    // COMPLETE
	for (iterator i = t.begin(); i != t.end (); ++i)
	{
		insert (*i);
	}
		
  }

  ~SearchTree ()
  {
	//clear isn't working, and is a mangled mess.  
    //clear ();
  }

  bool
  empty () const
  {
    // Return whether the tree is empty 
	return m_size == 0;
  }

  size_t
  size () const
  {
    // Return the size
	return m_size;
  }

  iterator
  begin ()
  {
    // Build an iterator that points to the smallest element
	return iterator (m_header.right);
  }

  const_iterator
  begin () const
  {
    // Build an iterator that points to the smallest element
	return const_iterator (m_header.right);
  }

  iterator
  end ()
  {
    // Build an iterator that points to one beyond the end
    // (i.e., the header node)
	return iterator (&m_header);
  }

  const_iterator
  end () const
  {
    // Build an iterator that points to one beyond the end
    // (i.e., the header node)
	return iterator (&m_header);
  }

  iterator
  find (const T& v) const
  {
    // Call private helper method
    ConstNodePtr n = findHelper (v);
    if (n == nullptr)
    {
      // Wasn't found so return end ()
      n = &m_header;
    }
    return iterator (n);
  }

  std::pair<iterator, bool>
  insert (const T& v)
  {
    NodePtr insertedNode = insert (v, m_header.parent, &m_header);
    bool inserted = insertedNode != nullptr;
    if (inserted)
    {
      // Update header right to point to smallest node
      if (m_header.right == nullptr || v < m_header.right->data)
        m_header.right = insertedNode;
      // Update header left to point to largest node
      if (m_header.left == nullptr || v > m_header.left->data)
        m_header.left = insertedNode;
    }

    return std::make_pair (iterator (insertedNode), inserted);
  }

  size_t
  erase (const T& v)
  {
    // Update header right to point to smallest node
    if (m_header.right != nullptr && v == m_header.right->data)
      m_header.right =
        const_cast<NodePtr> (iterator::increment (m_header.right));
    // Update header left to point to largest node
    if (m_header.left != nullptr && v == m_header.left->data)
      m_header.left =
        const_cast<NodePtr> (iterator::decrement (m_header.left));

    bool erased = erase (v, m_header.parent, &m_header);

    return erased ? 1 : 0;
  }

  void
  clear ()
  {
    // Delete all nodes and set size to 0
	clear ( m_header.right);
  }

  void
  printInOrder (ostream& out) const
  {
    printInOrder (out, m_header.parent);
  }

  void
  printLevelOrder (ostream& out) const
  {
    printLevelOrder (out, m_header.parent);
  }

private:

  typedef struct Node<T> Node;
  typedef Node*          NodePtr;
  typedef const Node*    ConstNodePtr;

  NodePtr
  minimum (NodePtr r) const
  {
    // Find minimum node in r's subtree
	if (r == nullptr)
	{
		return nullptr;
	}
	while (r -> left != nullptr)
	{
		r = r -> left;
	}
	return r;
  }

  NodePtr
  maximum (NodePtr r) const
  {
    // Find maximum node in r's subtree
	if (r == nullptr)
	{
		return nullptr;
	}
	while (r -> right != nullptr)
	{
		r = r -> right;
	}
	return r;
  }

  //Pretty sure this doesn't work right. 
  ConstNodePtr
  findHelper (const T& v) const
  {
    // Return a pointer to the node that contains "v"
	ConstNodePtr nodeFind = m_header.right;
	while (nodeFind != nullptr)
	{
		if (v == nodeFind->data)
		{
			return nodeFind;
		}
		else if (v > nodeFind->data)
		{
			nodeFind = nodeFind->right;
		}
		else if (v < nodeFind->data)
		{
			nodeFind = nodeFind->left;
		}
	}
	return &m_header;
			
  }

  NodePtr
  insert (const T& v, NodePtr& r, NodePtr parent)
  {
    // Insert "v" into the tree rooted at "r"
    // Use "parent" for recursion and setting the parent of the
    //   node containing "v"
	if (r == nullptr)
	{
		r = new Node (v, nullptr, nullptr, parent);
		//r = addNode;
		if (parent == &m_header)
		{
			m_header.parent = r;
		}
		++m_size;
		return r;
	}
	if ( v < r->data )
	{
		return insert (v, r->left, r);
	}
	if (v > r->data )
	{
		return insert (v, r->right, r);
	}
	else
	{
		return nullptr;
	}
  }

  //erase is broken. I don't even know what happened, it was working. 
  // too frustrated to continue. 
  bool
  erase (const T& v, NodePtr& r, NodePtr parent)
  {
    // Erase "v" from the tree rooted at "r"
    // Use "parent" for recursion and updating the parent of the
    //   node containing "v"
    // Return whether the erase succeeded or not
	
	if ( r == nullptr )
	{
		return false;
	}
	bool succErase = true;
	if (v < r->data)
	{
		succErase = erase(v, r->left, r);
	}
	else if (v > r->data)
	{
		succErase = erase(v, r->right, r);
	}
	//2 Children
	else if ( r->left != nullptr && r->right != nullptr)
	{
		NodePtr ios = r;
		++ios;
		r->data = ios->data;
		succErase = erase(r->data, r->right, r);
	}
	//0 or 1 Children
	else
	{
		NodePtr replaceNode = (r->left != nullptr) ? r->left : r->right;
		NodePtr temp = r;
		r = replaceNode;
		delete temp;
		--m_size;
	}
	return succErase;
	
	/* I know all the code below is wrong, but I couldn't get the code from the slides
	to fit into this project at all.
	
	
	iterator eraseNode = find (v);
	if (eraseNode == end())
	{
		return false;
	}
	if ( eraseNode->left != nullptr && eraseNode->right != nullptr)
	{
		iterator ios = eraseNode;
		++ios;
		eraseNode->data = ios->data;
	}
	else
	{
		iterator replaceNode = (eraseNode->left != nullptr) ? eraseNode->left : eraseNode->right;
		if (r == m_header.parent)
		{
			m_header.parent = replaceNode;
		}
		if (r->data < parent->data)
		{
			parent->left = replaceNode;
		}
		if (r->data > parent->data)
		{
			parent->right = replaceNode;
		}
		iterator temp = eraseNode;
		eraseNode = replaceNode;
		delete temp;
		--m_size;
	}
	return true;
	
  }
  //Cannot get to stop infinite looping. 
  void
  clear (NodePtr r)
  {
    // Delete all nodes in the tree
	//while ( r != nullptr )
	//{
	//	erase (r->data);
	//	r = m_header.right;
	//	std::cout << m_size;
	//}
	*/
  }
  
  void
  printInOrder (ostream& out, NodePtr r) const
  {
    if (r != nullptr)
    {
      printInOrder (out, r->left);
      out << r->data << " ";
      printInOrder (out, r->right);
    }
  }

  // FIXME: This routine is INCORRECT and is only meant to
  //   show you some techniques you MAY want to employ. 
  // Rewrite this method to output elements in the form required
  //   by the operator<< below. 
  
  void
  printLevelOrder (ostream& out, NodePtr r) const
  {
	/* 
    queue<NodePtr> q;
    q.push (r);
    while (!q.empty ())
    {
      r = q.front ();
      q.pop ();
      out << r->data << " ";
      if (r->left != nullptr)
        q.push (r->left);
      if (r->right != nullptr)
        q.push (r->right);
    }
	*/
	queue<NodePtr> q;
	
	
	
  }

private:

  Node   m_header;
  size_t m_size;
};

/************************************************************/

// Output tree as [ e1 e2 e3 ... en ]
//   with '|' to indicate the end of a level, and '-' to indicate
//   a missing node.
// E.g., the tree
//       4
//    2     7
//         6
// should be printed thus: [ 4 | 2 7 | - - 6 - ]
template<typename T>
ostream&
operator<< (ostream& out, const SearchTree<T>& tree)
{
  out << "[ ";
  tree.printInOrder (out);
  //tree.printLevelOrder (out);
  out << "]";

  return out;
}

/************************************************************/

#endif

/************************************************************/