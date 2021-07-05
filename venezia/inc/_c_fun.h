#pragma once

namespace _venezia
{
    class c_var;

    class _c_fun
    {
        protected:
            virtual _venezia::c_var _default_forward(const _venezia::c_var & in) = 0;
            virtual _venezia::c_var _default_backward(const _venezia::c_var  & gy,const _venezia::c_var  & x) =0 ;
    };
}