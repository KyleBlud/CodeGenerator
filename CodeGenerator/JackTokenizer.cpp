/*
 JackTokenizer.cpp
 SyntaxAnalyzer
 
 Project 10: Compiler I, Syntax Analysis
 
 The JackTokenizer and CompilationEngine modules act as a Syntax Analyzer that 
 parses Jack programs according to the Jack grammar, producing an XML file
 that captures the program's structure.

 Created by Kyle Bludworth & Josh Burks on 4/3/16.
 Copyright © 2016 Kyle Bludworth. All rights reserved.
*/

#include "JackTokenizer.hpp"

/*
 Sets the current position in the tokenList vector to 0 and define the preset
 keywords and symbols.
 */
JackTokenizer::JackTokenizer()
{
    listIndex = 0;
    defineKeywords();
    defineSymbols();
}

/*
 Returns the size of the tokenList vector.
 */
int JackTokenizer::getTokenListSize()
{
    return (int) tokenList.size();
}

/*
 Returns a token based on its position in the list.
 */
string JackTokenizer::getTokenInList(int index)
{
    return tokenList.at(index);
}

/*
 Sets the current token to the next token in the list.
 */
void JackTokenizer::nextToken()
{
    token = tokenList.at(listIndex++);
}

/*
 Returns the current token.
 */
string JackTokenizer::getToken()
{
    return token;
}

/*
 Sets the current position in the tokenList to a different value.
 */
void JackTokenizer::setListIndex(int index)
{
    if (index < 0)
        return;
    
    listIndex = index;
}

/*
 Returns the current position in the tokenList vector.
 */
int JackTokenizer::getListIndex()
{
    return listIndex;
}

/*
 Returns the type of the current token.
 */
int JackTokenizer::tokenType()
{
    if (isKeyword())
        return T_KEYWORD;
    if (isSymbol())
        return T_SYMBOL;
    if (isIntVal())
        return T_INT_CONST;
    if (isStringVal())
        return T_STRING_CONST;
    
    return T_IDENTIFIER;
}

/*
 Returns the keyword value which is it's enum value in KeywordType.
 */
int JackTokenizer::keyword()
{
    for (int i = 0; i < KW_SIZE; i++)
    {
        if (token == keywordList[i])
        {
            return (i + 1);
        }
    }
        
    return 0;
}

/*
 Determines if the current token is a keyword.
 */
bool JackTokenizer::isKeyword()
{
    for (int i = 0; i < KW_SIZE; i++)
    {
        if (token == keywordList[i])
            return true;
    }
    return false;
}

/*
 Returns the symbol which is the current token. Should be called only when 
 tokenType() is T_SYMBOL
*/
string JackTokenizer::symbol()
{
    return token;
}

/*
 Returns the identifier which is the current token. Should be called only when 
 tokenType() is T_IDENTIFIER
*/
string JackTokenizer::identifier()
{
    return token;
}

/*
 Returns the integer value of the current token as a string. Should be called
 only when tokenType() is T_INT_CONST
 */
string JackTokenizer::intVal()
{
    return token;
}

/*
 Determines if the current token is an integer constant value by checking if 
 each character in the token is a digit.
*/
bool JackTokenizer::isIntVal()
{
    for (int i = 0; i < token.length(); i++)
    {
        if (!isdigit(token[i]))
            return false;
    }
    return true;
}

/*
 Returns the string value of the current token, without the double quotes.
 Should be called only when tokenType() is T_STRING_CONST
 */
string JackTokenizer::stringVal()
{
    token = token.erase(token.front());
    return token;
}

/*
 Determines if the current token is a string constant value. A \" is left in the
 string in order to differentiate the string constant value with an identifier.
 */
bool JackTokenizer::isStringVal()
{
    if (token.find("\"") != std::string::npos)
    {
        return true;
    }
    return false;
}

/*
 Removes all comments and white space from the input steam and breaks it into
 Jack-language tokens, as specified by the Jack grammar. The tokens are stored
 in the tokenList vector.
 */
