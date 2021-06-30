#pragma once

#include <inc/c_var_base.h>

namespace _venezia
{
    class c_variable : public c_var_base
    {
    public:
    using _venezia::c_var_base::operator();
    using _venezia::c_var_base::operator=;
    using _venezia::c_var_base::operator+=;
    using _venezia::c_var_base::operator-=;
    using _venezia::c_var_base::operator*=;
    using _venezia::c_var_base::operator/=;

    virtual ~c_variable(){};

    c_variable() : c_var_base(){};
    c_variable( int d ) : c_var_base(d){};
    c_variable( double d ) : c_var_base(d){};
    c_variable( const Eigen::MatrixXd & matrix_data ) : c_var_base(matrix_data){};
    c_variable(EIGEN_DEFAULT_DENSE_INDEX_TYPE n_row, EIGEN_DEFAULT_DENSE_INDEX_TYPE n_col)
     : c_var_base(n_row,n_col)
    {};
    c_variable( const c_var_base& rhs ) : c_var_base(rhs){};

    protected:

    };
}