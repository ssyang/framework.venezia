#pragma once

namespace _venezia
{
    /**
     * @brief ths base teplate class of all variable 
     * 
     */
    template <class _T>
    class c_variable
    {
        public:
        c_variable(){}
        c_variable( const _T & data)
        {
            m_data = data;
        }

        void set(const _T data)
        {
            m_data = data;
        }

        _T get() const
        {
            return m_data;
        }

        _T operator()() const
        {
            return m_data;
        }

        c_variable& operator+=(const c_variable& rhs) // compound assignment (does not need to be a member,
        {                           // but often is, to modify the private members)
            /* addition of rhs to *this takes place here */
            m_data += rhs.get();
            return *this; // return the result by reference
        }
        c_variable& operator+=(const double& rhs) // compound assignment (does not need to be a member,
        {                           // but often is, to modify the private members)
            /* addition of rhs to *this takes place here */
            m_data += rhs;
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
        friend c_variable operator+(
            c_variable lhs,        // passing lhs by value helps optimize chained a+b+c
            const double& rhs// otherwise, both parameters may be const references
        )
        {
            lhs += rhs; // reuse compound assignment
            return lhs; // return the result by value (uses move constructor)
        }

        private:
        _T m_data;
    };
}