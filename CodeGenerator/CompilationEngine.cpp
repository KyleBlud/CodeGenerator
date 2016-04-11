/*
 CompilationEngine.cpp
 SyntaxAnalyzer
 
 Project 10: Compiler I, Syntax Analysis
 
 The JackTokenizer and CompilationEngine modules act as a Syntax Analyzer that
 parses Jack programs according to the Jack grammar, producing an XML file
 that captures the program's structure.
 
 For more information, go to:
 www.nand2tetris.org
 
 Created by Kyle Bludworth & Josh Burks on 4/3/16.
 Copyright © 2016 Kyle Bludworth. All rights reserved.
 */

#include "CompilationEngine.hpp"

/*
 Program consists of 3 stages: Building the token list, compiling the class 
 which initializes the recursive descent parsing, and then writes the parsed
 code into an .xml file.
 */
CompilationEngine::CompilationEngine(string inFileName)
{
    this->inFileName = inFileName;
    buildTokenList();
    compileClass();
    writeXMLFile();
}

/*
 Uses the JackTokenizer to build a list of the program's tokens.
 */
void CompilationEngine::buildTokenList()
{
    ifstream jackFile(inFileName);
    string line;
    string token;
    
    if (jackFile.is_open())
    {
        while (getline(jackFile, line))
        {
            if (!jt.isBlockComment(line) && !jt.isInLineComment(line))
            {
                jt.tokenize(line);
            }
        }
        jackFile.close();
    }
}

/*
 Writes the finished parsed code into an .xml file
 */
void CompilationEngine::writeXMLFile()
{
    
    string outFileName;
    outFileName += "Out";
    outFileName += inFileName.erase(inFileName.find("."), inFileName.length());
    outFileName += ".xml";
    
    ofstream XMLFile(outFileName);
    
    if (XMLFile.is_open())
    {
        for (int i = 0; i < XMLcode.size(); i++)
        {
            XMLFile << XMLcode.at(i) << endl;
        }
    }
    
    XMLFile.close();
}

/*
 'class' className '{' classVarDec* subroutineDec* '}'
 */
void CompilationEngine::compileClass()
{
    indentLevel = 0;
    jt.nextToken();
    startTag("class");
    
    writeKeyword("class", K_CLASS);
    jt.nextToken();
    writeIdentifier();
    jt.nextToken();
    writeSymbol("{");
    jt.nextToken();
    
    while (isTokenKeyword() && isValidVarDecKeyword())
    {
        compileClassVarDec();
    }
    
    while (isTokenKeyword() && isValidSubDecKeyword())
    {
        compileSubroutine();
    }
    writeSymbol("}");
    
    indentLevel = 0;
    endTag("class");
}

/*
 ('static' | 'field') type varName (',', varName)* ';'
 */
void CompilationEngine::compileClassVarDec()
{
    startTag("classVarDec");
    
    writeKeyword(jt.getToken(), jt.keyword());
    jt.nextToken();
    writeVarDec();
    jt.nextToken();
    
    endTag("classVarDec");
}

/*
 ('constructor' | 'function' | 'method') ('void' | type) subroutineName
 '(' parameterList ')' subroutineBody
 */
void CompilationEngine::compileSubroutine()
{
    startTag("subroutineDec");
    
    writeKeyword(jt.getToken(), jt.keyword());
    jt.nextToken();
    if (isTokenKeyword())
        writeKeyword("void", K_VOID);
    else if (isTokenIdentifier())
        writeIdentifier();
    jt.nextToken();
    writeIdentifier();
    jt.nextToken();
    writeSymbol("(");
    jt.nextToken();
    compileParameterList();
    writeSymbol(")");
    jt.nextToken();
    compileSubroutineBody();
    
    endTag("subroutineDec");
}

/*
 ((type varName) (',' type varName)*)?
 */
void CompilationEngine::compileParameterList()
{
    startTag("parameterList");

    if (!isTokenSymbol())
    {
        writeType();
        jt.nextToken();
        writeIdentifier();
        jt.nextToken();
        while (isTokenSymbol() && jt.symbol() == ",")
        {
            writeSymbol(",");
            jt.nextToken();
            writeType();
            jt.nextToken();
            writeIdentifier();
            jt.nextToken();
        }
    }

    endTag("parameterList");
}

/*
 '{' varDec* statements '}'
 */
