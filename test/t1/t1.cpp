#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include <inc/c_variable.h>
#include <inc/c_function.h>
//#include <inc/function.h>


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


class csqure : public _venezia::c_function
{

    protected:
    _venezia::c_variable _forward(const _venezia::c_variable & data)
    {
        //return data*data;
        return data*data;
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

int main()
{

    std::cout << "test t1 gogo." <<std::endl;
    //_test_matrix();
    //_test_function();
    _test_mul();

    //

    /*
    _venezia::c_variable x(Eigen::MatrixXd(7));
    _venezia::c_variable dy = _venezia::numerical_differentiation(fun_squre,x);
    std::wcout <<" dy = "<< dy() <<std::endl;
    */
}
