#ifndef CLASS_B_H
#define CLASS_B_H

#include <iostream>
#include "classB2.h"

class ClassB
{
public:
    ClassB(): b2(12) {}
    ~ClassB() {}

    void disp()
    {
        std::cout << "B " << b2.disp() << std::endl;
    }

private:
    ClassB2 b2;
};

#endif // CLASS_B_H