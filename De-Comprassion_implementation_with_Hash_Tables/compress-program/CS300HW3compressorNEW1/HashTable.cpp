#include <iostream>
#include "hashtable.h"
using namespace std;

int hash(const string & key, int tableSize)
{
	int sum = 0;
	int keyLength = key.length();
	for (int index = 0; index < keyLength; index++)
		sum = sum + static_cast<int>(key[index]);

	return (sum % tableSize);
}

template <class HashedObj>
HashTable<HashedObj>::HashTable()
{
	for (int i = 0; i < 4096; i++)
		arrayHash.push_back(HashEntry("", EMPTY));

}

// Return the position where the search for x terminates
template <class HashedObj>
int HashTable<HashedObj>::findPos(const HashedObj & x) const
{
	int cursor = 0;

	while (arrayHash[cursor].info != EMPTY && arrayHash[cursor].element != x)
		cursor++;
	if (cursor >= arrayHash.size())
		cursor = cursor - arrayHash.size();
	return cursor;
}

// Return true if cursor exists and is active.
template <class HashedObj>
bool HashTable<HashedObj>::isActive(int cursor) const
{
	return arrayHash[cursor].info == ACTIVE;
}

/*** Remove item x from the hash table.  x has to be in the table*/
template <class HashedObj>
void HashTable<HashedObj>::remove(const HashedObj & x)
{
	int cursor = findPos(x);
	if (isActive(cursor))
		arrayHash[cursor].info = DELETED;
}

/*** Find item x in the hash table. Return the matching item, or ITEM_NOT_FOUND, if not found.*/
template <class HashedObj>
const HashedObj & HashTable<HashedObj>::find(const HashedObj & x) const
{
	int cursor = findPos(x);
	if (isActive(cursor))
		return arrayHash[cursor].element;

	return   ITEM_NOT_FOUND;
}

/*** Insert item x into the hash table. If the item is already present, then do nothing.*/
template <class HashedObj>
void HashTable<HashedObj>::insert(const HashedObj & x)
{
	// Insert x as active
	int cursor = findPos(x);
	if (isActive(cursor))
		return;
	arrayHash[cursor] = HashEntry(x, ACTIVE);

	//// enlarge the hash table if necessary 
	//if (++currentSize >= array.size() / 2)
	//	rehash();
}

/**
* Expand the hash table.
*/
template <class HashedObj>
void HashTable<HashedObj>::rehash()
{
	vector<HashEntry> oldArray = array;

	// Create new double-sized, empty table
	array.resize(nextPrime(2 * oldArray.size()));
	for (int j = 0; j < array.size(); j++)
		array[j].info = EMPTY;

	// Copy table over
	currentSize = 0;
	for (int i = 0; i < oldArray.size(); i++)
		if (oldArray[i].info == ACTIVE)
			insert(oldArray[i].element);
}