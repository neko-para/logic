#include "expression.h"

static Token* replace(Token* fromL, Token* fromR, Token* to) {
	fromL->prev->setNext(to);
	to->setNext(fromR->next);
	return to;
}

static void checkIsMix(Token* head) {
	bool flag = true;
	auto t = head->next;
	for (; t; t = t->next) {
		if (flag ^ t->test<Expression>()) {
			throw t;
		}
		flag = !flag;
	}
	if (flag) {
		throw t;
	}
}

template <typename TokenType, typename ExpressionType>
void ParseToken(Token* head) {
	for (auto t = head->next; t; t = t->next) {
		if (t->test<TokenType>()) {
			ExpressionType* e = new ExpressionType(t->prev->cast<ExpressionType>(), t->next->cast<ExpressionType>());
			replace(t->prev, t->next, e);
			delete t;
			t = e;
		}
	}
}

Expression* Parse(Token* tokens) {
	HeadToken* head = tokens->cast<HeadToken>();
	for (auto t = head->next; t; t = t->next) {
		if (t->cast<LBracketToken>()) {
			LBracketToken* lb = t->cast<LBracketToken>();
			HeadToken ht;
			ht.setNext(lb->next);
			lb->right->prev->next = nullptr;
			t = replace(lb, lb->right, Parse(&ht));
			delete lb->right;
			delete lb;
		} else if (t->cast<ValueToken>()) {
			ValueToken* vt = t->cast<ValueToken>();
			Token* e = replace(t, t, new ValueExpression(vt->id));
			delete t;
			t = e;
		} else if (t->cast<TrueToken>()) {
			Token* e = replace(t, t, new TrueToken);
			delete t;
			t = e;
		} else if (t->cast<FalseToken>()) {
			Token* e = replace(t, t, new FalseToken);
			delete t;
			t = e;
		}
	}
	for (auto t = head->next; t; t = t->next) {
		if (t->cast<NotToken>()) {
			if (!t->next) {
				throw t;
			}
			Expression* s = t->next->cast<Expression>();
			if (!s) {
				throw t;
			}
			Token* e = replace(t, s, new NotExpression(s));
			delete t;
			t = e;
		}
	}
	checkIsMix(head);
	ParseToken<CapToken, CapExpression>(head);
	ParseToken<CupToken, CupExpression>(head);
	ParseToken<ContainToken, ContainExpression>(head);
	ParseToken<EqualToken, EqualExpression>(head);
	return head->next->cast<Expression>();
}
