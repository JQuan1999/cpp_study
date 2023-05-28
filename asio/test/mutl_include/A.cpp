#include"A.h"
#include"B.h"

A::A()
{
    b_ptr.reset(new B(1, 2));
}

void A::print()
{
    b_ptr->print();
}