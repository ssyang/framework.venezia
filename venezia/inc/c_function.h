#pragma once

#include <inc/c_variable.h>

namespace _venezia
{
    /**
     * @brief ths base class of all function 
     * this is virtual class
     * 
     */
    class c_function
    {
        public:
            c_function() : m_p_back_fun(nullptr), m_p_front_fun(nullptr)
            {}

            c_function( const c_function & f)
            {
                *this = f;
            }
            _venezia::c_variable operator()(const _venezia::c_variable & in_variable )
            {
                c_function *p_root_fun(nullptr);
                c_function *p_back_fun(m_p_back_fun);

                while(p_back_fun != nullptr){
                    p_root_fun = p_back_fun;
                    p_back_fun = p_back_fun->m_p_back_fun;
                }//end while

                _venezia::c_variable y(in_variable);
                if(p_root_fun == nullptr){
                    y = _forward(y);
                }
                else{

                    do{
                        y = p_root_fun->_forward(y);
                        p_root_fun = p_root_fun->m_p_front_fun;
                    }while(p_root_fun != this);
                    y = _forward(y);
                }

                return  y;
            }
            _venezia::c_function& operator()( c_function & f)
            {
                m_p_back_fun = &f;
                f.m_p_front_fun = this;
                return *this;
            }
            _venezia::c_function& operator()()
            {
                return *this;
            }


        protected:
            virtual _venezia::c_variable _forward(const _venezia::c_variable  & data)
            {
                return data;
            }
        protected:
            c_function *m_p_back_fun;
            c_function *m_p_front_fun;
    };
}