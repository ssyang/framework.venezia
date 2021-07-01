#pragma once

#include <inc/c_var_base.h>
#include <inc/c_fun_base.h>

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

    void set_creator(const _venezia::_c_fun_base &creator)
    {
        m_ptr_creator = _venezia::_c_fun_base::type_ptr(new _venezia::_c_fun_base(creator));
    }

    _venezia::_c_fun_base::type_ptr get_creator() const
    {
        return m_ptr_creator;
    }

    void backword()
    {
        if(m_ptr_creator){
            c_variable x = m_ptr_creator->m_var_in;
            x.set_gradient(m_ptr_creator->backword(m_matrix_grad));
            x.backword();
        }
    }

    protected:
        _venezia::_c_fun_base::type_ptr m_ptr_creator;
    };
}