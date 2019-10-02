#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QList>
#include "token.h"

struct Expression : public Token {
    virtual ~Expression();
	virtual bool eval(unsigned ) const = 0;
};

struct ValueExpression : public Expression {
	unsigned id;
	ValueExpression(unsigned i) : id(i) {}
	virtual bool eval(unsigned bit) const {
		return (bit >> id) & 1;
	}
};

struct TrueExpression : public Expression {
	virtual bool eval(unsigned ) const {
		return true;
	}
};

struct FalseExpression : public Expression {
	virtual bool eval(unsigned ) const {
		return false;
	}
};

struct NotExpression : public Expression {
	Expression* sub;
	NotExpression(Expression* s) : sub(s) {}
	virtual bool eval(unsigned bit) const {
		return !sub->eval(bit);
	}
};

struct BinaryExpression : public Expression {
	Expression* left;
	Expression* right;
	BinaryExpression(Expression* l, Expression* r) : left(l), right(r) {}
};

#define DECLARE_BINARY_EXPRESSION(type, op) \
	struct type##Expression : public BinaryExpression { \
		using BinaryExpression::BinaryExpression; \
		virtual bool eval(unsigned bit) const { \
			return left->eval(bit) op right->eval(bit); \
		} \
	}

DECLARE_BINARY_EXPRESSION(Cap, &);
DECLARE_BINARY_EXPRESSION(Cup, |);
DECLARE_BINARY_EXPRESSION(Equal, ==);

struct ContainExpression : public BinaryExpression {
	using BinaryExpression::BinaryExpression;
	virtual bool eval(unsigned bit) const {
		return !left->eval(bit) | right->eval(bit);
	}
};

Expression* Parse(Token* tokens);

#endif // EXPRESSION_H
