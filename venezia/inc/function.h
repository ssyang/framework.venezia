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

    Eigen::MatrixXd numerical_differentiation( _venezia::c_function & fun, const _venezia::c_variable & var, double d_eps = 1.00e-4 )
    {
        Eigen::MatrixXd mx_out;
        Eigen::MatrixXd mx_eps(var);

        mx_eps = d_eps;

        _venezia::c_variable x0(var.get()-d_eps);
        _venezia::c_variable x1(var.get()+d_eps);

        _venezia::c_variable y0(fun(x0));
        _venezia::c_variable y1(fun(x1));
        return (y1.get()-y0.get())/(2.00*d_eps);
    }
}