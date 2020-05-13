/*
 * List.h
 *
 *  Created on: Oct 8, 2019
 *      Authors: Alec Twining & Michael Sisko
 *      Alec did parts operator==, readfrom(istream), writeTo(string),
 *      // prepend(item), insert(Item, index) and wrote for the tests for them.
 *      Michael did operator!=,writeTo(ostream),readFrom(string),getIndexOf(Item), and remove(index
 *      // and wrote the test for them
 *
 */

#ifndef LIST_H_
#define LIST_H_


#include<iostream>
#include <cassert>
#include <fstream>
#include <cstring>
using namespace std;

template <class Item>

class List {
public:
	List();
	friend class ListTester;
	unsigned getSize() const ;
	Item getFirst() const ;
	Item getLast() const ;
	void append(const Item& it) ;
	List(const List& original) ;
	List& operator=(const List& original);
	~List() ;
	bool operator==(const List& original);
	void writeTo(string fileName) const;
	void readFrom(istream& in) ;
	void prepend(const Item& it) ;
	void insert(const Item it, unsigned index);
	//
	void writeTo( ostream& out ) ;
	void readFrom( const string& fileName );
	int getIndexOf( const Item& it );
	Item remove( unsigned index );
	bool operator!=(const List& l2) const ;


private:
	struct Node {
		Item myItem;
		Node* myNext;
		Node();
		Node(Item item, Node* nextNode);
		~Node();
	};
	unsigned mySize;
	Node* myFirst;
	Node* myLast;

	void makeCopyOf(const List &original);
	void makeCopyof(const List &original);
};

template <class Item>
List<Item>::List() {
	myFirst = NULL;
	myLast = NULL;
	mySize = 0;
	// TODO Auto-generated constructor stub

}
//initializes the node
template <class Item>
List<Item>::Node::Node() {
	myItem = Item();
	myNext = NULL;
}
//sets them eqaul to a variable
template <class Item>
List<Item>::Node::Node(Item item, Node* nextNode) {
	myItem = item;
	myNext = nextNode;
}
//rerurns teh size of the list
template <class Item>
unsigned List<Item>::getSize() const {
	return mySize;
}
//returns first value in the list
template <class Item>
Item List<Item>::getFirst() const {
	if(mySize < 0 || myFirst == NULL) {
		throw underflow_error("Warning: The list is empty");
	}else{
		return myFirst->myItem;
	}
}
//returns the last value in the list
template <class Item>
Item List<Item>::getLast() const {
	if(mySize < 0 || myFirst == NULL) {
		throw underflow_error("Warning: The list is empty");
	}else{
		return myLast->myItem;
	}

}
// adds value to list
template <class Item>
void List<Item>::append(const Item& it) {
	Node * nodePtr = new List::Node(it, NULL);
	if (mySize == 0) {
		myFirst = nodePtr;
	} else {
		myLast->myNext = nodePtr;
	}
	myLast = nodePtr;
	mySize ++;
}
//extracted function
template <class Item>
void List<Item>::makeCopyof(const List &original) {
	myFirst = myLast = NULL; //  set pointers
	mySize = 0; //   and size to 'empty' values
	if (original.getSize() > 0) {
		Node *oPtr = original.myFirst; //  start at the first node
		while (oPtr != NULL) { //  while there are nodes to copy:
			append(oPtr->myItem); //   append the item in that node
			oPtr = oPtr->myNext;  //   advance to next node

		}
	}
}
//list destructor
template <class Item>
List<Item>::~List() {
	delete myFirst;          // delete first node, invoking ~Node() (does nothing if myFirst == NULL)
	myFirst = myLast = NULL; // clear myFirst and myLast (optional)
	mySize = 0;              // clear mySize (optional)
}
//node desructor
template <class Item>
List<Item>::Node::~Node() {
	//	cout << "~Node() is deallocating the node containing item "
	//			<< myItem << endl;
	delete myNext;       // delete the next node, invoking ~Node() in it
	// (does nothing if myNext == NULL)
}
//performs makeCopyof constructor
template <class Item>
List<Item>::List(const List& original) {
	makeCopyof(original);
}

//deallocates chain nodes and perfoms makeCopyof
template <class Item>
List<Item>& List<Item>::operator=(const List& original) {
	if (&myFirst != &original.myFirst) {
		delete myFirst;
		makeCopyof(original);
	}
	return *this;

}
//checks equality
template <class Item>
bool List<Item>::operator==(const List& original) {
	if (mySize == original.mySize) {
		if (mySize == 0) {
			return true;
		}
		Node *myPtr = original.myFirst;
		Node *myPtr2 = myFirst;

		while (myPtr != NULL) {
			if (myPtr2 -> myItem != myPtr -> myItem) {
				return false;
			}
			else {
				return true;
			}
			myPtr = myPtr -> myNext;
			myPtr2 = myPtr2 -> myNext;
		}
	}
	if (mySize != original.mySize) {
		return false;
	}
	return true;
}
//checks file is open and then writes to it
template <class Item>
void List<Item>::writeTo(string fileName) const {
	ofstream fout(fileName.c_str());
	assert(fout.is_open());
	Node* myPtr = myFirst;
	while(myPtr) {
		fout << myPtr->myItem << '\t';
		myPtr = myPtr->myNext;
	}
}

