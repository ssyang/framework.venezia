#include <iostream>

#include <inc/c_variable.h>
#include <inc/c_function.h>
#include <inc/function.h>

class csqure : public _venezia::c_function
{

    protected:
    Eigen::MatrixXd _forward(const Eigen::MatrixXd & data)
    {
        return data*data;
    }
};

int main()
{
    std::wcout << "test t1 gogo." <<std::endl;

    _venezia::c_variable a(Eigen::MatrixXd(5));
    
    std::cout << " a = "<<a.get() <<std::endl;
    std::cout <<" a = "<< a() <<std::endl;
    //
    csqure fun_squre;
    _venezia::c_variable b = fun_squre(a);
    std::cout <<" b = "<< b() <<std::endl;

    //
    _venezia::c_variable x(Eigen::MatrixXd(7));
    _venezia::c_variable dy = _venezia::numerical_differentiation(fun_squre,x);
    std::cout <<" dy = "<< dy() <<std::endl;

}
