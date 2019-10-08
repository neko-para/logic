#include "expression.h"

// 将fromL到fromR替换为to
static Token* replace(Token* fromL, Token* fromR, Token* to) {
	fromL->prev->setNext(to);
	to->setNext(fromR->next);
	return to;
}

// 检查从head->next到末尾是否是exp tok exp ... exp tok exp
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

// 合并二元运算，将TokenType转换为对应的ExpressionType
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

// 语法解析
// throw token 异常为错误的token
Expression* Parse(Token* tokens) {
	HeadToken* head = tokens->cast<HeadToken>();
	for (auto t = head->next; t; t = t->next) {
		if (t->cast<LBracketToken>()) { // 递归解析括号内容
			LBracketToken* lb = t->cast<LBracketToken>();
			HeadToken ht(0);
			ht.setNext(lb->next);
			lb->right->prev->next = nullptr;
			t = replace(lb, lb->right, Parse(&ht));
			delete lb->right;
			delete lb;
		} else if (t->cast<ValueToken>()) { // 转化命题token为对应expression
			ValueToken* vt = t->cast<ValueToken>();
			Token* e = replace(t, t, new ValueExpression(vt->posL, vt->id));
			delete t;
			t = e;
		} else if (t->cast<TrueToken>()) { // 转化T
			Token* e = replace(t, t, new TrueToken(t->posL));
			delete t;
			t = e;
		} else if (t->cast<FalseToken>()) { // 转化F
			Token* e = replace(t, t, new FalseToken(t->posL));
			delete t;
			t = e;
		}
	}
	for (auto t = head->next; t; t = t->next) { // 单独解析一元运算not
		if (t->cast<NotToken>()) {
			if (!t->next) {
				throw t;
			}
			Expression* s = t->next->cast<Expression>();
			if (!s) {
				throw t;
			}
			Token* e = replace(t, s, new NotExpression(t, s));
			delete t;
			t = e;
		}
	}
	checkIsMix(head);
	ParseToken<CapToken, CapExpression>(head); // &
	ParseToken<CupToken, CupExpression>(head); // |
	ParseToken<ContainToken, ContainExpression>(head); // ->
	ParseToken<EqualToken, EqualExpression>(head); // <->
	return head->next->cast<Expression>();
}

Expression::~Expression() {}
