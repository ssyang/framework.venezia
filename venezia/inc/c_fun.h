#pragma once

#include <inc/_c_fun.h>

namespace _venezia
{
    class c_fun : public _c_fun
    {
        public:
            _venezia::_c_var operator()(const _venezia::_c_var & in)
            {
                m_x = in;
                m_y = _default_forward(in);
                m_y.set_creator(this);
                return m_y;
            }

            bool backword(const _venezia::_c_var & dy)
            {
                bool b_result(false);
                do{
                    if( m_x.empty() ){
                        continue;
                    }
                    _venezia::_c_var dx = _default_backward(dy,m_x);
                    m_x.set_gradient(dx);
                    b_result = m_x.backword();
                }while(false);
                return b_result;
            }

        protected:
            virtual _venezia::_c_var _default_forward(const _venezia::_c_var & in) = 0;
            virtual _venezia::_c_var _default_backward(const _venezia::_c_var  & gy,const _venezia::_c_var  & x) =0 ;

            virtual _venezia::_c_var _default_forward(const _venezia::_c_var & in) = 0;
            virtual _venezia::_c_var _default_backward(const _venezia::_c_var  & gy,const _venezia::_c_var  & x) =0 ;

        protected:
            _venezia::_c_var m_x,m_y;  
    };
}