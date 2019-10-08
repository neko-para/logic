#ifndef TOKEN_H
#define TOKEN_H

#include <QList>

struct Token {
	size_t posL, posR;
	Token* prev = nullptr;
	Token* next = nullptr;
	Token(size_t pos) : posL(pos), posR(pos) {}
	Token(size_t pl, size_t pr) : posL(pl), posR(pr) {}
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

#define DECLARE_TOKEN(type) \
	struct type : public Token { \
		using Token::Token; \
	}

#define DECLARE_TOKEN_DATA(type, dtype, dname) \
	struct type : public Token { \
		dtype dname; \
		type(size_t pos) : Token(pos) {} \
		type(size_t pos, dtype d) : Token(pos), dname(d) {} \
		type(size_t pl, size_t pr, dtype d) : Token(pl, pr), dname(d) {} \
	}

DECLARE_TOKEN(HeadToken);
DECLARE_TOKEN_DATA(ValueToken, unsigned, id);
DECLARE_TOKEN(TrueToken);
DECLARE_TOKEN(FalseToken);
DECLARE_TOKEN(CapToken);
DECLARE_TOKEN(CupToken);
DECLARE_TOKEN(NotToken);
DECLARE_TOKEN(ContainToken);
DECLARE_TOKEN(EqualToken);

struct LBracketToken;
struct RBracketToken;

DECLARE_TOKEN_DATA(LBracketToken, RBracketToken*, right);
DECLARE_TOKEN_DATA(RBracketToken, LBracketToken*, left);

Token* Lex(QString str);

#endif // TOKEN_H
