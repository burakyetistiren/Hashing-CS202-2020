#include "HashTable.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <string>
/**
* Author : Burak Yetistiren
* ID: 21802608
* Section : 1
* Assignment : 4
*/
#include <string>
using namespace std;

HashTable::HashTable( const int tableSize, const CollisionStrategy option )
{
    keys = new Item[tableSize];
    strategy = option;
    size = tableSize;
    for( int i = 0; i < size; ++i)
    {
        keys[i].type = EMPTY;
        keys[i].key = NULL;
    }
}

HashTable::~HashTable()
{
    delete []keys;
}

bool HashTable::insert( const int item )
{
    int index = item % size;
    int baseIndex = index;

    //initializing k for quadratic hashing approach
    int k = 1;

    //reversing the number for double hashing approach
    int toBeReversed = item, reversed = 0, remainder;
    while(toBeReversed != 0)
    {
        remainder = toBeReversed % 10;
        reversed = reversed * 10 + remainder;
        toBeReversed /= 10;
    }

    while(keys[index].type == OCCUPIED)
    {
        if(strategy == LINEAR)
        {
            index = (index + 1) % size;
        }
        if(strategy == QUADRATIC)
        {
            index = baseIndex;
            index = (index + k*k) % size;
            ++k;
        }
        if(strategy == DOUBLE)
        {
            index = baseIndex;
            index = (index + k*reversed) % size;
            ++k;
        }
        if( index == baseIndex)
            return false;
    }
    keys[index].key = item;
    keys[index].type = OCCUPIED;
    return true;
}

bool HashTable::remove( const int item )
{
    bool found = false;
    int index = item % size;
    int baseIndex = index;
    //initializing k for quadratic hashing approach
    int k = 1;

    //reversing the number for double hashing approach
    int toBeReversed = item, reversed = 0, remainder;
    while(toBeReversed != 0)
    {
        remainder = toBeReversed % 10;
        reversed = reversed * 10 + remainder;
        toBeReversed /= 10;
    }

    while(keys[index].type != EMPTY)
    {
        if(keys[index].key == item)
        {
            found = true;
            break;
        }
        if(strategy == LINEAR)
        {
            index = (index + 1) % size;
        }
        if(strategy == QUADRATIC)
        {
            index = baseIndex;
            index = (index + k*k) % size;
            ++k;
        }
        if(strategy == DOUBLE)
        {
            index = baseIndex;
            index = (index + k*reversed) % size;
            ++k;
        }

        if( index == baseIndex)
            return false;
    }
    if(found)
    {
        keys[index].key = NULL;
        keys[index].type = DELETED;
        return true;
    }
    return false;
}

bool HashTable::search( const int item, int& numProbes )
{
    int index = item % size;
    int baseIndex = index;
    numProbes = 0;
    //initializing k for quadratic hashing approach
    int k = 1;

    //reversing the number for double hashing approach
    int toBeReversed = item, reversed = 0, remainder;
    while(toBeReversed != 0)
    {
        remainder = toBeReversed % 10;
        reversed = reversed * 10 + remainder;
        toBeReversed /= 10;
    }

    while( keys[index].type != EMPTY)
    {
        if(keys[index].key == item)
            return true;
        if(strategy == LINEAR)
        {
            index = (index + 1) % size;
            ++numProbes;
        }
        if(strategy == QUADRATIC)
        {
            index = baseIndex;
            index = (index + k*k) % size;
            ++k;
            ++numProbes;
        }
        if(strategy == DOUBLE)
        {
            index = baseIndex;
            index = (index + k*reversed) % size;
            ++k;
            ++numProbes;
        }
        if( index == baseIndex)
            return false;
    }
    return false;
}

