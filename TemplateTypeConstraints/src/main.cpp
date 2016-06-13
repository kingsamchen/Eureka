/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include "type_constraints.h"

class A {};
class B : public A {};

int main()
{
    std::cout << IsDerivedFrom<B, A>::value;
    return 0;
}