#pragma once

#include <Eigen/Dense>

namespace _venezia
{
    /**
     * @brief ths base class of all variable 
     * 
     */
    class c_variable
    {
        public:
        c_variable(){}
        c_variable( const Eigen::MatrixXd & matrix_data )
        {
            m_matrix_data = matrix_data;
        }

        ~c_variable(){}

        void set(const Eigen::MatrixXd & matrix_data)
        {
            m_matrix_data = matrix_data;
        }

        Eigen::MatrixXd get() const
        {
            return m_matrix_data;
        }

        Eigen::MatrixXd operator()() const
        {
            return m_matrix_data;
        }

        c_variable& operator+=(const c_variable& rhs)
        {
            m_matrix_data += rhs.get();
            return *this;
        }

        friend c_variable operator+( c_variable lhs, const c_variable& rhs )
        {
            lhs += rhs;
            return lhs;
        }

        c_variable& operator-=(const c_variable& rhs)
        {
            m_matrix_data -= rhs.get();
            return *this;
        }

        friend c_variable operator-( c_variable lhs, const c_variable& rhs )
        {
            lhs -= rhs;
            return lhs;
        }

        c_variable& operator*=(const c_variable& rhs)
        {
            m_matrix_data *= rhs.get();
            return *this;
        }

        friend c_variable operator*( c_variable lhs, const c_variable& rhs )
        {
            lhs *= rhs;
            return lhs;
        }

        /*
        * scala operator
        */
       //int
        c_variable& operator*=(const int& rhs)
        {
            m_matrix_data *= rhs;
            return *this;
        }
        friend c_variable operator*( c_variable lhs, const int& rhs )
        {
            lhs *= rhs;
            return lhs;
        }
        friend c_variable operator*( const int& lhs, c_variable rhs )
        {
            rhs *= lhs;
            return rhs;
        }
        //long
        c_variable& operator*=(const long& rhs)
        {
            m_matrix_data *= rhs;
            return *this;
        }
        friend c_variable operator*( c_variable lhs, const long& rhs )
        {
            lhs *= rhs;
            return lhs;
        }
        friend c_variable operator*( const long& lhs, c_variable rhs )
        {
            rhs *= lhs;
            return rhs;
        }
        //float
        c_variable& operator*=(const float& rhs)
        {
            m_matrix_data *= rhs;
            return *this;
        }
        friend c_variable operator*( c_variable lhs, const float& rhs )
        {
            lhs *= rhs;
            return lhs;
        }
        friend c_variable operator*( const float& lhs, c_variable rhs )
        {
            rhs *= lhs;
            return rhs;
        }
        //double
        c_variable& operator*=(const double& rhs)
        {
            m_matrix_data *= rhs;
            return *this;
        }
        friend c_variable operator*( c_variable lhs, const double& rhs )
        {
            lhs *= rhs;
            return lhs;
        }
        friend c_variable operator*( const double& lhs, c_variable rhs )
        {
            rhs *= lhs;
            return rhs;
        }

        private:
            Eigen::MatrixXd m_matrix_data;
    };
}