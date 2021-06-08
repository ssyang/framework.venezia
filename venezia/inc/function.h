#pragma once

#include <inc/c_variable.h>
#include <inc/c_function.h>

namespace _venezia
{
    /**
     * @brief common function of venezia namespace.
     * this is virtual class
     * 
     */

    template<class _T> 
    _T numerical_differentiation( _venezia::c_function<_T> & fun, const _venezia::c_variable<_T> & var, double d_eps = 1.00e-4 )
    {
        _venezia::c_variable<_T> x0(var.get()-d_eps);
        _venezia::c_variable<_T> x1(var.get()+d_eps);

        _venezia::c_variable<_T> y0(fun(x0));
        _venezia::c_variable<_T> y1(fun(x1));
        return (y1.get()-y0.get())/(2.00*d_eps);
    }
}