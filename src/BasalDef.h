#pragma once

#include <string>
#include <iostream>

using std::string;


namespace basal
{

    enum TokenType   // types of tokens
    {
        IDENTIFIER,         // identifier
        ADDOP,              // additive operator
        MULOP,              // multiplicative operator 
        RELOP,              // relationnal operator
        EXPOP,              // exponent operator
        NOT,
        KEYWORD,            // keyword
        RESERVED_FUNC,           // reserved functions
        QUOTES,
        COMMA,
        AMPERSAND,
        DOT,
        COLON,
        LPAREN,
        RPAREN,
        LBRACKET,
        RBRACKET,
        LBRACES,
        RBRACES,
        DECIMAL_VALUE,      //  |
        FLOAT_VALUE,        //  |
        HEXA_VALUE,         //  |> values
        BINARY_VALUE,       //  |
        RESERVED_VALUE,     //  |
        TYPE,
        STRING,
        SPACES,
        ENDL,
        STOP,
        EQU,
        UNKNOWN
    };
    enum Type   // basal types // TODO add more types ?
    {
        INT,
        BIN,
        FLOAT,
        UNDECLARED,
        TYPE_ERROR
    };


    string getStringFromTokenType( TokenType type );

    // return the corresponding basal Type
    Type getTypeFromString( string s );

}

namespace lexer
{
    
    class token // contains the type and the text 
    {
    public:
        string text;
        basal::TokenType type = basal::UNKNOWN;
        token( const string& s, basal::TokenType t )
        : text( s )
        , type( t ) { }
        token()
        {
        }
    };
}
