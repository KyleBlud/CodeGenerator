//
//  SymbolTable.cpp
//  CodeGenerator
//
//  Created by Kyle Bludworth on 4/12/16.
//  Copyright © 2016 Kyle Bludworth. All rights reserved.
//

#include "SymbolTable.hpp"

/*
 This data structure was strongly influenced by the example given below:
 http://www.algolist.net/Data_structures/Hash_table/Simple_example
 */
/*
 The function hashSymbol() was strongly influenced by the user jonathanasdf
 on stackoverflow:
 http://stackoverflow.com/questions/2624192/good-hash-function-for-strings
 */

#include "SymbolTable.hpp"

SymbolTable::SymbolTable()
{
    table = new TableEntry*[TABLE_SIZE];
    
    for (int i = 0; i < TABLE_SIZE; i++)
        table[i] = NULL;
}

SymbolTable::~SymbolTable()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (table[i] != NULL)
            delete table[i];
    }
    
    delete[] table;
}

int SymbolTable::hashSymbol(string symbol)
{
    int hash = 7;
    
    for (int i = 0; i < symbol.length(); i++)
    {
        hash = (hash * 31) + symbol[i];
    }
    
    return std::abs(hash);
}

void SymbolTable::addEntry(string symbol, int address)
{
    int hash = hashSymbol(symbol) % TABLE_SIZE;
    
    while (table[hash] != NULL && table[hash]->getKey() != symbol)
    {
        hash = (hash + 1) % TABLE_SIZE;
    }
    
    if (table[hash] != NULL)
        delete table[hash];
    
    table[hash] = new TableEntry(symbol, address);
}

int SymbolTable::getAddress(string symbol)
{
    int hash = hashSymbol(symbol) % TABLE_SIZE;
    
    while (table[hash] != NULL && table[hash]->getKey() != symbol)
    {
        hash = (hash + 1) % TABLE_SIZE; // Probe, linear like.
    }
    
    if (table[hash] == NULL)
        return -1;
    else
        return table[hash]->getValue();
}

bool SymbolTable::contains(string symbol)
{
    int hash = hashSymbol(symbol) % TABLE_SIZE;
    
    while (table[hash] != NULL && table[hash]->getKey() != symbol)
    {
        hash = (hash + 1) % TABLE_SIZE;
    }
    
    if (table[hash] == NULL)
        return false;
    else
        return true;
}