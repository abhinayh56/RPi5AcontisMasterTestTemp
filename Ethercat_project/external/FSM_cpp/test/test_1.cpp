#include <iostream>
#include "../src/SubroutineBase/SubroutineBase.h"

struct TaskAll
{
    int a = 5;
    float b = -6.789101112;
    std::string c = "This is a test string";
};

class Subroutine1 : public SubroutineBase
{
public:
    Subroutine1()
    {
        std::cout << "Subroutine1 constructor" << std::endl;
    }
    ~Subroutine1()
    {
        std::cout << "Subroutine1 destructor" << std::endl;
    }

    uint32_t config() override
    {
        std::cout << "Subroutine1 config" << std::endl;
        return CallbackStatus::SUCCESS;
    }

    uint32_t callback() override
    {
        std::cout << "Subroutine1 callback" << std::endl;
        std::cout << "a: " << p_taskAll->a << std::endl;
        return CallbackStatus::SUCCESS;
    }
};

class Subroutine2 : public SubroutineBase
{
public:
    Subroutine2()
    {
        std::cout << "Subroutine2 constructor" << std::endl;
    }
    ~Subroutine2()
    {
        std::cout << "Subroutine2 destructor" << std::endl;
    }

    uint32_t config() override
    {
        std::cout << "Subroutine2 config" << std::endl;
        return CallbackStatus::SUCCESS;
    }

    uint32_t callback() override
    {
        std::cout << "Subroutine2 callback" << std::endl;
        std::cout << "b: " << p_taskAll->b << std::endl;
        return CallbackStatus::SUCCESS;
    }
};

class Subroutine3 : public SubroutineBase
{
public:
    Subroutine3()
    {
        std::cout << "Subroutine3 constructor" << std::endl;
    }
    ~Subroutine3()
    {
        std::cout << "Subroutine3 destructor" << std::endl;
    }

    uint32_t config() override
    {
        std::cout << "Subroutine3 config" << std::endl;
        return CallbackStatus::SUCCESS;
    }

    uint32_t callback() override
    {
        std::cout << "Subroutine3 callback" << std::endl;
        std::cout << "c: " << p_taskAll->c << std::endl;
        return CallbackStatus::SUCCESS;
    }
};

int main()
{
    std::cout << "TEST: SUBROUTINE" << std::endl;

    TaskAll task_all;

    Subroutine1 sbr1;
    Subroutine2 sbr2;
    Subroutine3 sbr3;

    sbr1.setTaskAddr(&task_all);
    sbr2.setTaskAddr(&task_all);
    sbr3.setTaskAddr(&task_all);
    sbr1.config();
    sbr2.config();
    sbr3.config();
    sbr1.callback();
    sbr2.callback();
    sbr3.callback();

    return 0;
}
