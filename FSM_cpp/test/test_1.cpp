#include <iostream>
#include "../src/Fsm/Fsm.h"

class OnEntrySubroutine : public Subroutine
{
public:
    OnEntrySubroutine(const std::string &name) : m_name(name)
    {
    }

    ~OnEntrySubroutine()
    {
    }

    uint32_t config() override
    {
        std::cout << "config: " << m_name << std::endl;
        return 0;
    }

    uint32_t callback() override
    {
        std::cout << "    on entry: " << m_name << std::endl;
        return 0;
    }

private:
    std::string m_name;
};

class OnExitSubroutine : public Subroutine
{
public:
    OnExitSubroutine(const std::string &name) : m_name(name)
    {
    }

    ~OnExitSubroutine()
    {
    }

    uint32_t config() override
    {
        std::cout << "config: " << m_name << std::endl;
        return 0;
    }

    uint32_t callback() override
    {
        std::cout << "    on exit: " << m_name << std::endl;
        return 0;
    }

private:
    std::string m_name;
};

class CallbackSubroutine : public Subroutine
{
public:
    CallbackSubroutine(const std::string &name) : m_name(name)
    {
    }

    ~CallbackSubroutine()
    {
    }

    uint32_t config() override
    {
        std::cout << "config: " << m_name << std::endl;
        return 0;
    }

    uint32_t callback() override
    {
        std::cout << "    callback: " << m_name << std::endl;
        return 0;
    }

private:
    std::string m_name;
};

class TransitionSubroutine : public SubroutineTransition
{
public:
    TransitionSubroutine(const std::string &name) : m_name(name)
    {
    }

    ~TransitionSubroutine()
    {
    }

    uint32_t config() override
    {
        std::cout << "config: " << m_name << std::endl;
        return 0;
    }

    uint32_t callback(uint32_t &nextStateId) override
    {
        std::cout << "    transition: " << m_name << std::endl;
        
        if(m_name == "state_a_trans")
        {
            nextStateId = 2;
        }
        else if(m_name == "state_b_trans")
        {
            nextStateId = 3;
        }
        else if(m_name == "state_c_trans")
        {
            nextStateId = 1;
        }

        return 0;
    }

private:
    std::string m_name;
};

int main()
{
    State state_a("state_a", 1);
    State state_b("state_b", 2);
    State state_c("state_c", 3);

    OnEntrySubroutine state_a_entry("state_a_entry");
    OnEntrySubroutine state_b_entry("state_b_entry");
    OnEntrySubroutine state_c_entry("state_c_entry");

    OnExitSubroutine state_a_exit("state_a_exit");
    OnExitSubroutine state_b_exit("state_b_exit");
    OnExitSubroutine state_c_exit("state_c_exit");

    CallbackSubroutine state_a_cb("state_a_cb");
    CallbackSubroutine state_b_cb("state_b_cb");
    CallbackSubroutine state_c_cb("state_c_cb");

    TransitionSubroutine state_a_trans("state_a_trans");
    TransitionSubroutine state_b_trans("state_b_trans");
    TransitionSubroutine state_c_trans("state_c_trans");

    state_a.addOnEntry(&state_a_entry);
    state_a.addOnExit(&state_a_exit);
    state_a.addCallback(&state_a_cb);
    state_a.addTransition(&state_a_trans);

    state_b.addOnEntry(&state_b_entry);
    state_b.addOnExit(&state_b_exit);
    state_b.addCallback(&state_b_cb);
    state_b.addTransition(&state_b_trans);

    state_c.addOnEntry(&state_c_entry);
    state_c.addOnExit(&state_c_exit);
    state_c.addCallback(&state_c_cb);
    state_c.addTransition(&state_c_trans);

    Fsm fsm(2);
    fsm.addState(&state_a);
    fsm.addState(&state_b);
    fsm.addState(&state_c);

    fsm.config();

    std::cout << "===\nFSM Updating" << std::endl;

    uint32_t num_iteration = 10;

    for(uint32_t i=1; i<=num_iteration; i++)
    {
        fsm.update();
    }

    return 0;
}
