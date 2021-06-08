#include <iostream>

#include <inc/c_variable.h>
#include <inc/c_function.h>
#include <inc/function.h>

class csqure : public _venezia::c_function<int>
{

    protected:
    int _forward(const int & data)
    {
        return data*data;
    }
};

int main()
{
    std::wcout << "test t1 gogo." <<std::endl;

    _venezia::c_variable<int> a(5);
    
    std::wcout << " a = "<<a.get() <<std::endl;
    std::wcout <<" a = "<< a() <<std::endl;
    //
    csqure fun_squre;
    _venezia::c_variable<int> b = fun_squre(a);
    std::wcout <<" b = "<< b() <<std::endl;

    //
    _venezia::c_variable<int> x(7);
    _venezia::c_variable<int> dy = _venezia::numerical_differentiation(fun_squre,x);
    std::wcout <<" dy = "<< dy() <<std::endl;

}
