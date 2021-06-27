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
        std::cout << "[cgeneric]";
        return data;
    }
};

class cdouble : public _venezia::c_fun<cdouble>
{
    public:
    using _venezia::c_fun_base::operator();
    using _venezia::c_fun<cdouble>::operator=;
    protected:
    virtual _venezia::c_variable _default_forward(const _venezia::c_variable & data)
    {
        std::cout << "[cdouble]";
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
    cdouble F[7];
    cgeneric G1,G2,G3,G4;

    Eigen::MatrixXd k(1,1);
    k << 2;
    _venezia::c_variable kv(k),kv_result;
    //+++++++++++++++++++++++++++++++++++++++++++++
    //G1 = F1();
    std::cout << std::endl;

    //G2 = G1(F1());
    std::cout << std::endl;

    G4 = G3(G2(G1(F[0]())));
    std::cout << std::endl;
    std::cout << "G4 info : " << G4.info() << std::endl;
    std::cout << "G3 info : " << G3.info() << std::endl;
    std::cout << "G2 info : " << G2.info() << std::endl;
    std::cout << "G1 info : " << G1.info() << std::endl;

    kv_result = F[0](kv);
    std::cout << "F1 = " << kv_result.get() << std::endl;
    
    kv_result = G1(kv);
    std::cout << "G1 = " <<kv_result.get() << std::endl;
    
    kv_result = G2(kv);
    std::cout << "G2 = " <<kv_result.get() << std::endl;

    kv_result = G3(kv);
    std::cout << "G3 = " <<kv_result.get() << std::endl;

    kv_result = G4(kv);
    std::cout << "G4 = " <<kv_result.get() << std::endl;

    G1 = F[0]();std::cout << std::endl;
    G2 = G1();std::cout << std::endl;
    G3 = G2();std::cout << std::endl;
    G4 = G3();std::cout << std::endl;

    kv_result = G1(kv);
    std::cout << "G1 = " <<kv_result.get() << std::endl;
    
    kv_result = G2(kv);
    std::cout << "G2 = " <<kv_result.get() << std::endl;

    kv_result = G3(kv);
    std::cout << "G3 = " <<kv_result.get() << std::endl;

    kv_result = G4(kv);
    std::cout << "G4 = " <<kv_result.get() << std::endl;

    F[3] = F[2](F[1](F[0]()));
    std::cout << std::endl;
    kv_result = F[3](kv);
    std::cout << "F[3] = " <<kv_result.get() << std::endl;

    csqure FS[4];
    cexp B;
    cgeneric composite_f;
    composite_f = FS[2](FS[1](FS[0]()));

    kv_result = composite_f(kv);
    std::cout << "composite_f = " <<kv_result.get() << std::endl;
    //
    Eigen::MatrixXd data(1,1);
    data << 0.5;
    _venezia::c_variable x(data);

    _venezia::c_variable dy = _venezia::numerical_differentiation(composite_f,x);

    std::cout << "dy=" << std::endl;
    std::cout << dy() << std::endl;
    
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
