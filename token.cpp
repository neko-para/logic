#include "token.h"

Token::~Token() {}

/*
 * 通用二元运算case
 * ch	运算符
 * type	token
 * */
#define TOKEN_CASE(ch, type, pos) case ch: \
	prev = prev->setNext(new type(pos)); \
	break

// 词法解析
// throw int 异常为错误的index
Token* Lex(QString str) {
	if (!str.length()) {
		throw 0;
	}
	QList<LBracketToken*> stack;
	static HeadToken head(0);
	Token* prev = &head;

	for (int i = 1; i <= str.length(); ++i) {
		switch (str[i - 1].unicode()) {
		case '(': {
			LBracketToken* token = new LBracketToken(i);
			prev = prev->setNext(token);
			stack.push_back(token);
			break;
		}
		case ')': {
			if (!stack.size()) {
				throw i;
			}
			RBracketToken* token = new RBracketToken(i, stack.back());
			prev = prev->setNext(token);
			stack.back()->right = token;
			stack.pop_back();
			break;
		}
		TOKEN_CASE('!', NotToken, i);
		TOKEN_CASE('&', CapToken, i);
		TOKEN_CASE('|', CupToken, i);
		TOKEN_CASE('^', ContainToken, i);
		TOKEN_CASE('~', EqualToken, i);
		default: {
			if (str[i].isLower()) {
				switch (str[i].unicode()) {
				TOKEN_CASE('T', TrueToken, i);
				TOKEN_CASE('F', FalseToken, i);
				default: {
					ValueToken* token = new ValueToken(i);
					token->id = str[i].unicode() - 'A';
					prev = prev->setNext(token);
				}
				}
			} else {
				throw i;
			}
		}
		}
	}
	if (stack.size()) {
		throw str.length();
	}
	return &head;
}