void CompilationEngine::compileSubroutineBody()
{
    startTag("subroutineBody");
    
    writeSymbol("{");
    jt.nextToken();
    compileVarDec();
    compileStatements();
    writeSymbol("}");
    jt.nextToken();
    
    endTag("subroutineBody");
}

/*
 'var' type varName (',' varName)* ';'
 */
void CompilationEngine::compileVarDec()
{
    if (isTokenKeyword() && jt.keyword() == K_VAR)
    {
        
        while (isTokenKeyword() && jt.keyword() == K_VAR)
        {
            startTag("varDec");
            writeKeyword("var", K_VAR);
            jt.nextToken();
            writeVarDec();
            jt.nextToken();
            endTag("varDec");
        }
    }
}

/*
 (letStatement | ifStatement | whileStatement | doStatement | returnStatement)*
 */
void CompilationEngine::compileStatements()
{
    startTag("statements");
    while (isTokenKeyword() && isValidStatementKeyword())
    {
        if (jt.keyword() == K_LET)
            compileLet();
        else if (jt.keyword() == K_IF)
            compileIf();
        else if (jt.keyword() == K_WHILE)
            compileWhile();
        else if (jt.keyword() == K_DO)
            compileDo();
        else if (jt.keyword() == K_RETURN)
            compileReturn();
    }
    endTag("statements");
}

/*
 'let' varName ('[' expression ']')? '=' expression ';'
 */
void CompilationEngine::compileLet()
{
    startTag("letStatement");
    
    writeKeyword("let", K_LET);
    jt.nextToken();
    writeIdentifier();
    jt.nextToken();
    if (isTokenSymbol() && jt.symbol() == "[")
    {
        writeEnclosedExpression("[", "]");
        jt.nextToken();
    }
    writeSymbol("=");
    jt.nextToken();
    compileExpression();
    writeSymbol(";");
    jt.nextToken();
    
    endTag("letStatement");
}

/*
 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
 */
void CompilationEngine::compileIf()
{
    startTag("ifStatement");
    
    writeKeyword("if", K_IF);
    jt.nextToken();
    writeEnclosedExpression("(",")");
    jt.nextToken();
    writeEnclosedStatements();
    jt.nextToken();
    if (isTokenKeyword())
    {
        if (jt.keyword() == K_ELSE)
        {
            writeKeyword("else", K_ELSE);
            jt.nextToken();
            writeEnclosedStatements();
            jt.nextToken();
        }
    }
    
    endTag("ifStatement");
}

/*
 'while' '(' expression ')' '{' statements '}'
 */
void CompilationEngine::compileWhile()
{
    startTag("whileStatement");
    
    writeKeyword("while", K_WHILE);
    jt.nextToken();
    writeEnclosedExpression("(",")");
    jt.nextToken();
    writeEnclosedStatements();
    jt.nextToken();
    
    endTag("whileStatement");
}

/*
 'do' subroutineCall ';'
 */
void CompilationEngine::compileDo()
{
    startTag("doStatement");
    
    writeKeyword("do", K_DO);
    jt.nextToken();
    writeSubroutineCall();
    jt.nextToken();
    writeSymbol(";");
    jt.nextToken();
    
    endTag("doStatement");
}

/*
 'return' expression? ';'
 */
void CompilationEngine::compileReturn()
{
    startTag("returnStatement");
    
    writeKeyword("return", K_RETURN);
    jt.nextToken();
    if (isTokenIdentifier() || isTokenKeyword() || jt.symbol() == "(")
    {
        compileExpression();
    }
    writeSymbol(";");
    jt.nextToken();
    
    endTag("returnStatement");
}

/*
 term (op term)*
 */
void CompilationEngine::compileExpression()
{
    startTag("expression");
    
    compileTerm();
    jt.nextToken();
    while (isTokenSymbol() && isValidOpSymbol())
    {
        writeSymbol(jt.symbol());
        jt.nextToken();
        compileTerm();
        jt.nextToken();
    }
    
    endTag("expression");
}

/*
 integerConstant | stringConstant | keywordConstant | varName |
 varName '[' expression ']' | subroutineCall | '(' expression ')' |
 unaryOp term
 */
