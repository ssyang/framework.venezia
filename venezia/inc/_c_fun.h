#pragma once

#include <inc/_c_var.h>

namespace _venezia
{
    class _c_fun
    {
        protected:
            virtual _venezia::_c_var _default_forward(const _venezia::_c_var & in) = 0;
            virtual _venezia::_c_var _default_backward(const _venezia::_c_var  & gy,const _venezia::_c_var  & x) =0 ;
    };
}