#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <typeinfo>

#include <cmath>

#include <inc/c_var.h>
#include <inc/c_fun.h>
#include <inc/function.h>

#include <inc/c_var.h>
#include <inc/c_fun.h>

class cunit : public _venezia::c_fun
{
    public:
    using _venezia::c_fun::operator();
    using _venezia::c_fun::operator=;
    public:
    cunit(){}
    cunit( const _venezia::c_fun::type_ptr_list_ptr & ptr_list_ptr){}

    protected:
    virtual const  _venezia::c_var &_default_forward(const _venezia::c_var & x)
    {
        static _venezia::c_var y;
        y = x;
        return y;
    }
    virtual const  _venezia::c_var& _default_backward(const _venezia::c_var  & gy,const _venezia::c_var  & x)
    {
        static _venezia::c_var gx;
        gx = 1;
        return gx;
    };
    virtual void *_new_instance()
    {
        return new cunit();
    }

};

class cdouble : public _venezia::c_fun
{
    public:
    using _venezia::c_fun::operator();
    using _venezia::c_fun::operator=;
    protected:
    virtual const  _venezia::c_var &_default_forward(const _venezia::c_var & x)
    {
        static _venezia::c_var y;
        y = 2*x;
        return y;
    }
    virtual const  _venezia::c_var& _default_backward(const _venezia::c_var  & gy,const _venezia::c_var  & x)
    {
        static _venezia::c_var gx;
        gx = 2*gy;
        return gx;
    };
    virtual void *_new_instance()
    {
        return new cdouble();
    }

};

class csqure : public _venezia::c_fun
{
    public:
    using _venezia::c_fun::operator();
    using _venezia::c_fun::operator=;
    protected:
    virtual const  _venezia::c_var &_default_forward(const _venezia::c_var & x)
    {
        static _venezia::c_var y;
        y = x*x;
        return y;
    }
    virtual const  _venezia::c_var &_default_backward(const _venezia::c_var  & gy,const _venezia::c_var  & x)
    {
        static _venezia::c_var gx;
        gx = 2*x*gy;
        return gx;
    };
    virtual void *_new_instance()
    {
        return new csqure();
    }

};

class cexp : public _venezia::c_fun
{
    public:
    using _venezia::c_fun::operator();
    using _venezia::c_fun::operator=;
    protected:
    virtual const _venezia::c_var& _default_forward(const _venezia::c_var & x)
    {
        static _venezia::c_var y;
        y = x;
        _venezia::c_var::type_size mt_size(y.size());

        for( size_t i = 0; i<mt_size.first; i++){
            for( size_t j = 0; j<mt_size.second; j++){
                y.set(i,j, exp(x.get()(i,j)) );
            }
        }
        return y;
    }
    virtual const _venezia::c_var &_default_backward(const _venezia::c_var  & gy,const _venezia::c_var  & x)
    {
        static _venezia::c_var gx;
        gx = gy;
        _venezia::c_var::type_size mt_size(gx.size());
        double dy,df;
        for( size_t i = 0; i<mt_size.first; i++){
            for( size_t j = 0; j<mt_size.second; j++){
                dy = gy.get()(i,j);
                df = exp(x.get()(i,j));
                gx.set(i,j,  df*dy );
            }
        }
        return gx;
    };
    virtual void *_new_instance()
    {
        return new cexp();
    }

};

void _test_matrix()
{
    Eigen::MatrixXd data23(2,3);
    data23 << 11,12,13,21,22,23;

    Eigen::MatrixXd data32(3,2);
    data32 << 11,12,21,22,31,32;

    _venezia::c_var a(data23);
    _venezia::c_var b(data32);
    
    std::cout << " a = " <<std::endl;
    std::cout << a.get() <<std::endl;


    std::cout << " b = " <<std::endl;
    std::cout << b() <<std::endl;
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
    _venezia::c_var c;

    std::cout << (a*b)() <<std::endl;
    std::cout <<std::endl;
    std::cout << (b*a)() <<std::endl;
    std::cout <<std::endl;

}

