/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include "observer_list.h"

class Subject {
};

int main()
{
    ObserverList<Subject> observer_list;
    observer_list.Add(std::make_shared<Subject>());
    observer_list.Remove(std::make_shared<Subject>());
    std::cout << "hello world" << std::endl;
    return 0;
}