template <class Item>
void List<Item>::readFrom(istream& in) {
	Item it;
	while (in) { //while in !=0
		in >> it;
		append(it);
	}
}
//adds node in beginning
template <class Item>
void List<Item>::prepend(const Item& it) {
	Node *myPtr = new Node(it, ( mySize==0? NULL : myFirst ) );
	if ( mySize == 0 ) { myLast = myPtr; }
	myFirst = myPtr;
	mySize++;
}

//inserts node at any point
template <class Item>
void List<Item>::insert(const Item it, unsigned index) {
	Node *nodePtr2 = myFirst;

	if(mySize == 0||index == 0) {
		prepend(it);
	}
	else{
		for(unsigned i = 0; i <index-1; i++){
			nodePtr2 = nodePtr2->myNext;
		}
		nodePtr2->myNext = new Node(it,nodePtr2->myNext);
		if (index >= mySize) {
			myLast = new Node(it,NULL);
		}
		mySize++;
	}
}


// Michael Sisko, List Functions:

/* Overload Inequality operator
 * @param:			l2, a List template
 * Precondition:	Checks if dimensions of Node Items match
 * Postcondition:	Otherwise returns that Node Items in do not match
 * AUTHORED BY:		Michael Sisko
 */
template <class Item>
bool List<Item>::operator!=(const List& l2) const {
	if ( mySize == l2.getSize() ) {
		Node* myNode = myFirst;
		Node* l2Node = l2.myFirst;
		while ( myNode != nullptr) {
			if ( myNode->myItem != l2Node->myItem ) {
				return true;
			}
			myNode = myNode->myNext;
			l2Node = l2Node->myNext;
		}
		return false;
	} else {
		return true;
	}
}

/* Write to console
 * @param:			out, an ostream
 * Precondition:	stream contains values in list
 * Postcondition:	values printed to screen
 * AUTHORED BY:		Michael Sisko
 */
template <class Item>
void List<Item>::writeTo( ostream& out ) {
	Node* myNode = myFirst;
	for ( unsigned i = 0; i < mySize; i++ ) {
		out << myNode->myItem << '\n';
		myNode = myNode->myNext;
	}
}

/* Index Function
 * @param:			it, an Item
 * Precondition:	it contains a value within a Node
 * Postcondition:	function returns position in list
 * Postcondition:	function returns -1 if list empty
 * AUTHORED BY:		Michael Sisko
 */
template <class Item>
int List<Item>::getIndexOf( const Item& it ) {
	Node* myNode = myFirst;
	int index = 0;
	if ( mySize > 0 ) {
		while ( it != myNode->myItem ) {
			if ( myNode == nullptr ) {
				throw range_error( "Cannot get index for empty list.");
			} else {
				myNode = myNode->myNext;
				index++;
			}
		}
		return index;
	} else {
		return -1;
	}
}

/* Node Remove Function
 * @param:			index, an unsigned
 * Precondition:	index contains position of Node to be removed
 * Postcondition:	Node at index is removed
 * AUTHORED BY:		Michael Sisko
 */
template <class Item>
Item List<Item>::remove( unsigned index ) {
	Node* myNode = myFirst;
	for ( unsigned i = 0; i <= index; i++ ) {
		if ( i+1 == index ) {
			Node* temp = myNode->myNext->myNext;
			myNode->myNext = nullptr;
			delete myNode->myNext;
			myNode->myNext = temp;
			mySize--;
			break;
		} else if (index == 0) {
			Node* temp = myFirst->myNext;
			myFirst->myNext = nullptr;
			delete myFirst;
			myFirst = temp;
			mySize--;
			break;
		} else {
			myNode = myNode->myNext;
		}
	}
	return myNode->myItem;
}

/* Reads from fileName
 * @param:			fileName, a string
 * Precondition:	fileName contains a file name to be read
 * Postcondition:	values are incorporated into List
 * AUTHORED BY:		Michael Sisko
 */
template <class Item>
void List<Item>::readFrom( const string& fileName ) {
	ifstream fin( fileName.c_str() );
	assert( fin.is_open() );

	Item it;
	while( mySize < 4 ) {
		fin >> it;
		append(it);
	}
	fin.close();
}

#endif /* LIST_H_ */







