#ifndef CLASS_B_H
#define CLASS_B_H

#include <iostream>

class ClassB
{
public:
    ClassB() {}
    ~ClassB() {}

    virtual void disp()
    {
        std::cout << "B" << std::endl;
    }
};

#endif // CLASS_B_H