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

struct Identifier
{
    string type;
    int kind;
    int number;
};

class TableEntry
{
private:
    string key; // key
    Identifier values; // values
public:
    TableEntry(string key, string type, int kind, int number)
    {
        this->key = key;
        values.type = type;
        values.kind = kind;
        values.number = number;
    }

    string getKey()
    {
        return key;
    }
    
    string getType()
    {
        return values.type;
    }
    
    int getKindOf()
    {
        return values.kind;
    }
    
    int getNumber()
    {
        return values.number;
    }
};

#endif /* TableEntry_h */
