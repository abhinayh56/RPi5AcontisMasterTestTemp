#ifndef CLASS_B2_H
#define CLASS_B2_H

#include <iostream>

class ClassB2
{
public:
    ClassB2(int a): b(a) {}
    ~ClassB2() {}

    int disp()
    {
        std::cout << " B2 " << b << " " << std::endl;
        return 0;
    }

    int b;
};

#endif // CLASS_B2_H