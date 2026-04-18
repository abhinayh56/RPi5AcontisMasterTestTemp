#ifndef CLASS_D3_H
#define CLASS_D3_H

#include <iostream>

class ClassD3  : public ClassB
{
public:
    ClassD3() {}
    ~ClassD3() {}

    virtual void disp() override
    {
        std::cout << "D3" << std::endl;
    }
};

#endif // CLASS_D3_H