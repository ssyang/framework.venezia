#pragma once
#include <memory>
#include <list>

#include <inc/c_variable.h>

namespace _venezia
{
    /**
     * @brief ths base class of all function 
     * this is virtual class
     * 
     */
    template <class T>
    class c_function
    {
        public:
            typedef std::shared_ptr<c_function>     type_ptr_c_function;
            typedef std::list<type_ptr_c_function>  type_list_ptr_c_function;
            typedef std::shared_ptr<type_list_ptr_c_function> type_ptr_list_ptr_c_function;

        public:
            c_function()
            {}

            c_function( const c_function & f)
            {
                m_ptr_list_ptr_override_fun = f.m_ptr_list_ptr_override_fun;
            }

            c_function & operator=(const c_function & f)
            {
                m_ptr_list_ptr_override_fun = f.m_ptr_list_ptr_override_fun;
                return *this;
            }
            c_function & operator=(const type_ptr_list_ptr_c_function & list_ptr_fun)
            {
                m_ptr_list_ptr_override_fun = list_ptr_fun;
                return *this;
            }

            _venezia::c_variable operator()(const _venezia::c_variable & in_variable )
            {
                _venezia::c_variable out_var;
                do{
                    if(!m_ptr_list_ptr_override_fun){
                        out_var = _default_forward(in_variable);
                        continue;
                    }
                    if(m_ptr_list_ptr_override_fun->empty()){
                        out_var = _default_forward(in_variable);
                        continue;
                    }

                    out_var = in_variable;
                    std::for_each( m_ptr_list_ptr_override_fun->begin(),m_ptr_list_ptr_override_fun->end(),[&](type_ptr_c_function &ptr_fun){
                        out_var = (*ptr_fun)(out_var);
                    });


                }while(false);
                return  out_var;
            }

            type_ptr_list_ptr_c_function operator()( const type_ptr_list_ptr_c_function & ptr_list_ptr_fun)
            {
                type_ptr_list_ptr_c_function ptr_list_ptr_c_function=ptr_list_ptr_fun;

                if(!ptr_list_ptr_c_function){
                    ptr_list_ptr_c_function = type_ptr_list_ptr_c_function( new type_list_ptr_c_function() );
                }
                type_ptr_c_function ptr_c_function( new T() );
                ptr_list_ptr_c_function->push_back(ptr_c_function);
                return ptr_list_ptr_c_function;
            }
            type_ptr_list_ptr_c_function operator()()
            {
                type_ptr_list_ptr_c_function ptr_list_ptr_c_function( new type_list_ptr_c_function() );
                type_ptr_c_function ptr_c_function( new T() );
                ptr_list_ptr_c_function->push_back(ptr_c_function);
                return ptr_list_ptr_c_function;
            }

        protected:
            virtual _venezia::c_variable _default_forward(const _venezia::c_variable  & data) = 0;
        protected:
            type_ptr_list_ptr_c_function m_ptr_list_ptr_override_fun;
    };
}