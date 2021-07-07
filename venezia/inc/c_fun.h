#pragma once

#include <inc/_c_fun.h>
#include <inc/c_var.h>

namespace _venezia
{
   
    class c_fun
    {
        protected:
            virtual const _venezia::c_var& _default_forward(const _venezia::c_var & p_in) = 0;
            virtual const _venezia::c_var& _default_backward(const _venezia::c_var  & gy,const _venezia::c_var  & x) =0 ;

        public:
            _venezia::c_var operator()(const _venezia::c_var & in)
            {
                m_x = in;
                m_y = _default_forward(in);
                m_y.set_creator(this);
                return m_y;
            }

            bool backword(const _venezia::c_var & dy)
            {
                bool b_result(false);
                do{
                    if( m_x.empty() ){
                        continue;
                    }
                    _venezia::c_var dx = _default_backward(dy,m_x);
                    m_x.set_gradient(dx.get());
                    b_result = m_x.backword();
                }while(false);
                return b_result;
            }


        protected:
            _venezia::c_var m_x,m_y;  
    };
}