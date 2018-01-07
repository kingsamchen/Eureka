
#include <cxxabi.h>
#include <execinfo.h>

#include <iostream>
#include <string>

std::string CaptureStackTrace()
{
    const size_t kMaxLen = 64;
    void* stack_frames[kMaxLen];

    int valid_frame_count = backtrace(stack_frames, kMaxLen);
    if (valid_frame_count == 0) {
        return std::string("<empty, possibily corrupt>\n");
    }

    size_t func_name_length = 256;
    char* func_name = static_cast<char*>(malloc(sizeof(char) * func_name_length));

    char** symbol_list = backtrace_symbols(stack_frames, valid_frame_count);
    std::string stack_trace;
    if (symbol_list) {
        for (int i = 0; i < valid_frame_count; ++i) {
            std::string symbol(symbol_list[i]);
            auto begin_name = symbol.find('(') + 1;
            auto end_name = symbol.find('+', begin_name);
            if (end_name == std::string::npos) {
                std::cerr << "no offset found" << std::endl;
                stack_trace.append(symbol).append("\n");
            } else {
                int status;
                auto name = symbol.substr(begin_name, end_name - begin_name);
                auto demangled = abi::__cxa_demangle(name.c_str(),
                                                     func_name,
                                                     &func_name_length,
                                                     &status);
                if (status == 0) {
                    func_name = demangled;
                    symbol.replace(begin_name, end_name - begin_name, func_name);
                    stack_trace.append(symbol).append("\n");
                } else {
                    std::cerr << "demangle failed: " << status << std::endl;
                    stack_trace.append(symbol).append("\n");
                }
            }
        }
    }

    free(func_name);
    free(symbol_list);

    return stack_trace;
}

void dump()
{
    auto callstack = CaptureStackTrace();
    std::cout << callstack;
}

void bar()
{
    dump();
}

void foo()
{
    bar();
}

int main(int, char* [])
{
    foo();
    return 0;
}
