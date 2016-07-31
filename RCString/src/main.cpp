
#include "rc_string.h"

int main()
{
    RCString str("hello world");
    std::cout << str << std::endl;
    RCString copy(str);
    std::cout << copy << std::endl;
    std::cout << "---" << std::endl;
    str.Append("-> Padding part");
    std::cout << "str: " << str << std::endl << "copy: " << copy << std::endl;
    return 0;
}