//Dilara Ademoðlu
//This is the already written Linkedlist template class from CS204. I copied it and edited it, since it was told that we can use previous classes for Linkedlist.
#ifndef _LINKEDLISTTEMPLATE_H
#define _LINKEDLISTTEMPLATE_H

template <class itemType>
struct node {
	itemType info;
	node *next; 
	node ()
	{}

	node (itemType s, node * link)
		: info(s), next (link)
	{ }
	
};

template <class itemType>
class LinkedList 
{
	public:
		 int size;
		 node<itemType> *head;
		 LinkedList();
		 void printList() const;
		 void add(itemType n);
		 void deleteList ();
};

#include "LinkedListTemplate.cpp"
#endif