#ifndef __HASH_TABLE_H
#define __HASH_TABLE_H
/**
* Author : Burak Yetistiren
* ID: 21802608
* Section : 1
* Assignment : 4
*/
#include <string>
using namespace std;

enum CollisionStrategy {LINEAR, QUADRATIC, DOUBLE};
enum ItemType {OCCUPIED, EMPTY, DELETED};

class HashTable{

public:
    HashTable( const int tableSize, const CollisionStrategy option );
    ~HashTable();
    bool insert( const int item );
    bool remove( const int item );
    bool search( const int item, int& numProbes );
    int getSize();
    int getNoOfElemets();
    void display();
    void analyze( int& numSuccProbes, int& numUnsuccProbes );

private:
    struct Item
    {
        int key;
        ItemType type;
    };
    Item* keys;
    int size;
    CollisionStrategy strategy;
};
#endif // __HASH_TABLE_H
