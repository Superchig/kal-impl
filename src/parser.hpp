#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <memory>
#include <map>
#include "ast.hpp"

namespace Kaleidoscope {

//! Current token the parser is looking at.
extern char cur_tok;
//! Reads a token from the lexer and updates cur_tok with its results.
int get_next_token();

//! Little helper function for error handling.
std::unique_ptr<ExprAST> error(const char *str);

std::unique_ptr<PrototypeAST> errorp(const char *str);

//! numberexpr ::= number
std::unique_ptr<ExprAST> parse_number_expr();

//! parenexpr ::= '(' expression ')'
std::unique_ptr<ExprAST> parse_paren_expr();

/**
  identifierexpr
    ::= identifier
	::= identifier '(' expression* ')'
 */
std::unique_ptr<ExprAST> parse_identifier_expr();

/**
  primary
    ::= identifierexpr
	::= numberexpr
	::= parenexpr
 */
std::unique_ptr<ExprAST> parse_primary();

/**
  expression
    ::= primary binoprhs
 */
std::unique_ptr<ExprAST> parse_expression();

/**
  binoprhs
    ::= ('+' primary)*
 */
std::unique_ptr<ExprAST> parse_bin_op_rhs(int expr_prec,
										  std::unique_ptr<ExprAST> lhs);

//! Holds the precedence for each binary operator that is defined.
extern std::map<char, int> binop_precedence;

//! Get the precedence of the pending binary operator token.
int get_tok_precedence();

/*
  prototype
    ::= id '(' id* ')'
 */
std::unique_ptr<PrototypeAST> parse_prototype();

//! definition ::= 'def' prototype expression
std::unique_ptr<FunctionAST> parse_definition();

//! external ::= 'extern' prototype
std::unique_ptr<PrototypeAST> parse_extern();

//! toplevelexpr ::= expression
std::unique_ptr<FunctionAST> parse_top_level_expr();

}

#endif // PARSER_HPP_
