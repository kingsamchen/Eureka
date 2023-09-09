/***************************************

** Project:Expression Tree

** File:exptree.cpp

** Edition:v1.0.0 Beta

** Coder:KingsamChen [MDSA Group]

** Last Modify:2011-8-6

****************************************/

#include "stdafx.h"
#include <vector>
#include <cassert>
#include "exptree.h"

#define pop(x)	x.back();\
				x.pop_back();


node* ConstructExpTree(const std::wstring& postfixExp)
{
	std::vector<node*> nodeStack;

	for (std::wstring::const_iterator it = postfixExp.begin();
		 it != postfixExp.end(); ++it)
	{
		// operands
		if (L'0' <= *it && *it <= L'9')
		{
			node* pNode = CreateTree(*it - L'0', NULL, NULL);
			nodeStack.push_back(pNode);
		}
		else
		{
			// forms a new tree
			node* pRight = pop(nodeStack);
			node* pLeft = pop(nodeStack);
			node* pNewTree = CreateTree(*it, pLeft, pRight);

			nodeStack.push_back(pNewTree);
		}
	}

	node* pRoot = pop(nodeStack);
	return pRoot;
}


int Eval(const node* pRoot)
{
	if (NULL == pRoot->pLeftChild && NULL == pRoot->pRightChild)
	{
		return pRoot->data;
	}

	// should be refactored if wanna support more operations
	// cos the Calculate will not only evaluate binary operators
	// but also unary or ternary operators
	int num1 = Eval(pRoot->pLeftChild);
	int num2 = Eval(pRoot->pRightChild);

	return Calculate(num1, num2, pRoot->data);
}


// only for dump
void Visit(const node* pRoot)
{
	if (NULL == pRoot)
	{
		return;
	}

	Visit(pRoot->pLeftChild);
	// will invalid if number is euqal to or great than 10
	if (pRoot->data < 10)
	{
		wprintf(L"%d", pRoot->data);
	}
	else
	{
		wprintf(L"%c", pRoot->data);
	}
	Visit(pRoot->pRightChild);
}


node* CreateTree(int data, const node* pLeftChild, const node* pRightChild)
{
	node* p = new node;
	assert(p != NULL);

	p->data = data;
	p->pLeftChild = const_cast<node*>(pLeftChild);
	p->pRightChild = const_cast<node*>(pRightChild);

	return p;
}


void DestroyExpTree(node*& pRoot)
{
	if (NULL == pRoot)
	{
		return;
	}

	node* pLeft = pRoot->pLeftChild;
	node* pRight = pRoot->pRightChild;

	delete pRoot;
	pRoot = NULL;

	DestroyExpTree(pLeft);
	DestroyExpTree(pRight);
}


std::wstring InfixToPostfix(const wchar_t* pszInfixExp)
{
	std::wstring postfixExp;
	std::vector<wchar_t> stackOp;

	for (const wchar_t* p = pszInfixExp; *p != NULL; ++p)
	{
		if (L'0' <= *p && *p <= L'9')
		{
			postfixExp.push_back(*p);
		}
		else	// operators
		{
			if (L'(' == *p)
			{
				stackOp.push_back(*p);
			}
			else if (L')' == *p)
			{
				do
				{
					wchar_t op = pop(stackOp);

					// parentheses own the most higher priority in RPN
					// right parenthesis must match the left one
					if (L'(' == op)
					{
						break;
					}
					else
					{
						postfixExp.push_back(op);
					}
				} while (true);
			}
			else	// operators except ( & )
			{
				do
				{
					if (stackOp.empty() || L'(' == stackOp.back() || GetPriority(*p) > GetPriority(stackOp.back()))
					{
						stackOp.push_back(*p);
						break;
					}
					else
					{
						wchar_t op = pop(stackOp);
						postfixExp.push_back(op);
					}
				} while (true);
			}
		}
	}

	// output remaining operators in stack
	while (!stackOp.empty())
	{
		wchar_t op = pop(stackOp);
		postfixExp.push_back(op);
	}

	return postfixExp;
}


int GetPriority(wchar_t op)
{
	int priority = -1;

	switch (op)
	{
	case L'+':
	case L'-':
		priority = 1;
		break;

	case L'*':
	case L'/':
		priority = 2;
		break;

	case L'(':
	case L')':
		priority = 3;
		break;

	default:
		break;
	}

	assert(priority != -1);
	return priority;
}


int Calculate(int num, int num2, wchar_t op)
{
	int ret = INT_MIN;

	switch (op)
	{
	case L'+':
		ret = (num + num2);
		break;

	case L'-':
		ret = (num - num2);
		break;

	case L'*':
		ret = (num * num2);
		break;

	case L'/':
		ret = (num / num2);
		break;
	}

	assert(ret != INT_MIN);
	return ret;
}