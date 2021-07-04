#pragma once

#include <inc/c_var_base.h>
#include <inc/c_fun_base.h>

namespace _venezia
{
    class c_variable : public c_var_base
    {
    public:
    using _venezia::c_var_base::operator();
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

    c_variable& operator=(const c_variable& rhs)
    {
        c_var_base::operator=(rhs);
        m_creator = rhs.m_creator;
        m_b_set_creatotr = rhs.m_b_set_creatotr;
        return *this;
    }

    c_variable& operator=(const Eigen::MatrixXd& rhs)
    {
        c_var_base::operator=(rhs);
        return *this;
    }

    void set_creator(const _venezia::_c_fun_base &creator)
    {
        m_b_set_creatotr = true;
        m_creator = creator;
    }

    virtual void backword()
    {
        if(m_b_set_creatotr){
            std::cout << "^";
            c_var_base dx = m_creator.backword(m_matrix_grad);
            m_creator.m_var_in.set_gradient(dx);
            m_creator.m_var_in.backword();
        }
    }

    std::string info()
    {
        std::string s_info;
        std::stringstream ss_in,ss_out;
        ss_in << m_matrix_data;
        ss_out << m_matrix_grad;

        s_info += "[";
        s_info +=typeid(this).name();
        s_info += "][";

        if(m_b_set_creatotr)
            s_info +=typeid(&m_creator).name();
        else
            s_info += "none";

        s_info += "][";
        s_info += ss_in.str();
        s_info += "][";
        s_info += ss_out.str();
        s_info += ']';
        return s_info;
    }

    protected:
        _venezia::_c_fun_base m_creator;
        bool m_b_set_creatotr = false;
    };
}