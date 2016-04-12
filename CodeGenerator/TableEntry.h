//
//  TableEntry.h
//  CodeGenerator
//
//  Created by Kyle Bludworth on 4/12/16.
//  Copyright © 2016 Kyle Bludworth. All rights reserved.
//
/*
 This data structure was strongly influenced by the example given below:
 http://www.algolist.net/Data_structures/Hash_table/Simple_example
*/

#ifndef TableEntry_h
#define TableEntry_h

#include <iostream>

using std::string;

class TableEntry
{
private:
    string symbol; // key
    int address; // value
public:
    TableEntry(string symbol, int address)
    {
        this->symbol = symbol;
        this->address = address;
    }
    
    string getKey()
    {
        return symbol;
    }
    
    int getValue()
    {
        return address;
    }
};

#endif /* TableEntry_h */
