#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QList>
#include "token.h"

struct Expression : public Token {
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

struct CapExpression : public Expression {
	QList<Expression*> sub;
	virtual bool eval(unsigned bit) const {
		bool val = true;
		for (auto exp : sub) {
			val &= exp->eval(bit);
			if (!val) {
				break;
			}
		}
		return val;
	}
};

struct CupExpression : public Expression {
	QList<Expression*> sub;
	virtual bool eval(unsigned bit) const {
		bool val = false;
		for (auto exp : sub) {
			val |= exp->eval(bit);
			if (val) {
				break;
			}
		}
		return val;
	}
};

struct ContainExpression : public Expression {
	Expression* from;
	Expression* to;
	ContainExpression(Expression* f, Expression* t) : from(f), to(t) {}
	virtual bool eval(unsigned bit) const {
		return !from->eval(bit) | to->eval(bit);
	}
};
/*
struct EqualExpression : public Expression {
	Expression* a;
	Expression* b;
	virtual bool eval(unsigned bit) const {
		return !(a->eval(bit) ^ b->eval(bit));
	}
};
*/
Expression* Parse(Token* tokens);

#endif // EXPRESSION_H
