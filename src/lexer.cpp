#include <iostream>
#include "lexer.hpp"

namespace Kaleidoscope {

std::string ident_str;
double num_val;

int gettok() {
	static int last_char = ' ';

	// Skip any whitespace
	while (isspace(last_char)) {
		last_char = getchar();
	}

	if (isalpha(last_char)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
	  ident_str = last_char;

		while (isalnum((last_char = getchar()))) {
			ident_str += last_char;
		}

		if (ident_str == "def") {
			return static_cast<int>(Token::def);
		} else if (ident_str == "extern") {
			return static_cast<int>(Token::kal_extern);
		}

		return static_cast<int>(Token::identifier);
	}

	if (isdigit(last_char) || last_char == '.') { // Number: [0-9.]+
		auto num_str = std::string();

		do {
			num_str += last_char;
		} while (isdigit(last_char) || last_char == '.');

		num_val = strtod(num_str.c_str(), 0);
		return static_cast<int>(Token::number);
	}

	if (last_char == '#') {
		// Comment until end of line
		do {
			last_char = getchar();
		} while (comment_end(last_char));

		if (last_char != EOF) {
			return gettok();
		}
	}

	// Check for end of file. Don't eat the EOF.
	if (last_char == EOF) {
		return static_cast<int>(Token::eof);
	}

	int this_char = last_char;
	last_char = getchar();
	return this_char;
}

inline bool comment_end(const char character) {
	return character != EOF && character != '\n' && character != '\r';
}

}
