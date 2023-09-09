/**************************************************

** Project:ExpressionCalculator

** File:EvaluateExp.cpp

** Edition:v1.0.0 Demo

** Coder:KingsamChen [MDSA Group]

** Last Modify:2011-5-4

**************************************************/

#include "stdafx.h"
#include "EvaluateExp.h"
#include <vector>
#include <cassert>

typedef std::vector<int> stackOperand;
typedef std::vector<wchar_t> stackOperator;

#define pop(x) x.back(); \
				x.pop_back();

struct NumData
{
	NumData() : signal(false), pos(0)
	{
		wmemset(sz, L'\0', NUM_LEN);
	}

	enum{NUM_LEN = 10};
	wchar_t sz[NUM_LEN];
	bool signal;
	unsigned int pos;
};

void PushNumber(stackOperand& op, NumData& data);
int FormatNumtoInt(NumData& data);
int Calculate(int num, int num2, wchar_t op);
int GetPriority(wchar_t op);
template<typename T>
void DumpStack(const T& op);

int EvaluateExpression(wchar_t* pszExpression)
{
	stackOperand oprand;
	stackOperator oprator;
	bool isNumbericMode = true;
	NumData data;

	for (wchar_t* p = pszExpression; *p != NULL; ++p)
	{
		if (*p >= L'0' && *p <= L'9')
		{
			data.sz[data.pos++] = *p;
			isNumbericMode = false;
		}
		else if (L'-' == *p && isNumbericMode)
		{
			data.signal = !data.signal;
		}
		else	// pure operators
		{
			isNumbericMode = true;

			if (L'(' == *p)
			{
				oprator.push_back(*p);
			}
			else if (L')' == *p)
			{
				int num;

				if (data.pos)
				{
					num = FormatNumtoInt(data);
				} 
				else
				{
					num = pop(oprand);
				}
				
				do 
				{
					wchar_t op = pop(oprator);
					
					if (L'(' == op)
					{
						oprand.push_back(num);
						break;
					} 
					else
					{
						int num2 = pop(oprand);
						num = Calculate(num2, num, op);
					}

				} while (true);
			} 
			else	// operators except ( and )
			{
				int num;

				if (data.pos)
				{
					num = FormatNumtoInt(data);
				} 
				else
				{
					num = pop(oprand);
				}

				do 
				{
					if (oprator.empty() || (GetPriority(*p) > GetPriority(oprator.back())))
					{
						oprand.push_back(num);
						oprator.push_back(*p);

						break;
					} 
					else
					{
						int num2 = pop(oprand);
						wchar_t op = pop(oprator);
						
						num = Calculate(num2, num, op);
					}
	
				} while (true);
			}

			//DumpStack(oprand);			
		}
	}

	return oprand.back();
}


int FormatNumtoInt(NumData& data)
{
	assert(data.pos);

	int num = _wtoi(data.sz);
	num = data.signal ? -num : num;

	wmemset(data.sz, L'\0', NumData::NUM_LEN);
	data.pos = 0;
	data.signal = false;

	return num;
}


//void PushNumber(stackOperand& op, NumData& data)
//{
//	if (!data.pos)
//	{
//		return;
//	}
//
//	op.push_back(FormatNumtoInt(data));
//
//	data.pos = 0;
//	data.signal = 0;
//}


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


int GetPriority(wchar_t op)
{
	int priority = -1;

	switch (op)
	{
		case L'#':
			priority = 0;
			break;
		
		case L'+':
		case L'-':
			priority = 2;
			break;

		case L'*':
		case L'/':
			priority = 3;
			break;

		case L'(':
		case L')':
			priority = 1;
			break;

		default:
			break;		
	}

	assert(priority != -1);
	return priority;
}

template<typename T>
void DumpStack(const T& op)
{
	for (T::const_iterator i = op.begin(); i != op.end(); ++i)
	{
		wcout<<*i<<L"\t";
	}

	wcout<<endl;
}