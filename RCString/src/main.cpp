
#include "rc_string.h"
#include "thread_safe_rc_string.h"

void RCStringTest()
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
}

void ThreadSafeRCStringTest()
{
    std::cout << "---test 1---\n";
    ThreadSafeRCString str("hello world");
    ThreadSafeRCString copy(str);
    std::cout << copy << std::endl;
    copy[1] = 'i';
    ThreadSafeRCString tmp(copy);
    tmp.Append(":duang");
    std::cout << "---\n" << str << std::endl << copy << std::endl << tmp << std::endl;

    std::cout << "---test 2---\n";
    ThreadSafeRCString es;
    std::cout << "size for es: " << es.size() << std::endl;
    es.Append("ab");
    std::cout << es << std::endl;
    es.Append(" is not angelababy");
    std::cout << es << std::endl;
}

int main()
{
    ThreadSafeRCStringTest();
    return 0;
}