void HashTable::display()
{
    int number = 0;
    for( int i = 0; i < size; ++i)
    {
        if(keys[i].key)
        {
            cout << i << ":" << keys[i].key << endl;
            ++number;
        }

        else
        {
            if( keys[i].type == OCCUPIED)
            {
                cout << i << ":" << keys[i].key << endl;
                ++number;
            }
            else
                cout << i << ":" << endl;
        }
    }
    cout << "Number of elements in the table: " << number << endl;
    cout << endl;
}

int HashTable::getNoOfElemets()
{
    int number = 0;
    for( int i = 0; i < size; ++i)
    {
        if(keys[i].key)
            ++number;
        else
        {
            if( keys[i].type == OCCUPIED)
                ++number;
        }
    }
}

int HashTable::getSize(){return size;}

void HashTable::analyze( int& numSuccProbes, int& numUnsuccProbes )
{
    int successful = 0;
    int unsuccessful = 0;
    int dummy = 0, dummyItem;

    for( int i = 0; i < size; ++i)
    {
        if(keys[i].key)
        {
            search(keys[i].key, successful);
            numSuccProbes = numSuccProbes + successful;
            if( strategy != DOUBLE)
            {
                dummyItem = i;
                while( search(dummyItem, dummy))
                    dummyItem += size;
                search(dummyItem, unsuccessful);
                dummyItem = 0;
                numUnsuccProbes = numUnsuccProbes + unsuccessful;
            }
            else
                numUnsuccProbes = -1;
        }
        else
        {
            if(strategy == DOUBLE)
                numUnsuccProbes = -1;
            else
            {
                search(i, unsuccessful);
                numUnsuccProbes = numUnsuccProbes + unsuccessful;
            }
        }
    }
}

//int main()
//{
//    int noOfProbes1 = 0, noOfProbes2 = 0;
//    HashTable* table = new HashTable( 11, LINEAR);
//    table->insert(1);
//    table->insert(0);
////    table->insert(23);
////    table->insert(44);
////    table->insert(232);
////    table->insert(445);
////    table->insert(655);
////    table->insert(7676);
////    table->insert(656);
////    table->insert(330);
////    table->insert(5454);
//
//    table->display();
//
//    //cout << table->search(6, noOfProbes1) << endl;
//    //cout << table->search(1000,noOfProbes2) << endl;
//    //cout << noOfProbes1 << endl;
//    table->analyze(noOfProbes1, noOfProbes2);
//
////    cout << noOfProbes1 << endl;
////    cout << noOfProbes2 << endl;
//
//    delete table;
//
//    return 0;
//}

