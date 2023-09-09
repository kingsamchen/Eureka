/***************************************

** Project:Expression Tree

** File:exptree.h

** Edition:v1.0.0 Demo

** Coder:KingsamChen [MDSA Group]

** Last Modify:2011-8-6

****************************************/

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _EXPTREE_0B1CAB0A_6441_414C_B40B_326B23FC681C
#define _EXPTREE_0B1CAB0A_6441_414C_B40B_326B23FC681C

#include <string>

struct node
{
	int data;
	node* pLeftChild;
	node* pRightChild;
};

node* ConstructExpTree(const std::wstring& postfixExp);
void DestroyExpTree(node*& pRoot);
int Eval(const node* pRoot);
std::wstring InfixToPostfix(const wchar_t* pszInfixExp);
static void Visit(const node* pRoot);
static node* CreateTree(int data, const node* pLeftChild, const node* pRightChild);
static int GetPriority(wchar_t op);
static int Calculate(int num, int num2, wchar_t op);

#endif
