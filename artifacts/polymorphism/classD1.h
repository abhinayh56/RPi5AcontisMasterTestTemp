#ifndef CLASS_D1_H
#define CLASS_D1_H

#include <iostream>

class ClassD1 : public ClassB
{
public:
    ClassD1() {}
    ~ClassD1() {}

    virtual void disp() override
    {
        std::cout << "D1" << std::endl;
    }
};

#endif // CLASS_D1_H