int main()
{
    int noOfProbesSucc = 0, noOfProbesUnsucc = 0;
    HashTable* tableLinear = new HashTable( 11, LINEAR);
    HashTable* tableQuadratic = new HashTable( 11, QUADRATIC);
    HashTable* tableDouble = new HashTable( 11, DOUBLE);

    char x;
    string s = "";
    int value = 0, temp = 0, valueToBeAdded = 0, noOfProbes = 0;

    ifstream inFile;
    inFile.open("input.txt");
    if (!inFile)
        cout << "Unable to open file";

    while (inFile >> x)
    {
        if ( x == 'I')
        {
            getline( inFile, s);

            for( int i = 1; i < s.size() - 1; ++i)
            {
                temp = s[i] - '0';
                valueToBeAdded = ceil(temp * pow( 10, s.size() - (i + 2)));
                value = value + valueToBeAdded;
            }
            if( tableLinear->insert(value))
                cout << value << " inserted. (LINEAR)" << endl;
            else
                cout << value << " not inserted. (LINEAR)" << endl;

            if( tableQuadratic->insert(value))
                cout << value << " inserted. (QUADRATIC)" << endl;
            else
                cout << value << " not inserted. (QUADRATIC)" << endl;

            if( tableDouble->insert(value))
                cout << value << " inserted. (DOUBLE)" << endl;
            else
                cout << value << " not inserted. (DOUBLE)" << endl;
            s = "";
            value = 0;
            cout << endl;
        }
        if ( x == 'R')
        {
            getline( inFile, s);

            for( int i = 1; i < s.size() - 1; ++i)
            {
                temp = s[i] - '0';
                valueToBeAdded = ceil(temp * pow( 10, s.size() - (i + 2)));
                value = value + valueToBeAdded;
            }

            if( tableLinear->remove(value))
                cout << value << " removed. (LINEAR)" << endl;
            else
                cout << value << " not removed. (LINEAR)" << endl;
            if( tableQuadratic->remove(value))
                cout << value << " removed. (QUADRATIC)" << endl;
            else
                cout << value << " not removed. (QUADRATIC)" << endl;
            if( tableDouble->remove(value))
                cout << value << " removed. (DOUBLE)" << endl;
            else
                cout << value << " not removed. (DOUBLE)" << endl;
            s = "";
            value = 0;
            cout << endl;
        }
        if ( x == 'S')
        {
            getline( inFile, s);

            for( int i = 1; i < s.size() - 1; ++i)
            {
                temp = s[i] - '0';
                valueToBeAdded = ceil(temp * pow( 10, s.size() - (i + 2)));
                value = value + valueToBeAdded;
            }

            if( tableLinear->search(value, noOfProbes))
                cout << value << " found after " << noOfProbes << " probes. (LINEAR)" << endl;
            else
                cout << value << " not found after " << noOfProbes << " probes. (LINEAR)" << endl;
            if( tableQuadratic->search(value, noOfProbes))
                cout << value << " found after " << noOfProbes << " probes. (QUADRATIC)" << endl;
            else
                cout << value << " not found after " << noOfProbes << " probes. (QUADRATIC)" << endl;
            if( tableLinear->search(value, noOfProbes))
                cout << value << " found after " << noOfProbes << " probes. (DOUBLE)" << endl;
            else
                cout << value << " not found after " << noOfProbes << " probes. (DOUBLE)" << endl;
            s = "";
            value = 0;
            noOfProbes = 0;
            cout << endl;
        }
    }

    inFile.close();

    //print linear table
    cout << endl;
    cout << "----------------------LINEAR TABLE----------------------" << endl;
    cout << "Table size: " << tableLinear->getSize() << endl;
    tableLinear->display();

    tableLinear->analyze(noOfProbesSucc, noOfProbesUnsucc);
    cout << "Average number of probes for successful searches: "
    << (double)noOfProbesSucc / tableLinear->getNoOfElemets()  << "\n";
    cout << "Average number of probes for unsuccessful searches: "
    << (double)noOfProbesUnsucc / tableLinear->getSize() << endl;
    cout << "--------------------------------------------------------" << endl << endl;

    //print quadratic table
    cout << "---------------------QUADRATIC TABLE--------------------" << endl;
    cout << "Table size: " << tableQuadratic->getSize() << endl;
    tableQuadratic->display();

    tableQuadratic->analyze(noOfProbesSucc, noOfProbesUnsucc);
    cout << "Average number of probes for successful searches: "
    << (double)noOfProbesSucc / tableQuadratic->getNoOfElemets() << "\n";
    cout << "Average number of probes for unsuccessful searches: "
    << (double)noOfProbesUnsucc / tableQuadratic->getSize()<< endl;
    cout << "--------------------------------------------------------" << endl << endl;

    //print double table
    cout << "-----------------------DOUBLE TABLE---------------------" << endl;
    cout << "Table size: " << tableDouble->getSize() << endl;
    tableDouble->display();

    tableDouble->analyze(noOfProbesSucc, noOfProbesUnsucc);
    cout << "Average number of probes for successful searches: "
    << (double)noOfProbesSucc / tableDouble->getNoOfElemets() << "\n";
    cout << "The analysis is made only for successful searches in double hashing" << endl;
    cout << "---------------------------------------------------------------------" << endl << endl;

    delete tableLinear;
    delete tableDouble;
    delete tableQuadratic;

    return 0;
}
