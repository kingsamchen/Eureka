/*
 @ 0xCCCCCCCC
*/

#include "stack_walker.h"

#include <mutex>
#include <sstream>

#include <Windows.h>
#include <DbgHelp.h>

#pragma comment(lib, "dbghelp.lib")

std::wstring GetExeDir()
{
    wchar_t exe_path[MAX_PATH];
    GetModuleFileNameW(nullptr, exe_path, MAX_PATH);
    auto slash_pos = wcsrchr(exe_path, L'\\');

    return std::wstring(exe_path, slash_pos);
}

// A helper class to resolve symbol information in callstack.
class SymbolContext {
public:
    ~SymbolContext() = default;

    static SymbolContext* GetInstance();

    DISALLOW_COPY(SymbolContext);

    DISALLOW_MOVE(SymbolContext);

    void ResolveCallStackToStream(const void* const* stack_frames,
                                  size_t frame_count,
                                  std::ostream& os) const;

    bool SymbolInitialized() const
    {
        return last_error_code == ERROR_SUCCESS;
    }

    unsigned long error_code() const
    {
        return last_error_code;
    }

private:
    SymbolContext();

    void InitializeSymbols();

private:
    DWORD last_error_code;
    mutable std::mutex mutex_;
};

SymbolContext::SymbolContext()
{
    InitializeSymbols();
}

// static
SymbolContext* SymbolContext::GetInstance()
{
    static SymbolContext instance;
    return &instance;
}

void SymbolContext::InitializeSymbols()
{
    SymSetOptions(SYMOPT_DEFERRED_LOADS |
                  SYMOPT_FAIL_CRITICAL_ERRORS |
                  SYMOPT_LOAD_LINES |
                  SYMOPT_UNDNAME);

    if (!SymInitializeW(GetCurrentProcess(), nullptr, TRUE)) {
        last_error_code = GetLastError();
        return;
    }

    // The directory that contains the executable file is not on the list by default,
    // include it on our onw.
    constexpr size_t kMaxListSize = 1024;
    wchar_t search_path_buffer[kMaxListSize];

    if (!SymGetSearchPathW(GetCurrentProcess(), search_path_buffer, kMaxListSize)) {
        last_error_code = GetLastError();
        return;
    }

    std::wstring search_path_list(search_path_buffer);
    search_path_list.append(L";").append(GetExeDir());

    if (!SymSetSearchPathW(GetCurrentProcess(), search_path_buffer)) {
        last_error_code = GetLastError();
        return;
    }

    last_error_code = ERROR_SUCCESS;
}

void SymbolContext::ResolveCallStackToStream(const void* const* stack_frames, size_t frame_count,
                                             std::ostream& os) const
{
    std::lock_guard<std::mutex> lock(mutex_);

    constexpr int kSymbolInfoBufferSize = sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(char);
    uint8_t buffer[kSymbolInfoBufferSize];

    for (size_t i = 0; i < frame_count; ++i) {
        DWORD64 frame = reinterpret_cast<DWORD64>(stack_frames[i]);
        // Clean the buffer.
        memset(buffer, 0, sizeof(buffer));

        PSYMBOL_INFO symbol_info = reinterpret_cast<PSYMBOL_INFO>(buffer);
        symbol_info->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol_info->MaxNameLen = MAX_SYM_NAME;
        DWORD64 symbol_displacement = 0;
        BOOL symbol_resolved = SymFromAddr(GetCurrentProcess(), frame, &symbol_displacement,
                                           symbol_info);

        DWORD line_displacement = 0;
        IMAGEHLP_LINE64 line_info = { sizeof(IMAGEHLP_LINE64) };
        BOOL line_resolved = SymGetLineFromAddr64(GetCurrentProcess(), frame, &line_displacement,
                                                  &line_info);

        os << "\t";
        if (symbol_resolved) {
            os << symbol_info->Name << " [0x" << stack_frames[i] << "+" << symbol_displacement
               << "]";
        } else {
            os << "(No Symbol) [0x" << stack_frames[i] << "]";
        }

        if (line_resolved) {
            os << " (" << line_info.FileName << ":" << line_info.LineNumber << ")";
        }

        os << "\n";
    }
}

// Prevent stack frames getting messed by FPO.
#pragma optimize("", off)

StackWalker::StackWalker()
{
    auto captured_frame_count = RtlCaptureStackBackTrace(0,
                                                         static_cast<DWORD>(stack_frames_.size()),
                                                         stack_frames_.data(),
                                                         nullptr);
    valid_frame_count_ = captured_frame_count;
}

#pragma optimize("", on)

StackWalker::StackWalker(CONTEXT* context)
{
    STACKFRAME64 stack_frame { 0 };
#if defined(_WIN64)
    unsigned int machine_arch = IMAGE_FILE_MACHINE_AMD64;
    stack_frame.AddrPC.Offset = context->Rip;
    stack_frame.AddrFrame.Offset = context->Rbp;
    stack_frame.AddrStack.Offset = context->Rsp;
#else
    unsigned int machine_arch = IMAGE_FILE_MACHINE_I386;
    stack_frame.AddrPC.Offset = context->Eip;
    stack_frame.AddrFrame.Offset = context->Ebp;
    stack_frame.AddrStack.Offset = context->Esp;
#endif
    stack_frame.AddrPC.Mode = AddrModeFlat;
    stack_frame.AddrFrame.Mode = AddrModeFlat;
    stack_frame.AddrStack.Mode = AddrModeFlat;
}

void StackWalker::OutputCallStack(std::ostream& stream)
{
    if (!SymbolContext::GetInstance()->SymbolInitialized()) {
        stream << "Failed to initialize symbols ("
               << SymbolContext::GetInstance()->error_code()
               << "). Dumping unresolved callstack:\n";
        for (size_t i = 0; i < valid_frame_count_; ++i) {
            stream << "\t" << stack_frames_[i] << "\n";
        }
    } else {
        stream << "Dumping callstack:\n";
        SymbolContext::GetInstance()->ResolveCallStackToStream(stack_frames_.data(),
                                                               valid_frame_count_,
                                                               stream);
    }
}

std::string StackWalker::CallStackToString()
{
    std::ostringstream callstack;
    OutputCallStack(callstack);

    return callstack.str();
}
