#include "token.h"

#define TOKEN_CASE(ch, type) case ch: \
	prev = prev->setNext(new type); \
	break

Token* Lex(QString str) {
	if (!str.length()) {
		throw 0;
	}
	QList<LBracketToken*> stack;
	static HeadToken head;
	Token* prev = &head;

	for (int i = 0; i < str.length(); ++i) {
		switch (str[i].unicode()) {
		case '(': {
			LBracketToken* token = new LBracketToken;
			prev = prev->setNext(token);
			stack.push_back(token);
			break;
		}
		case ')': {
			if (!stack.size()) {
				throw i;
			}
			RBracketToken* token = new RBracketToken;
			prev = prev->setNext(token);
			stack.back()->right = token;
			token->left = stack.back();
			stack.pop_back();
			break;
		}
		TOKEN_CASE('~', NotToken);
		TOKEN_CASE('&', CapToken);
		TOKEN_CASE('|', CupToken);
		TOKEN_CASE('^', ContainToken);
		default: {
			if (str[i].isLower()) {
				switch (str[i].unicode()) {
				TOKEN_CASE('T', TrueToken);
				TOKEN_CASE('F', FalseToken);
				default: {
					ValueToken* token = new ValueToken;
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
