#ifndef CLASS_D2_H
#define CLASS_D2_H

#include <iostream>

class ClassD2  : public ClassB
{
public:
    ClassD2() {}
    ~ClassD2() {}

    virtual void disp() override
    {
        std::cout << "D2" << std::endl;
    }
};

#endif // CLASS_D2_H