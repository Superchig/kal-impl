#ifndef LEXER_HPP_
#define LEXER_HPP_

#include <iostream>

namespace Kaleidoscope {

enum class Token {
	eof = -1,

	// commands
def = -2,
kal_extern = -3,

	// primary
identifier = -4,
number = -5,
};

//! Typesafe shorter form of Token::eof, for use as an int.
const int tok_eof = static_cast<int>(Token::eof);
//! Typesafe shorter form of Token::def, for use as an int.
const int tok_def = static_cast<int>(Token::def);
//! Typesafe shorter form of Token::kal_extern, for use as an int.
const int tok_extern = static_cast<int>(Token::kal_extern);
//! Typesafe shorter form of Token::indentifier, for use as an int.
const int tok_identifier = static_cast<int>(Token::identifier);
//! Typesafe shorter form of Token::number, for use as an int.
const int tok_number = static_cast<int>(Token::number);

extern std::string ident_str; // Filled in if Token::identifier
extern double num_val; // Filled in if Token::number

//! Return the next token from standard input.
int gettok();
inline bool comment_end(const char character);

}

#endif // LEXER_HPP_
