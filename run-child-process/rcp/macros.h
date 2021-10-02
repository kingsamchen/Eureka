#pragma once

#ifndef RCP_MACROS_H_
#define RCP_MACROS_H_

#if defined(_WIN32) || defined(_WIN64)
#define OS_WIN 1
#elif defined(__linux__)
#define OS_POSIX 1
#else
#error Platform not supported
#endif

#endif // RCP_MACROS_H_
