#include <memory>

#include "lexer.hpp"
#include "parser.hpp"

namespace Kaleidoscope {

std::map<char, int> binop_precedence;
char cur_tok;

int get_next_token() {
	return cur_tok = gettok();
}

std::unique_ptr<ExprAST> error(const char* str) {
	std::cerr << "Error: " << str << '\n';
	return nullptr;
}

std::unique_ptr<PrototypeAST> errorp(const char* str) {
	error(str);
	return nullptr;
}

std::unique_ptr<ExprAST> parse_number_expr() {
	auto result = std::make_unique<NumberExprAST>(num_val);
	get_next_token(); // Consume the number
	return std::move(result);
}

std::unique_ptr<ExprAST> parse_paren_expr() {
	get_next_token(); // Eat (.

	auto v = parse_expression();
	if (!v) {
		return nullptr;
	}

	if (cur_tok != ')') {
		return error("expected ')'");
	}

	get_next_token(); // eat ).
	return v;
}

std::unique_ptr<ExprAST> parse_identifier_expr() {
	auto id_name = ident_str;

	get_next_token(); // eat identifier.

	if (cur_tok != '(') {
		return std::make_unique<VariableExprAST>(id_name);
	}

	// Call.
	get_next_token(); // eat (
	std::vector<std::unique_ptr<ExprAST>> args;
	if (cur_tok != ')') {
		while (1) {
			if (auto arg = parse_expression()) {
				args.push_back(std::move(arg));
			} else {
				return nullptr;
			}

			if (cur_tok == ')') {
				break;
			}

			if (cur_tok != ',') {
				return error("Expected ')' or ',' in argument list.");
			}
		}
	}

	// Eat the ')'.
	get_next_token();

	return std::make_unique<CallExprAST>(id_name, std::move(args));
}

std::unique_ptr<ExprAST> parse_primary() {
	switch (cur_tok) {
	case tok_identifier:
		return parse_identifier_expr();
	case static_cast<int>(Token::number):
		return parse_number_expr();
	case '(':
		return parse_paren_expr();
	default:
		return error("Unknown token when expecting an expression");
	}
}

int get_tok_precedence() {
	if (!isascii(cur_tok)) {
		return -1;
	}

	// Make sure it's a declared binop.
	int tok_prec = binop_precedence[cur_tok];
	if (tok_prec <= 0) return -1;
	return tok_prec;
}

std::unique_ptr<ExprAST> parse_expression() {
	auto lhs = parse_primary();
	if (!lhs) {
		return nullptr;
	}
	return parse_bin_op_rhs(0, std::move(lhs));
}

std::unique_ptr<ExprAST> parse_bin_op_rhs(const int expr_prec,
										  std::unique_ptr<ExprAST> lhs) {
	// If this is a binop, find its precedence.
	while (1) {
		int tok_prec = get_tok_precedence();

		// If this is a binop that binds at least as tightly as the current binop,
		// consume it, otherwise we are done.
		if (tok_prec < expr_prec) {
			return lhs;
		}

		// Ok we know this is a binop.
		char bin_op = cur_tok;
		get_next_token(); // eat binop.

		// Parse the primary expression after the binary operator.
		auto rhs = parse_primary();
		if (!rhs) {
			return nullptr;
		}

		// If bin_op binds less tightly with rhs than the operator after rhs, let
		// the pending operator take rhs as its lhs.
		int next_prec = get_tok_precedence();
		if (tok_prec < next_prec) {
			rhs = parse_bin_op_rhs(tok_prec+1, std::move(rhs));
			if (!rhs) {
				return nullptr;
			}
		}

		// Merge lhs/rhs.
		lhs = std::make_unique<BinaryExprAST>(bin_op, std::move(lhs),
											  std::move(rhs));
	}
}

std::unique_ptr<PrototypeAST> parse_prototype() {
	if (cur_tok != tok_identifier) {
		return errorp("Expected function name in prototype");
	}

	auto fn_name = ident_str;
	get_next_token();

	if (cur_tok != '(') {
		return errorp("Expected '(' in prototype");
	}

	// Read the list of argument names.
	std::vector<std::string> arg_names;
	while (get_next_token() == tok_identifier) {
		arg_names.push_back(ident_str);
	}
	if (cur_tok != ')') {
		return errorp("Expected ')' in prototype");
	}

	// Success.
	get_next_token(); // eat ')'

	return std::make_unique<PrototypeAST>(fn_name, std::move(arg_names));
}

std::unique_ptr<FunctionAST> parse_definition() {
	get_next_token(); // Eat def.
	auto proto = parse_prototype();
	if (!proto) {
		return nullptr;
	}

	if (auto e = parse_expression()) {
		return std::make_unique<FunctionAST>(std::move(proto), std::move(e));
	}
	return nullptr;
}

std::unique_ptr<PrototypeAST> parse_extern() {
	get_next_token(); // Eat extern.
	return parse_prototype();
}


std::unique_ptr<FunctionAST> parse_top_level_expr() {
	if (auto e = parse_expression()) {
		// Make an anonymous proto.
		auto proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());
		return std::make_unique<FunctionAST>(std::move(proto), std::move(e));
	}
	return nullptr;
}

}