void JackTokenizer::tokenize(string line)
{
    string currToken;
    
    if (line.find("//") != string::npos)
    {
        line.erase(line.find("//"), line.length());
    }
    
    for (int i = 0; i < line.length(); i++)
    {
        if (isSymbol(line[i]))
        {
            if (!currToken.empty())
            {
                tokenList.push_back(currToken);
                currToken.clear();
            }
            currToken += line[i];
            tokenList.push_back(currToken);
            currToken.clear();
        }
        else if (line[i] == '\"')
        {
            currToken += line[i];
            i++;
            while (line[i] != '\"')
            {
                currToken += line[i];
                i++;
            }
            tokenList.push_back(currToken);
            currToken.clear();
        }
        else if (isalpha(line[i]) || isdigit(line[i]) || line[i] == '_')
        {
            currToken += line[i];
        }
        else if (!currToken.empty())
        {
            tokenList.push_back(currToken);
            currToken.clear();
        }
    }
}

/*
 Determines if the current character being analyzed is a symbol. The
 character is converted into a string in order to compare it to the symbol list
 comiled at the start of the program.
 */
bool JackTokenizer::isSymbol(char token)
{
    string s;
    s.push_back(token);
    
    for (int i = 0; i < SYM_SIZE; i++)
    {
        if (s == symbolList[i])
            return true;
    }
    return false;
}

/*
 Determines if the current token is a symbol. The token is compared to the 
 symbol list compiled at the start of the program.
 */
bool JackTokenizer::isSymbol()
{
    for (int i = 0; i < SYM_SIZE; i++)
    {
        if (token == symbolList[i])
            return true;
    }
    return false;
}

/*
 Determines whether the current line starts or ends a block comment. If the 
 current line starts a block comment, then the program will not analyze another
 line until the end is reached.
 */
bool JackTokenizer::isBlockComment(string line)
{
    if (line.find("*/") != std::string::npos) // If "*/" is found
    {
        inComment = false;
        return true;
    }
    
    if (line.find("/**") != std::string::npos) // If "/**" is found
    {
        inComment = true;
    }
    
    return inComment;
}

/*
 Determines whether the current line being analyzed is an inline comment.
 If the line is an inline comment, the program refuses to analyze the line.
 */
bool JackTokenizer::isInLineComment(string line)
{
    bool isComment = false;
    
    for(int i = 0; i < line.length(); i++)
    {
        if (!isspace(line[i]))
        {
            if (line[i] == '/')
            {
                isComment = true;
                break;
            }
            else
            {
                isComment = false;
                break;
            }
        }
    }
    
    return isComment;
}

/*
 Defines the keyword terminal elements.
 */
void JackTokenizer::defineKeywords()
{
    keywordList[0] = "class";
    keywordList[1] = "method";
    keywordList[2] = "function";
    keywordList[3] = "constructor";
    keywordList[4] = "int";
    keywordList[5] = "boolean";
    keywordList[6] = "char";
    keywordList[7] = "void";
    keywordList[8] = "var";
    keywordList[9] = "static";
    keywordList[10] = "field";
    keywordList[11] = "let";
    keywordList[12] = "do";
    keywordList[13] = "if";
    keywordList[14] = "else";
    keywordList[15] = "while";
    keywordList[16] = "return";
    keywordList[17] = "true";
    keywordList[18] = "false";
    keywordList[19] = "null";
    keywordList[20] = "this";
}

/*
 Defines the symbol terminal elements.
 */
void JackTokenizer::defineSymbols()
{
    symbolList[0] = "{";
    symbolList[1] = "}";
    symbolList[2] = "(";
    symbolList[3] = ")";
    symbolList[4] = "[";
    symbolList[5] = "]";
    symbolList[6] = ".";
    symbolList[7] = ",";
    symbolList[8] = ";";
    symbolList[9] = "+";
    symbolList[10] = "-";
    symbolList[11] = "*";
    symbolList[12] = "/";
    symbolList[13] = "&";
    symbolList[14] = "|";
    symbolList[15] = "<";
    symbolList[16] = ">";
    symbolList[17] = "=";
    symbolList[18] = "~";
}