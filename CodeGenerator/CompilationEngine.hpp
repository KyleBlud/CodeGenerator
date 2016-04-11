/*
 CompilationEngine.hpp
 SyntaxAnalyzer
 
 Project 10: Compiler I, Syntax Analysis
 
 The JackTokenizer and CompilationEngine modules act as a Syntax Analyzer that
 parses Jack programs according to the Jack grammar, producing an XML file
 that captures the program's structure.
 
 Created by Kyle Bludworth & Josh Burks on 4/3/16.
 Copyright © 2016 Kyle Bludworth. All rights reserved.
 */

#ifndef CompilationEngine_hpp
#define CompilationEngine_hpp

#include <iostream>
#include <fstream>
#include "JackTokenizer.hpp"

using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

class CompilationEngine
{
private:
    int const INDENT_SPACES = 2;
    int indentLevel;
    JackTokenizer jt;
    vector<string> XMLcode;
    string inFileName;
    
private:
    void buildTokenList();
    void writeXMLFile();
    void compileClass();
    void compileClassVarDec();
    void compileSubroutine();
    void compileParameterList();
    void compileSubroutineBody();
    void compileVarDec();
    void compileStatements();
    void compileDo();
    void compileLet();
    void compileWhile();
    void compileReturn();
    void compileIf();
    void compileExpression();
    void compileTerm();
    void compileExpressionList();
    void writeKeyword(string token, int keywordType);
    void writeIdentifier();
    void writeSymbol(string token);
    void writeIntVal();
    void writeStringVal();
    void writeType();
    void writeVarDec();
    void writeSubroutineCall();
    void writeEnclosedExpression(string open, string close);
    void writeEnclosedStatements();
    void writeError(string errorMessage);
    void startTag(string token);
    void endTag(string token);
    void tagTerminal(string token);
    string addIndentToLine();
    bool isTokenKeyword();
    bool isTokenSymbol();
    bool isTokenIdentifier();
    bool isTokenIntConst();
    bool isTokenStringConst();
    bool isSubroutineCall();
    bool isArrayReference();
    bool isValidStatementKeyword();
    bool isValidVarDecKeyword();
    bool isValidSubDecKeyword();
    bool isValidOpSymbol();
    
public:
    CompilationEngine(string inFileName);
};

#endif /* CompilationEngine_hpp */
