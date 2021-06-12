#include <iostream>

#include <inc/c_variable.h>
#include <inc/c_function.h>
//#include <inc/function.h>

class csqure : public _venezia::c_function
{

    protected:
    _venezia::c_variable _forward(const _venezia::c_variable & data)
    {
        return data*data;
    }
};

void _test_matrix()
{
    Eigen::MatrixXd data23(2,3);
    data23 << 11,12,13,21,22,23;

    Eigen::MatrixXd data32(3,2);
    data32 << 11,12,21,22,31,32;

    _venezia::c_variable a(data23);
    _venezia::c_variable b(data32);
    
    std::cout << " a = " <<std::endl;
    std::cout << a.get() <<std::endl;


    std::cout << " b = " <<std::endl;
    std::cout << b() <<std::endl;

    std::cout << (a*b)() <<std::endl;

    std::cout <<std::endl;
    std::cout << (a*2).get() <<std::endl;

    std::cout <<std::endl;
    std::cout << (3*a).get() <<std::endl;

    std::cout <<std::endl;
    std::cout << (1.1*a).get() <<std::endl;

    std::cout <<std::endl;
    std::cout << (1.2f*a).get() <<std::endl;
}

int main()
{

    std::cout << "test t1 gogo." <<std::endl;
    _test_matrix();


    //
    Eigen::MatrixXd data23(2,3);
    data23 << 11,12,13,21,22,23;

    _venezia::c_variable a(data23);
    
    csqure fun_squre;
    _venezia::c_variable b = fun_squre(a);
    std::cout <<" b = "<< b() <<std::endl;

    /*
    _venezia::c_variable x(Eigen::MatrixXd(7));
    _venezia::c_variable dy = _venezia::numerical_differentiation(fun_squre,x);
    std::wcout <<" dy = "<< dy() <<std::endl;
    */
}
