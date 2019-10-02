#ifndef TOKEN_H
#define TOKEN_H

#include <QList>

struct Token {
	Token* prev = nullptr;
	Token* next = nullptr;
    virtual ~Token();
	Token* setNext(Token* t) {
		next = t;
		if (t) {
			t->prev = this;
		}
		return t;
	}
	template <typename T>
	T* cast() {
		return dynamic_cast<T*>(this);
	}
	template <typename T>
	const T* cast() const {
		return dynamic_cast<const T*>(this);
	}
	template <typename T>
	bool test() const {
		return cast<T>();
	}
};

struct HeadToken : public Token {};

struct ValueToken : public Token {
	unsigned id;
};

struct TrueToken : public Token {};

struct FalseToken : public Token {};

struct LBracketToken;
struct RBracketToken;

struct LBracketToken : public Token {
	RBracketToken* right;
};

struct RBracketToken : public Token {
	LBracketToken* left;
};

struct CapToken : public Token {};

struct CupToken : public Token {};

struct NotToken : public Token {};

struct ContainToken : public Token {};

struct EqualToken : public Token {};

Token* Lex(QString str);

#endif // TOKEN_H
