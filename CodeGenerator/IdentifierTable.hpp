//
//  IdentifierTable.hpp
//  CodeGenerator
//
//  Created by Kyle Bludworth on 4/12/16.
//  Copyright © 2016 Kyle Bludworth. All rights reserved.
//

#ifndef IdentifierTable_hpp
#define IdentifierTable_hpp

#include <iostream>
#include <cmath>
#include "TableEntry.h"

using std::string;

class IdentifierTable
{
private:
    TableEntry **table;
    const int TABLE_SIZE = 523;
    int hashIdentifier(string identifier);
public:
    IdentifierTable();
    ~IdentifierTable();
    void addEntry(string identifier, string type, int kind, int number);
    int probe(string identifier);
    string getType(string identifier);
    int getKindOf(string identifier);
    int getNumber(string identifier);
    bool contains(string identifier);
};

#endif /* IdentifierTable_hpp */
