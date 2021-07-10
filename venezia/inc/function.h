#pragma once

#include <inc/c_var.h>
#include <inc/c_fun.h>

namespace _venezia
{
    /**
     * @brief common function of venezia namespace.
     * this is virtual class
     * 
     */

    _venezia::c_var numerical_differentiation( _venezia::c_fun &fun, const _venezia::c_var & var, double d_eps = 1.00e-4 )
    {
        _venezia::c_var result;

        _venezia::c_var x0(var-d_eps);
        _venezia::c_var x1(var+d_eps);

        _venezia::c_var y0(fun(x0));
        _venezia::c_var y1(fun(x1));

        result = (y1.get()-y0.get())/(2.00*d_eps);
        return result;
    }
}