void CompilationEngine::compileTerm()
{
    startTag("term");
    
    if (isTokenIntConst())
    {
        writeIntVal();
    }
    else if (isTokenStringConst())
    {
        writeStringVal();
    }
    else if (isTokenKeyword())
    {
        if (jt.keyword() == K_TRUE)
            writeKeyword("true", K_TRUE);
        else if (jt.keyword() == K_FALSE)
            writeKeyword("false", K_FALSE);
        else if (jt.keyword() == K_NULL)
            writeKeyword("null", K_NULL);
        else if (jt.keyword() == K_THIS)
            writeKeyword("this", K_THIS);
    }
    else if (isTokenIdentifier() && !isArrayReference() && !isSubroutineCall())
    {
        writeIdentifier();
    }
    else if (isTokenIdentifier() && isArrayReference())
    {
        writeIdentifier();
        jt.nextToken();
        writeEnclosedExpression("[", "]");
    }
    else if (isTokenIdentifier() && isSubroutineCall())
    {
        writeSubroutineCall();
    }
    else if (isTokenSymbol())
    {
        if (jt.symbol() != "(")
        {
            if (jt.symbol() == "-")
                writeSymbol("-");
            else if (jt.symbol() == "~")
                writeSymbol("~");
            else
                writeError("Expected unary operator");
            
            jt.nextToken();
            compileTerm();
        }
        else
        {
            writeEnclosedExpression("(",")");
        }
    }
    
    endTag("term");
}

/*
 (expression(',' expression)*)?
 */
void CompilationEngine::compileExpressionList()
{
    startTag("expressionList");
    
    if (!isTokenSymbol() || jt.symbol() == "(")
    {
        compileExpression();
    }
    while (isTokenSymbol() && jt.symbol() == ",")
    {
        writeSymbol(",");
        jt.nextToken();
        compileExpression();
    }
    
    endTag("expressionList");
}

/*
 Writes the current token which is a keyword with its corresponding XML tags.
 */
void CompilationEngine::writeKeyword(string token, int keywordType)
{
    string line;
    
    if (isTokenKeyword() && jt.keyword() == keywordType)
    {
        line = addIndentToLine();
        line += "<keyword> ";
        line += token;
        line += " </keyword>";
        
        XMLcode.push_back(line);
    }
    else
    {
        line += "Expected '";
        line += token;
        line += "' keyword, received '";
        line += jt.getToken();
        line += "'";
        
        writeError(line);
    }
}

/*
 Writes the current token which is an identifier with its corresponding XML
 tags.
 */
void CompilationEngine::writeIdentifier()
{
    string line;
    
    if (isTokenIdentifier())
    {
        line = addIndentToLine();
        line += "<identifier> ";
        line += jt.identifier();
        line += " </identifier>";
        
        XMLcode.push_back(line);
    }
    else
    {
        writeError("Expected identifier");
    }
}

/*
 Writes the current token which is a symbol with its corresponding XML tags.
 */
void CompilationEngine::writeSymbol(string token)
{
    string symbol;
    
    if (isTokenSymbol() && jt.symbol() == token)
    {
        if (token == "\"")
            token = "&quot;";
        else if (token == "&")
            token = "&amp;";
        else if (token == "<")
            token = "&lt;";
        else if (token == ">")
            token = "&gt;";
        
        string line = addIndentToLine();
        line += "<symbol> ";
        line += token;
        line += " </symbol>";

        XMLcode.push_back(line);
    }
    else
    {
        symbol = "Expected ";
        symbol += token;
        symbol += ", Received '";
        symbol += jt.getToken();
        symbol += "'";
        writeError(symbol);
    }
    
}

/*
 Writes the current token which is an integer constant with its corresponding
 XML tags.
 */
void CompilationEngine::writeIntVal()
{
    string line = addIndentToLine();
    line += "<integerConstant> ";
    line += jt.intVal();
    line += " </integerConstant>";
    
    XMLcode.push_back(line);
}

/*
 Writes the current token which is a string constant with its corresponding XML
 tags.
 */
void CompilationEngine::writeStringVal()
{
    string line = addIndentToLine();
    line += "<stringConstant> ";
    line += jt.stringVal();
    line += " </stringConstant>";
    
    XMLcode.push_back(line);
}

/*
 'int' | 'char' | 'boolean' | className
 */
void CompilationEngine::writeType()
{
    if (isTokenKeyword())
    {
        if (jt.keyword() == K_INT)
            writeKeyword("int", K_INT);
        else if (jt.keyword() == K_CHAR)
            writeKeyword("char", K_CHAR);
        else if (jt.keyword() == K_BOOLEAN)
            writeKeyword("boolean", K_BOOLEAN);
    }
    else if (isTokenIdentifier())
    {
        writeIdentifier();
    }
}

/*
 Writes variable declarations
 */
