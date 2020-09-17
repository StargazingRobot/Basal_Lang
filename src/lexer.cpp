#include <string>
#include <iostream>
#include "lexer.h"

using std::string;

namespace lexer
{

    // return lower cased string
    string to_lower( string s )
    {
        string low = s;
        for( unsigned i=0; i<s.length(); i++ )
        {
            if( s[i] >= 'A' and s[i] <= 'Z' )
            {
                low[i] += 32;
            }
        }
        return low;
    }

    // return lower cased string
    string to_upper( string s )
    {
        string low = s;
        for( unsigned i=0; i<s.length(); i++ )
        {
            if( s[i] >= 'a' and s[i] <= 'z' )
            {
                low[i] -= 32;
            }
        }
        return low;
    }

    // return true if the string is a basal operator
    bool matchOP( string op )
    {
        return( op=="="  or op=="==" or op=="<"   or op==">" or op==">=" or op=="<=" or op=="!=" 
             or op=="+"  or op=="-" or op=="/"   or op=="*"   or op=="^"  or op=="%" );
    }

    // return true if the string is a basal keyword
    bool matchKEYWORD( string op )
    {
        return( op=="var" or op=="bin" or op=="array" or op=="tableau" or op=="FUNCTION" or op=="FONCTION" or op=="RETURN" 
             or op=="RENVOYER" or op=="END" or op=="FIN" or op=="IF" or op=="SI" or op=="THEN" or op=="ALORS"  or op=="ELSE" or op=="SINON"
             or op=="FOR" or op=="POUR" or op=="UNTIL" or op=="JUSQUA" or op=="DO" or op=="FAIRE" or op=="WHILE" or op == "TANTQUE" );
    }

    // return true if the string is a basal reserved function
    bool matchRESERVED( string op )
    {
        op = to_upper( op ); // non case sensitive op 
        return( op=="DISP" or op=="AFFICHER" );  
    }


    bool isSpace( const char& c )
    {
        return( c == ' ' or c == '\t' );
    }
    bool isNumber( const char& c )
    {
        return( c >= '0' and c <= '9' );
    }
    bool isAlphaNumerical( const char& c )
    {
        return(( c >= 'a' and c <= 'z') or ( c >= 'A' and c <= 'Z' ) or ( c >= '0' and c <= '9'));
    }
    bool isAlpha( const char& c )
    {
        return(( c >= 'a' and c <= 'z') or ( c >= 'A' and c <= 'Z' ));
    }
    bool isHexa( const char& c )
    {
        return( ( c >= '0' and c <= '9' ) or ( c >= 'A' and c <= 'F' ));
    }

    bool matchDecimalValue( const string& s )
    {
        unsigned off = 0; // offset to start the loop approprietly ( skip special char at begining ex : -123 )

        if( s[0]  == '-' ) off = 1;
        if( not (s.length() > off )) return false; // must have atleast one number
        for( unsigned i=off; i < s.length(); i++ )
        {
            if( not isNumber( s[i] )) return false; // must have atleast one number
        }
        return true;
    }

    bool matchHexaValue( const string& s )
    {
        unsigned off = 0;

        if( s[0]  == '0' and s[1] == 'x') off = 2;
        else return false;
        if( not (s.length() > off )) return false; // must have atleast one number
        for( unsigned i=off; i < s.length(); i++ )
        {
            if( not isHexa( s[i] )) return false; // must have atleast one number
        }
        return true;
    }

    bool matchBinValue( const string& s )
    {
        unsigned off = 0;

        if( s[0]  == '0' and s[1] == 'b') off = 2;
        else return false;
        if( not (s.length() > off )) return false; // must have atleast one number
        for( unsigned i=off; i < s.length(); i++ )
        {
            if(( s[i] != '0') and ( s[i] != '1')) return false; // must have atleast one number
        }
        return true;
    }

    bool matchCharValue( const string& s )
    {
        if( s[0] != '\'' )
            return false;
        else
        {
            if( s.length() == 3 )
            {
                if( s[2] == '\'' ) 
                    return true;
                return false;
            }
            else if( s.length() == 4 )
            {
                if( s[3] == '\'' )
                    if( s[1] == '\\' )
                        return true;
                return false;
            }
            else
                return false;
        }
    }

