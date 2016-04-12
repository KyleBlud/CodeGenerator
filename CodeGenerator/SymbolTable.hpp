//
//  SymbolTable.hpp
//  CodeGenerator
//
//  Created by Kyle Bludworth on 4/12/16.
//  Copyright © 2016 Kyle Bludworth. All rights reserved.
//

#ifndef SymbolTable_hpp
#define SymbolTable_hpp

#include <iostream>
#include <cmath>
#include "TableEntry.h"

using std::string;

class SymbolTable
{
private:
    TableEntry **table;
    const int TABLE_SIZE = 523;
    int hashSymbol(string symbol);
    void addPredefinedSymbols();
public:
    SymbolTable();
    ~SymbolTable();
    void addEntry(string symbol, int address);
    int getAddress(string symbol);
    bool contains(string symbol);
};

#endif /* SymbolTable_hpp */
