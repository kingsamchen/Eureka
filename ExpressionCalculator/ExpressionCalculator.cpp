#include "stdafx.h"
#include "EvaluateExp.h"
#include <conio.h>

int _tmain(int argc, _TCHAR* argv[])
{
	wcout<<L"Please input the expreesion ending up with the #"<<endl;

	wcout<<EvaluateExpression(L"1+2*3+(4*5+6)*7-8/4#")<<endl;
	wcout<<EvaluateExpression(L"12-(32-4*3)/2+1#")<<endl;
	wcout<<EvaluateExpression(L"3-(-(-(-6)))#");

	_getch();
	return 0;
}



