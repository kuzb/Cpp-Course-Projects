#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <string>
#include <iostream>
#include <vector>
#define table_size 4096
#define HASH_MACRO(key, table_size) (key & (table_size - 1))
using namespace std;
template <class HashedObj>
class HashTable
{
public:
	HashTable();

	const HashedObj & find(const HashedObj & x) const;

	void insert(const HashedObj & x);
	void remove(const HashedObj & x);

	const HashTable & operator=(const HashTable & rhs);

	enum EntryType { ACTIVE, EMPTY, DELETED };

	int findPos(const HashedObj & x) const;
	struct HashEntry
	{
		HashedObj element;
		EntryType info;

		HashEntry(const HashedObj & e = HashedObj(), EntryType i = EMPTY)
			: element(e), info(i) {}
	};
	vector<HashEntry> arrayHash;

	bool isActive(int cursor) const;
	
private:
	const HashedObj ITEM_NOT_FOUND;
	unsigned int key;
	void rehash();
};


int hash(const string & key, int tableSize);




#include "hashtable.cpp"
#endif