void CompilationEngine::writeVarDec()
{
    writeType();
    jt.nextToken();
    writeIdentifier();
    jt.nextToken();
    while (isTokenSymbol() && jt.symbol() == ",")
    {
        writeSymbol(",");
        jt.nextToken();
        writeIdentifier();
        jt.nextToken();
    }
    writeSymbol(";");
}

/*
 Writes a subroutine call
 */
void CompilationEngine::writeSubroutineCall()
{
    writeIdentifier();
    jt.nextToken();
    if (isTokenSymbol())
    {
        if (jt.symbol() == "(")
        {
            writeSymbol("(");
            jt.nextToken();
            compileExpressionList();
            writeSymbol(")");
        }
        else if (jt.symbol() == ".")
        {
            writeSymbol(".");
            jt.nextToken();
            writeIdentifier();
            jt.nextToken();
            writeSymbol("(");
            jt.nextToken();
            compileExpressionList();
            writeSymbol(")");
        }
        else
        {
            writeError("Expected '(' or '.' symbol");
        }
    }
}

/*
 Writes an expression enclosed in a symbol (i.e. '(' and ')')
 */
void CompilationEngine::writeEnclosedExpression(string open, string close)
{
    writeSymbol(open);
    jt.nextToken();
    compileExpression();
    writeSymbol(close);
}

/*
 Writes an expression enclosed in curly brackets
 */
void CompilationEngine::writeEnclosedStatements()
{
    writeSymbol("{");
    jt.nextToken();
    compileStatements();
    writeSymbol("}");
}

/*
 Writes an error message
 */
void CompilationEngine::writeError(string errorMessage)
{
    string line = addIndentToLine();
    line += "<error> ";
    line += errorMessage;
    line += " </error>";
    
    XMLcode.push_back(line);
}

void CompilationEngine::startTag(string token)
{
    string line = addIndentToLine();
    indentLevel++;
    
    line += "<";
    line += token;
    line += ">";
    
    XMLcode.push_back(line);
}

void CompilationEngine::endTag(string token)
{
    indentLevel--;
    string line = addIndentToLine();
    
    line += "</";
    line += token;
    line += ">";
    
    XMLcode.push_back(line);
}

string CompilationEngine::addIndentToLine()
{
    string indent;
    for (int i = 0; i < indentLevel; i++)
    {
        for (int j = 0; j < INDENT_SPACES; j++)
        {
            indent += " ";
        }
    }
    return indent;
}

bool CompilationEngine::isTokenKeyword()
{
    return (jt.tokenType() == T_KEYWORD);
}

bool CompilationEngine::isTokenSymbol()
{
    return (jt.tokenType() == T_SYMBOL);
}

bool CompilationEngine::isTokenIdentifier()
{
    return (jt.tokenType() == T_IDENTIFIER);
}

bool CompilationEngine::isTokenIntConst()
{
    return (jt.tokenType() == T_INT_CONST);
}

bool CompilationEngine::isTokenStringConst()
{
    return (jt.tokenType() == T_STRING_CONST);
}

bool CompilationEngine::isSubroutineCall()
{
    return (jt.getTokenInList(jt.getListIndex()) == "(" ||
            jt.getTokenInList(jt.getListIndex()) == ".");
}

bool CompilationEngine::isArrayReference()
{
    return (jt.getTokenInList(jt.getListIndex()) == "[");
}

bool CompilationEngine::isValidStatementKeyword()
{
    return (jt.keyword() == K_LET || jt.keyword() == K_IF ||
            jt.keyword() == K_WHILE || jt.keyword() == K_DO ||
            jt.keyword() == K_RETURN);
}

bool CompilationEngine::isValidVarDecKeyword()
{
    return (jt.keyword() == K_FIELD || jt.keyword() == K_STATIC);
}

bool CompilationEngine::isValidSubDecKeyword()
{
    return (jt.keyword() == K_CONSTRUCTOR || jt.keyword() == K_METHOD ||
            jt.keyword() == K_FUNCTION || jt.keyword() == K_INT ||
            jt.keyword() == K_CHAR || jt.keyword() == K_BOOLEAN ||
            isTokenIdentifier());
}

bool CompilationEngine::isValidOpSymbol()
{
    return (jt.symbol() == "+" || jt.symbol() == "-" || jt.symbol() == "*" ||
            jt.symbol() == "/" || jt.symbol() == "&" || jt.symbol() == "|" ||
            jt.symbol() == "<" || jt.symbol() == ">" || jt.symbol() == "=");
}