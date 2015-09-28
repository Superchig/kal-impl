#ifndef AST_HPP_
#define AST_HPP_

#include <iostream>
#include <vector>
#include <memory>

namespace Kaleidoscope {

//! Base class for all expression nodes.
class ExprAST {
public:
	virtual ~ExprAST() {}
};

//! Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
	double val;

public:
	NumberExprAST(double val) : val(val) {}
};

//! Expression class for referencing a variable, like "foo"
class VariableExprAST : public ExprAST {
	std::string name;

public:
	VariableExprAST(const std::string &name) : name(name) {}
};

//! Expression class for binary operators.
class BinaryExprAST : public ExprAST {
	char op;
	std::unique_ptr<ExprAST> lhs, rhs;

public:
	BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs,
				  std::unique_ptr<ExprAST> rhs)
		: op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
};

//! Expression class for function calls.
class CallExprAST : public ExprAST {
	using ExprASTPtr = std::unique_ptr<ExprAST>;
	using ExprVector = std::vector<ExprASTPtr>;
	std::string callee;
	ExprVector args;

public:
	CallExprAST(const std::string &callee,
				ExprVector args)
		: callee(callee), args(std::move(args)) {}
};

class PrototypeAST {
	std::string name;
	std::vector<std::string> args;

public:
	PrototypeAST(const std::string &name, std::vector<std::string> args)
		: name(name), args(std::move(args)) {}
};

class FunctionAST {
	std::unique_ptr<PrototypeAST> proto;
	std::unique_ptr<ExprAST> body;

public:
	FunctionAST(std::unique_ptr<PrototypeAST> proto,
				std::unique_ptr<ExprAST> body)
		: proto(std::move(proto)), body(std::move(body)) {}
};

}

#endif // AST_HPP_
