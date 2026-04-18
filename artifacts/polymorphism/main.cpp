#include <iostream>
#include "classC.h"
#include <vector>

int main()
{
    ClassC objC1("name 1");
    ClassC objC2("name 2");
    ClassC objC3("name 3");
    // objC1.disp();
    // objC2.disp();
    // objC3.disp();

    std::vector<ClassC*> classCVector;
    classCVector.reserve(10);
    classCVector.push_back(new ClassC("name 1"));
    classCVector.push_back(new ClassC("name 2"));
    classCVector.push_back(new ClassC("name 3"));

    for (int i = 0; i < classCVector.size(); i++)
    {
        classCVector[i]->disp();
    }

    return 0;
}
