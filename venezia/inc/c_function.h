
#pragma once

#include <inc/c_fun_base.h>

namespace _venezia
{
    /**
     * @brief c_function : unit function
     * 
     */
    class c_function : public _venezia::c_fun_base_t<c_function>
    {
        public:
        using _venezia::_c_fun_base::operator();
        using _venezia::c_fun_base_t<c_function>::operator=;

        protected:
        virtual _venezia::c_var_base _default_forward(const _venezia::c_var_base & x)
        {
            return x;//y=x function
        }
        virtual _venezia::c_var_base _default_backward(const _venezia::c_var_base  & gy,const _venezia::c_var_base  & x)
        {
            return _venezia::c_var_base(1);
        };

    };

}