void _test_function()
{
    Eigen::MatrixXd data23(2,3);
    data23 << 1,2,3,4,5,6;

    _venezia::c_var a(data23);
    
    csqure fun_squre;
    _venezia::c_var b = fun_squre(a);
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
    _venezia::c_var a,b,c;
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

    _venezia::c_var a(data23);
    
    csqure fun_squre;
    _venezia::c_var b = fun_squre(a);
    if(b.error()){
        std::cout <<" fun_squre : error. " <<std::endl;
    }
    else{
        std::cout <<" b = " << std::endl;
        std::cout << b() <<std::endl;
    }
    //
   _venezia::c_var diff = _venezia::numerical_differentiation(fun_squre,a);
    std::cout <<" numerical_differentiation " << std::endl;
    std::cout << diff() << std::endl;
}

void _test_composite_function()
{
    cdouble F[7];
    cunit G1,G2,G3,G4;

    Eigen::MatrixXd k(1,1);
    k << 2;
    _venezia::c_var kv(k),kv_result;
    //+++++++++++++++++++++++++++++++++++++++++++++

    G4 = G3( G2( G1( (F[0])() ) ) );
    std::cout << std::endl;
    std::cout << "G4 info : " << G4.string() << std::endl;
    std::cout << "G3 info : " << G3.string() << std::endl;
    std::cout << "G2 info : " << G2.string() << std::endl;
    std::cout << "G1 info : " << G1.string() << std::endl;

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

    G1 = F[0]();
    G2 = G1();
    G3 = G2();
    G4 = G3();

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
    if(kv_result.error()){
        std::cout << "F[3] = " << "error" << std::endl;
    }
    else{
        std::cout << "F[3] = " <<kv_result.get() << std::endl;
    }

    csqure FS[4];
    cexp B;
    cunit composite_f;
    composite_f = FS[2](FS[1](FS[0]()));

    kv_result = composite_f(kv);
    std::cout << "composite_f = " <<kv_result.get() << std::endl;
    //
    Eigen::MatrixXd data(1,1);
    data << 0.5;
    _venezia::c_var x(data);

    _venezia::c_var dy = _venezia::numerical_differentiation(composite_f,x);

    std::cout << "dy=" << std::endl;
    std::cout << dy() << std::endl;
    
}


void _test_backword()
{
    csqure A,C;
    cexp B;

    _venezia::c_var x(0.5);

    //
    std::cout << std::endl;
    std::cout << "x : " << std::endl;
    std::cout << x.string() << std::endl;

    _venezia::c_var a = A(x);

    std::cout << std::endl;
    std::cout << "A : " << std::endl;
    std::cout << A.string() << std::endl;

    std::cout << std::endl;
    std::cout << "a : " << std::endl;
    std::cout << a.string() << std::endl;
    //
    _venezia::c_var b = B(a);

    std::cout << std::endl;
    std::cout << "B : " << std::endl;
    std::cout << B.string() << std::endl;

    std::cout << std::endl;
    std::cout << "b : " << std::endl;
    std::cout << b.string() << std::endl;
    //
    _venezia::c_var y = C(b);
    std::cout << std::endl;
    std::cout << "C : " << std::endl;
    std::cout << C.string() << std::endl;

    std::cout << std::endl;
    std::cout << "y : " << std::endl;
    std::cout << y.string() << std::endl;

    std::cout << "forward : "<< y() <<std::endl;
    //
    std::cout << "." <<std::endl;
    y.set_gradient(_venezia::c_var(1));
    std::cout << ".." <<std::endl;
    y.backword();
    std::cout << "..." <<std::endl;

    /////////////////////////////////////////////////////////////
    std::cout << std::endl;
    std::cout << "x : " << std::endl;
    std::cout << x.string() << std::endl;

    std::cout << "A : " << std::endl;
    std::cout << A.string() << std::endl;

    std::cout << "a : " << std::endl;
    std::cout << a.string() << std::endl;

    std::cout << "B : " << std::endl;
    std::cout << B.string() << std::endl;

    std::cout << "b : " << std::endl;
    std::cout << b.string() << std::endl;

    std::cout << "C : " << std::endl;
    std::cout << C.string() << std::endl;

    std::cout << "y : " << std::endl;
    std::cout << y.string() << std::endl;

    /////////////////////////////////////////////////////////////
    /*
    b.set_gradient(C.backword(y.get_gradient()));
    a.set_gradient(B.backword(b.get_gradient()));
    x.set_gradient(A.backword(a.get_gradient()));
    */
    _venezia::c_var out = x.get_gradient();
    std::cout << "backward : "<< out() <<std::endl;
    std::cout << "...." <<std::endl;
}

