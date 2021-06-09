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

        c_variable& operator+=(const c_variable& rhs) // compound assignment (does not need to be a member,
        {                           // but often is, to modify the private members)
            /* addition of rhs to *this takes place here */
            m_matrix_data += rhs.get();
            return *this; // return the result by reference
        }

        friend c_variable operator+(
            c_variable lhs,        // passing lhs by value helps optimize chained a+b+c
            const c_variable& rhs// otherwise, both parameters may be const references
        )
        {
            lhs += rhs; // reuse compound assignment
            return lhs; // return the result by value (uses move constructor)
        }

        private:
            Eigen::MatrixXd m_matrix_data;
    };
}