// Data Functions Design.cpp
//

#include "stdafx.h"
#include <conio.h>
#include <iostream>

using std::wcout;
using std::endl;

struct Date
{
	Date(int y, int m, int d) : Year(y), Month(m), Day(d)
	{}
	int Year;
	int Month;
	int Day;
};

int DateDiff(const Date& dateFirst, const Date& dateSecond);
int QueryWeekday(const Date& date);
void PrintCalendar(int Year, int Month);

int _tmain(int argc, _TCHAR* argv[])
{
	wcout<<L"\t2011-11\n";
	PrintCalendar(2011, 11);
	wcout<<L"\n\n";
	wcout<<L"\t2011-12\n";
	PrintCalendar(2011, 12);
	wcout<<L"\n\n";
	wcout<<L"\t2012-1\n";
	PrintCalendar(2012, 1);
	wcout<<L"\n\n";
	wcout<<L"\t2012-2\n";
	PrintCalendar(2012, 2);
	wcout<<L"\n\n";
	wcout<<L"\t2012-3\n";
	PrintCalendar(2012, 3);
	_getch();
	return 0;
}


inline bool IsLeapYear(int Year)
{
	return ((!(Year % 4) && (Year % 100)) || !(Year % 400)) ? true : false;
}


inline int GetDaysPast(const Date& date)
{
	int daysPastPerMonth[11] = {31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
	int days = date.Month > 1 ? daysPastPerMonth[date.Month - 2] + date.Day : date.Day;
	return IsLeapYear(date.Year) && date.Month > 2 ? ++days : days;
}


int DateDiff(const Date& dateFirst, const Date& dateSecond)
{
	// be sure that day2 is later
	int day1 = GetDaysPast(dateFirst);
	int day2 = GetDaysPast(dateSecond);

	// in a one year
	if (dateFirst.Year == dateSecond.Year)
	{
		return day2 - day1;
	}

	int diff = day2 - day1 + (dateSecond.Year - dateFirst.Year) * 365;

	for (int i = dateFirst.Year; i < dateSecond.Year; ++i)
	{
		if (IsLeapYear(i))
		{
			++diff;
		}
	}

	return diff;
}


int QueryWeekday(const Date& date)
{
	// Sunday
	const Date eraDay(1899, 12, 31);
	int dayDiff = DateDiff(eraDay, date);

	return dayDiff % 7;
}


void PrintCalendar(int Year, int Month)
{
	int daysPerMon[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int daysTotal = daysPerMon[Month - 1];

	if (IsLeapYear(Year) && (2 == Month))
	{
		++daysTotal;
	}

	// query the first weekday in the month
	int firstWeekday = QueryWeekday(Date(Year, Month, 1));

	wcout<<L"Sun\tMon\tTue\tWen\tThu\tFri\tSat"<<endl;

	// set start position
	for (int i = 0; i < firstWeekday; ++i)
	{
		wcout<<L"\t";
	}

	int e = firstWeekday ? 7 - firstWeekday : 0;
	for (int i = 1; i <= daysTotal; ++i)
	{
		wcout<<i;

		if (i % 7 == e)
		{
			wcout<<endl;
		}
		else
		{
			wcout<<L"\t";
		}
	}
}