void _test_cpp()
{
    class ts
    {
        public: 
        typedef std::shared_ptr<ts> type_ptr;
        ts()
        {
            std::cout << "[ts::ts]\n";
        }
        virtual void string()
        {
            std::cout << "[ts::info]\n";
        }
    };

    class ts_a : public ts
    {
        public: 
        ts_a()
        {
            std::cout << "[ts_a::ts_a]\n";
        }

        virtual void string()
        {
            std::cout << "[ts_a::info]\n";
        }
    };

    ts root;
    ts_a a;
    root.string();
    a.string();

    ts::type_ptr ptr_root(new ts_a());
    ptr_root->string();

}

void _test_new1()
{
    std::cout << "======================" <<std::endl;
    std::cout << "   test _test_new1." <<std::endl;
    std::cout << "======================" <<std::endl;

    csqure A,C;
    cexp B;

    _venezia::c_var x(0.5);
    std::cout << "x : " << x.string() << std::endl;

    _venezia::c_var a = A(x);

    std::cout << std::endl;
    std::cout << "a : " << a.string() << std::endl;
    //
    _venezia::c_var b = B(a);
    std::cout << "b : " << b.string() << std::endl;
    //
    _venezia::c_var y = C(b);


    std::cout << "forward : "<< y.string() <<std::endl;
    
    std::cout << "." <<std::endl;
    //y.set_gradient(_venezia::c_var(1));
    
    std::cout << ".." <<std::endl;
    if( y.backword() ){
        std::cout << "success" <<std::endl;
    }
    else{
        std::cout << "fail" <<std::endl;
    }
    
    
    std::cout << "...." <<std::endl;

    std::cout << " dy = " << y.string(true) <<std::endl;
    std::cout << " db = " << b.string(true) <<std::endl;
    std::cout << " da = " << a.string(true) <<std::endl;
    std::cout << " dx = " << x.string(true) <<std::endl;

    std::cout << " y = " << y.string() <<std::endl;
    std::cout << " b = " << b.string() <<std::endl;
    std::cout << " a = " << a.string() <<std::endl;
    std::cout << " x = " << x.string() <<std::endl;
}
void _test_new2()
{
    std::cout << "======================" <<std::endl;
    std::cout << "   test _test_new2." <<std::endl;
    std::cout << "======================" <<std::endl;

    csqure A,C;
    cdouble B;

    _venezia::c_var x(4);
    _venezia::c_var y = C(B(A(x)));


    std::cout << "forward : "<< y.string() <<std::endl;
    
    std::cout << "." <<std::endl;
    if( y.backword() ){
        std::cout << "success" <<std::endl;
    }
    else{
        std::cout << "fail" <<std::endl;
    }
    
    
    std::cout << "...." <<std::endl;

    std::cout << " dy = " << y.string(true) <<std::endl;
    std::cout << " dx = " << x.string(true) <<std::endl;

    std::cout << " y = " << y.string() <<std::endl;
    std::cout << " x = " << x.string() <<std::endl;

    cunit D(C(B(A())));
    y = D(x);
    std::cout << " y = " << y.string() <<std::endl;
    y= _venezia::numerical_differentiation(D,x);
    std::cout << " y = " << y.string() <<std::endl;
    std::cout << " x = " << x.string() <<std::endl;

}
int main()
{

    std::cout << "test t1 gogo." <<std::endl;
    //_test_matrix();
    //_test_function();
    //_test_mul();
    //_test_numerical_differentiation();
    //_test_composite_function();
    //_test_backword();
    //_test_cpp();
    //_test_new1();
    _test_new2();
    return 0;
}
