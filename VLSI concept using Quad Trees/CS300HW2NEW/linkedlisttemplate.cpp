//This is the already written Linkedlist template class from CS204. I copied it and edited a little,
//since it was told that we can use previous classes for Linkedlist.
#include <iostream>
#include <string>
#include "LinkedListTemplate.h"
using namespace std;

//default constructor
template <class itemType>
LinkedList<itemType>::LinkedList ()
{
	head = NULL;
	size = 0;
}
template <class itemType>
void LinkedList<itemType>::add (itemType n)
{
	node<itemType> *ptr;
	if(head==NULL)
	{
		ptr = new node<itemType>(n,head);
		head = ptr;	
		size++;
	}
	else
	{
		ptr = head;
		while(ptr->next!=NULL)
			ptr=ptr->next;
		node<itemType> *p=new node<itemType>(n,ptr->next);
		ptr=p;
	}
}	

template <class itemType>
void LinkedList<itemType>::printList () const
{
	node<itemType> * ptr = head;
    while (ptr != NULL) 
	{
		cout << ptr ->info << endl;
		ptr = ptr->next;
	}
	cout << endl;
}

template <class itemType>
void LinkedList<itemType>::deleteList ()
{
	node<itemType> * ptr = head;
    while (ptr != NULL) 
	{
		node<itemType> * temp = ptr->next;
		delete ptr;
		ptr = temp;
	}
	head = NULL;
	size = 0;
}

