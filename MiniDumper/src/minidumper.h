/*
@ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef MINIDUMPER_MINIDUMPER_H_
#define MINIDUMPER_MINIDUMPER_H_

#include <string>

#include <Windows.h>

void CreateMiniDump(const std::wstring& path, EXCEPTION_POINTERS* exception_pointers);

#endif  // MINIDUMPER_MINIDUMPER_H_