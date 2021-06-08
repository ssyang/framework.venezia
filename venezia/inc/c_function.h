#pragma once

#include <inc/c_variable.h>

namespace _venezia
{
    /**
     * @brief ths base class of all function 
     * this is virtual class
     * 
     */
    template <class _T>    
    class c_function
    {
        public:
            _venezia::c_variable<_T> operator()(const _venezia::c_variable<_T> & in_variable )
            {
                _T x(in_variable.get());
                _T y = _forward(x);
                return  _venezia::c_variable<_T>(y);
            }

        protected:
            virtual _T _forward(const _T & data) = 0;

    };
}