//! Main parser repl-style prompt.

#include <iostream>

#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"

static void handle_definition();
static void handle_extern();
static void handle_top_level_expr();

int main(const int, const char**) {
	using Kaleidoscope::Token;
	using Kaleidoscope::get_next_token;
	using Kaleidoscope::binop_precedence;

	// Install standard binary operators.
	// 1 is the lowest precedence.
	binop_precedence['<'] = 10;
	binop_precedence['>'] = 10;
	binop_precedence['+'] = 20;
	binop_precedence['-'] = 20;
	binop_precedence['*'] = 40;
	binop_precedence['/'] = 40; // highest

	// Prime the first token.
	std::cerr << "ready> ";
	get_next_token();

	while (1) {
		std::cerr << "ready> ";
		std::cout << "cur_tok: " << Kaleidoscope::cur_tok << '\n';

		getchar();

		switch (Kaleidoscope::cur_tok) {
		case Kaleidoscope::tok_eof:
			return 0;
		case ';': // Ignore top-level semicolons.
			get_next_token();
			break;
		case static_cast<int>(Kaleidoscope::Token::def):
			handle_definition();
			break;
		case static_cast<int>(Kaleidoscope::Token::kal_extern):
			handle_extern();
			break;
		default:
			handle_top_level_expr();
			break;
		}
	}
}

static void handle_definition() {
	if (Kaleidoscope::parse_definition()) {
		std::cerr << "Parsed a function definition." << '\n';
		return;
	}

	// Skip token for error recovery.
	Kaleidoscope::get_next_token();
}

static void handle_extern() {
	if (Kaleidoscope::parse_extern()) {
		std::cerr << "Parsed an extern." << '\n';
		return;
	}

	// Skip token for error recovery.
	Kaleidoscope::get_next_token();
}

static void handle_top_level_expr() {
	// Evaluate a top-level expression into an anonymous function.
	if (Kaleidoscope::parse_top_level_expr()) {
		std::cerr << "Parsed a top-level expression." << '\n';
		return;
	}

	// Skip token for error recovery.
	Kaleidoscope::get_next_token();
}
