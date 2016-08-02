
#include "rc_string.h"

int main()
{
    std::cout << "---test 1---\n";
    RCString str("hello world");
    RCString copy(str);
    std::cout << copy << std::endl;
    copy[1] = 'i';
    RCString tmp(copy);
    tmp.Append(":duang");
    std::cout << "---\n" << str << std::endl << copy << std::endl << tmp << std::endl;

    std::cout << "---test 2---\n";
    RCString es;
    std::cout << "size for es: " << es.size() << std::endl;
    es.Append("ab");
    std::cout << es << std::endl;
    es.Append(" is not angelababy");
    std::cout << es << std::endl;

    return 0;
}