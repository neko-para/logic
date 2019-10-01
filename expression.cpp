#include "expression.h"

static Token* replace(Token* fromL, Token* fromR, Token* to) {
	fromL->prev->setNext(to);
	to->setNext(fromR->next);
	return to;
}

enum class CapCupToken {
	CAP,
	CUP,
	HEAD,
};

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
	for (auto t = head->next; t; t = t->next) {
		if (t->cast<ContainToken>()) {
			if (!t->next) {
				throw t;
			}
			Expression* from = t->prev->cast<Expression>();
			Expression* to = t->next->cast<Expression>();
			if (!from || !to) {
				throw t;
			}
			Token* e = replace(from, to, new ContainExpression(from, to));
			delete t;
			t = e;
		}
	}
	bool flag = true;
	auto t = head->next;
	for (; t; t = t->next) {
		if (flag && !t->test<Expression>()) {
			throw t;
		}
		if (!flag && !(t->test<CapToken>() || t->test<CupToken>())) {
			throw t;
		}
		flag = !flag;
	}
	if (flag) {
		throw t;
	}
	t = head->next;
	delete head;
	CapCupToken prev = CapCupToken::HEAD;
	Expression* result = t->cast<Expression>();
	while (t->next) {
		if (t->test<CapToken>()) {
			t = t->next;
			delete t->prev;
			Expression* nxt = t->cast<Expression>();
			if (prev == CapCupToken::CAP) {
				result->cast<CapExpression>()->sub.push_back(nxt);
			} else {
				prev = CapCupToken::CAP;
				CapExpression* e = new CapExpression;
				e->sub.push_back(result);
				e->sub.push_back(nxt);
				result = e;
			}
		} else {
			t = t->next;
			delete t->prev;
			Expression* nxt = t->cast<Expression>();
			if (prev == CapCupToken::CUP) {
				result->cast<CupExpression>()->sub.push_back(nxt);
			} else {
				prev = CapCupToken::CUP;
				CupExpression* e = new CupExpression;
				e->sub.push_back(result);
				e->sub.push_back(nxt);
				result = e;
			}
		}
		t = t->next;
	}
	return result;
}
