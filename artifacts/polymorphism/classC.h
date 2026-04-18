#ifndef CLASS_C_H
#define CLASS_C_H

#include <iostream>
#include <string>

class ClassC
{
public:
    ClassC(const std::string &name) : name(name) {}
    ~ClassC() {}

    virtual void disp()
    {
        std::cout << "C : " << name << std::endl;
    }

private:
    std::string name;
};

#endif // CLASS_C_H