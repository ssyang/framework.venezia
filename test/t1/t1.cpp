#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <typeinfo>

#include <cmath>

#include <inc/c_variable.h>
#include <inc/c_function.h>
#include <inc/function.h>


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
/*
    std::cout << (a*b)() <<std::endl;

    std::cout <<std::endl;
    std::cout << (a*2).get() <<std::endl;

    std::cout <<std::endl;
    std::cout << (3*a).get() <<std::endl;

    std::cout <<std::endl;
    std::cout << (1.1*a).get() <<std::endl;

    std::cout <<std::endl;
    std::cout << (1.2f*a).get() <<std::endl;
*/
    std::cout <<std::endl;

    Eigen::MatrixXd data13(1,3);
    data13 << 11,12,13;

    Eigen::MatrixXd data31(3,1);
    data31 << 31,32,33;

    std::cout << data13 <<std::endl;
    std::cout <<std::endl;

    std::cout << data31 <<std::endl;
    std::cout <<std::endl;

    std::cout << data31 * data13 <<std::endl;
    std::cout <<std::endl;

    //
    a = data13;
    b = data31;
    _venezia::c_variable c;

    std::cout << (a*b)() <<std::endl;
    std::cout <<std::endl;
    std::cout << (b*a)() <<std::endl;
    std::cout <<std::endl;

}

class cgeneric : public _venezia::c_fun<cgeneric>
{
    public:
    using _venezia::c_fun_base::operator();
    using _venezia::c_fun<cgeneric>::operator=;

    protected:
    virtual _venezia::c_variable _default_forward(const _venezia::c_variable & data)
    {
        return data;
    }
};

//class cdouble : public _venezia::c_fun<cdouble>
class cdouble : public _venezia::c_fun<cdouble>
{
    public:
    using _venezia::c_fun_base::operator();
    using _venezia::c_fun<cdouble>::operator=;
    protected:
    virtual _venezia::c_variable _default_forward(const _venezia::c_variable & data)
    {
        return data*2;
    }
};

class csqure : public _venezia::c_fun<csqure>
{
    public:
    using _venezia::c_fun_base::operator();
    using _venezia::c_fun<csqure>::operator=;
    protected:
    virtual _venezia::c_variable _default_forward(const _venezia::c_variable & data)
    {
        return data*data;
    }
};

class cexp : public _venezia::c_fun<cexp>
{
    public:
    using _venezia::c_fun_base::operator();
    using _venezia::c_fun<cexp>::operator=;
    protected:
    virtual _venezia::c_variable _default_forward(const _venezia::c_variable & data)
    {
        _venezia::c_variable result(data);

        _venezia::c_variable::type_size mt_size(result.size());

        for( size_t i = 0; i<mt_size.first; i++){
            for( size_t j = 0; j<mt_size.second; j++){
                result.set(i,j, exp(data.get()(i,j)) );
            }
        }
        return result;
    }
};

void _test_function()
{
    Eigen::MatrixXd data23(2,3);
    data23 << 1,2,3,4,5,6;

    _venezia::c_variable a(data23);
    
    csqure fun_squre;
    _venezia::c_variable b = fun_squre(a);
    if(b.error()){
        std::cout <<" fun_squre : error. " <<std::endl;
    }
    else{
        std::cout <<" b = " << std::endl;
        std::cout << b() <<std::endl;
    }
}

void _test_mul()
{
    _venezia::c_variable a,b,c;
    size_t N(3),M(3);

    for( size_t n_r = 0; n_r<N; n_r++ ){
        for( size_t n_c = 0; n_c<M; n_c++ ){

            Eigen::MatrixXd da(n_r,n_c);
            da.fill(2);
            a=da;

            for( size_t i = 0; i<N; i++ ){
                for( size_t j = 0; j<M; j++ ){
                    Eigen::MatrixXd db(i,j);
                    db.fill(3);
                    b = db;
                    c = a.mul(b);
                    //
                    if(c.error()){
                        //std::cout << "error" <<std::endl;
                    }
                    else{
                        std::cout << "(" << a.get().rows() << " X " << a.get().cols() << ")(" << b.get().rows() << " X " << b.get().cols() << ")"<<std::endl;
                        std::cout << c.get() <<std::endl;
                        //std::this_thread::sleep_for (std::chrono::seconds(2));                        
                    }
                }
            }
            std::cout <<std::endl;
            
        }//endf for
    }//end for
}

void _test_numerical_differentiation()
{
    Eigen::MatrixXd data23(2,3);
    data23 << 1,2,3,4,5,6;

    _venezia::c_variable a(data23);
    
    csqure fun_squre;
    _venezia::c_variable b = fun_squre(a);
    if(b.error()){
        std::cout <<" fun_squre : error. " <<std::endl;
    }
    else{
        std::cout <<" b = " << std::endl;
        std::cout << b() <<std::endl;
    }
    //
   _venezia::c_variable diff = _venezia::numerical_differentiation<csqure>(fun_squre,a);
    std::cout <<" numerical_differentiation " << std::endl;
    std::cout << diff() << std::endl;
}

void _test_composite_function()
{
    cdouble F1, F2, F3;
    cgeneric F4,F5,F6;

    Eigen::MatrixXd k(1,1);
    k << 1;
    _venezia::c_variable kv(k),kv_result;

    F4 = F3();
    F5 = F4(F3);
    //_venezia::c_fun_base f=F3();
    F6 = F5(F4);

    kv_result = F3(kv);
    std::cout << "F3 = " << kv_result.get() << std::endl;
    
    kv_result = F4(kv);
    std::cout << "F4 = " <<kv_result.get() << std::endl;
    
    kv_result = F5(kv);
    std::cout << "F5 = " <<kv_result.get() << std::endl;

    kv_result = F6(kv);
    std::cout << "F6 = " <<kv_result.get() << std::endl;

    /*
    csqure A, C;
    cexp B;
    _venezia::c_fun composite_f(C(B(A())));

    Eigen::MatrixXd data(1,1);
    data << 0.5;
    _venezia::c_variable x(data);

    _venezia::c_variable dy = _venezia::numerical_differentiation(composite_f,x);

    std::cout << "dy=" << std::endl;
    std::cout << dy() << std::endl;
    */
}

int main()
{

    std::cout << "test t1 gogo." <<std::endl;
    //_test_matrix();
    //_test_function();
    //_test_mul();
    //_test_numerical_differentiation();
    _test_composite_function();

    //

    /*
    _venezia::c_variable x(Eigen::MatrixXd(7));
    _venezia::c_variable dy = _venezia::numerical_differentiation(fun_squre,x);
    std::wcout <<" dy = "<< dy() <<std::endl;
    */
}
