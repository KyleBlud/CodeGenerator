/*
 JackTokenizer.hpp
 SyntaxAnalyzer
 
 Project 10: Compiler I, Syntax Analysis
 
 The JackTokenizer and CompilationEngine modules act as a Syntax Analyzer that
 parses Jack programs according to the Jack grammar, producing an XML file
 that captures the program's structure.
 
 Created by Kyle Bludworth & Josh Burks on 4/3/16.
 Copyright © 2016 Kyle Bludworth. All rights reserved.
 */

#ifndef JackTokenizer_hpp
#define JackTokenizer_hpp

#include <iostream>
#include <vector>

using std::string;
using std::vector;

enum TokenType
{
    T_KEYWORD = 1,
    T_SYMBOL,
    T_IDENTIFIER,
    T_INT_CONST,
    T_STRING_CONST
};

enum Keyword
{
    K_CLASS = 1,
    K_METHOD,
    K_FUNCTION,
    K_CONSTRUCTOR,
    K_INT,
    K_BOOLEAN,
    K_CHAR,
    K_VOID,
    K_VAR,
    K_STATIC,
    K_FIELD,
    K_LET,
    K_DO,
    K_IF,
    K_ELSE,
    K_WHILE,
    K_RETURN,
    K_TRUE,
    K_FALSE,
    K_NULL,
    K_THIS
};

class JackTokenizer
{
private:
    static const int KW_SIZE = 21;
    static const int SYM_SIZE = 19;
    int listIndex;
    string token;
    vector<string> tokenList;
    string keywordList[21];
    string symbolList[19];
    bool inComment;
    
private:
    bool isKeyword();
    bool isSymbol(char token);
    bool isSymbol();
    bool isIntVal();
    bool isStringVal();
    void defineKeywords();
    void defineSymbols();
    
public:
    JackTokenizer();
    void nextToken();
    string getTokenInList(int index);
    int getTokenListSize();
    string getToken();
    void setListIndex(int index);
    int getListIndex();
    void tokenize(string line);
    int tokenType();
    int keyword();
    string symbol();
    string identifier();
    string intVal();
    string stringVal();
    bool isBlockComment(string line);
    bool isInLineComment(string line);
};

#endif /* JackTokenizer_hpp */