    bool matchIdentifier( const string& s )
    {

        if( not isAlpha( s[0] ) ) return false; // must start with [a-Z] 
        for( unsigned i=0; i < s.length(); i++ )
        {
            if( not (isAlphaNumerical( s[i] ) or s[i] == '_') ) return false; // must have atleast one number
        }
        return true;
    }



    // can't have nested functions in C++ :( , this is a work-around
    void endWord( vector<string>& words, string& word ) // push word if not empty
    {
        if( word == "" ) return;
        words.push_back( word );
        word = "";  // empty variable for the next word
    }

    bool isEscaped( const string& line, const uint32_t& i )
    {
        return( i > 0 and line[i-1] == '\\');
    }

    // split a string into words, stored in a vector
    vector<string> splitLine( string line )
    {
        vector<string> words;
        string word = "";

        bool Esc = false; // true if the previous char was '\'
        bool quotes = false; // wether or not the current char is part of a quotation ex: DISP("HelloWorld!")
                             //                                                                       ^--------- quotes would be true here 
        for( uint32_t i=0; i < line.length(); i++) // every char of the string
        {
            char c = line[i];

            if( isSpace( line[i]) and quotes == false )
            {    
                endWord( words, word );
                while( isSpace( line[i+1]) ) 
                {
                    i++;    
                }
                continue;
            }
            else if( c==',' or c=='&' or c=='(' or c==')' or c=='[' or c==']' or c=='{' or c=='}' )
            {
                if( quotes ) word += line[i];
                else
                {
                    endWord( words, word );
                    word += line[i];
                    endWord( words, word );
                }
                continue;
            }
            else if( line[i] == '\"' )
            {
                if( not Esc )
                {
                    endWord( words, word ); 
                    quotes = not quotes;
                    word += line[i];
                    endWord( words, word );
                }
                else word += line[i];
                continue;
            }
            else if( line[i] == '#') // discard the rest of the line if the char is not escaped
            {
                if( Esc ) word += line[i]; // continue normally
                else 
                { 
                    endWord( words, word );
                    break;      // stop processing the line
                }
                continue;
            }
            if( line[i] == '\\' ) 
            {
                if( Esc == false ) Esc = true;       // chain escapement ex : DISP("\\\\")
                else if( Esc == true )
                {
                    word += line[i];
                    Esc = false;
                } 
            }
            else Esc = false;
            // default
            word += line[i];
        }
        endWord( words, word);
        word += "#"; // end of line token, use '#' char, so ';' can be used as a separator, not incrementing line count
        endWord( words, word);
        
        return words;
    }

    token tokenizeOneWord( string txt )
    {
        using namespace basal;
        TokenType type = UNKNOWN;
        if     ( txt == "," ) type = COMMA;
        else if( txt == ";" or txt == "#" ) type = ENDL;
        else if( txt == "&" ) type = AMPERSAND;
        else if( txt == "(" ) type = LPAREN;
        else if( txt == ")" ) type = RPAREN;
        else if( txt == "[" ) type = LBRACKET;
        else if( txt == "]" ) type = RBRACKET;
        else if( txt == "{" ) type = LBRACES;
        else if( txt == "}" ) type = RBRACES;
        else if( txt == "var" or txt == "bin" or txt == "array" or txt == "tableau" ) type = KEYWORD;         // try to display types 
        else if( matchOP( txt ))            type = OP;              // try to match op
        else if( matchRESERVED( txt ))      type = RESERVED;        // try to match reserved functions
        else if( matchKEYWORD( txt ))       type = KEYWORD;         // try to match basal keywords
        else if( matchDecimalValue( txt ))  type = DECIMAL_VALUE;   // try to match decimal values
        else if( matchHexaValue( txt ))     type = HEXA_VALUE;      // try to match hexa values
        else if( matchBinValue( txt ))      type = BINARY_VALUE;    // try to match binary values
        else if( matchIdentifier( txt))     type = IDENTIFIER;      // try to match label call ex: jump Hello_World_Proc

        token ret( txt, type );
        return( ret ); 
    }
}