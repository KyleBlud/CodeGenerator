//
//  IdentifierTable.cpp
//  CodeGenerator
//
//  Created by Kyle Bludworth on 4/12/16.
//  Copyright © 2016 Kyle Bludworth. All rights reserved.
//

#include "IdentifierTable.hpp"

/*
 This data structure was strongly influenced by the example given below:
 http://www.algolist.net/Data_structures/Hash_table/Simple_example
 */
/*
 The function hashIdentifier() was strongly influenced by the user jonathanasdf
 on stackoverflow:
 http://stackoverflow.com/questions/2624192/good-hash-function-for-strings
 */

#include "IdentifierTable.hpp"

IdentifierTable::IdentifierTable()
{
    table = new TableEntry*[TABLE_SIZE];
    
    for (int i = 0; i < TABLE_SIZE; i++)
        table[i] = NULL;
}

IdentifierTable::~IdentifierTable()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (table[i] != NULL)
            delete table[i];
    }
    
    delete[] table;
}

int IdentifierTable::hashIdentifier(string identifier)
{
    int hash = 7;
    
    for (int i = 0; i < identifier.length(); i++)
    {
        hash = (hash * 31) + identifier[i];
    }
    
    return std::abs(hash);
}

void IdentifierTable::addEntry(string identifier, string type, int kind,
                               int number)
{
    int hash = probe(identifier);
    
    if (table[hash] != NULL)
        delete table[hash];
    
    table[hash] = new TableEntry(identifier, type, kind, number);
}

int IdentifierTable::probe(string identifier)
{
    int hash = hashIdentifier(identifier) % TABLE_SIZE;
    
    while (table[hash] != NULL && table[hash]->getKey() != identifier)
    {
        hash = (hash + 1) % TABLE_SIZE; // Probe, linear like.
    }
    
    return hash;
}

string IdentifierTable::getType(string identifier)
{
    int hash = probe(identifier);
    
    if (table[hash] == NULL)
        return "";
    else
        return table[hash]->getType();
}

int IdentifierTable::getKindOf(string identifier)
{
    int hash = probe(identifier);
    
    if (table[hash] == NULL)
        return -1;
    else
        return table[hash]->getKindOf();
}

int IdentifierTable::getNumber(string identifier)
{
    int hash = probe(identifier);
    
    if (table[hash] == NULL)
        return -1;
    else
        return table[hash]->getNumber();
}

bool IdentifierTable::contains(string identifier)
{
    int hash = probe(identifier);
    
    if (table[hash] == NULL)
        return false;
    else
        return true;
}