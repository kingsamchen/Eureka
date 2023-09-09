// Expression Tree.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "exptree.h"
#include <conio.h>
#include <iostream>
#include <crtdbg.h>

using std::wcout;
using std::endl;
using std::wcin;

int ExpEval(const wchar_t* pszExp);

int _tmain(int argc, _TCHAR* argv[])
{	
	std::wstring exp;
	wcin>>exp;
	wcout<<ExpEval(exp.c_str());
	_getch();
	return 0;
}

int ExpEval(const wchar_t* pszExp)
{
	std::wstring pexp = InfixToPostfix(pszExp);
	node* pR = ConstructExpTree(pexp);
	int ret = Eval(pR);
	DestroyExpTree(pR);
	return ret;
}