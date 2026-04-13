#include <iostream>

using namespace std;

int a = 1;
int b = 1;
int c = 1;

void f1();
void f2();
void f3();



int main()
{
    a = 1;
    b = 2;
    c = 2;

    f1();

    return 0;
}

void f1()
{
    cout << "f1" << endl;
    if(a==1) f2();
    if(a==2) f3();
}

void f2()
{
    cout << "f2" << endl;
    if(b==1) f1();
    if(b==2) f3();
}

void f3()
{
    cout << "f3" << endl;
    if(c==1) f3();
    if(c==2) f1();
}