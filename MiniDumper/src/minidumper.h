/*
@ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef MINIDUMPER_MINIDUMPER_H_
#define MINIDUMPER_MINIDUMPER_H_

#include <string>

struct _EXCEPTION_POINTERS;
using EXCEPTION_POINTERS = _EXCEPTION_POINTERS;

void CreateMiniDump(const std::wstring& path);
void WriteMiniDumpFile(const std::wstring& path, EXCEPTION_POINTERS* exception_pointers);

#endif  // MINIDUMPER_MINIDUMPER_H_