#include <iostream>
#include "classB.h"
#include "classD1.h"
#include "classD2.h"
#include "classD3.h"
#include <vector>

int main()
{
    // ClassB objB;
    // objB.disp();

    ClassD1 objD1;
    ClassD2 objD2;
    ClassD3 objD3;
    // objD1.disp();
    // objD2.disp();
    // objD3.disp();

    // ClassB* p_classB = nullptr;
    // p_classB = &objD1;
    // p_classB->disp();

    std::vector<ClassB *> p_classBVector;
    p_classBVector.reserve(10);
    p_classBVector.push_back(&objD1);
    p_classBVector.push_back(&objD2);
    p_classBVector.push_back(&objD3);

    for (int i = 0; i < p_classBVector.size(); i++)
    {
        p_classBVector[i]->disp();
    }

    // objD1.disp();
    // objD2.disp();
    // objD3.disp();

    return 0;
}
