#pragma once
#include <memory>
#include <list>
#include <string>
#include <typeinfo>

#include <inc/c_fun_base.h>
#include <inc/c_variable.h>

namespace _venezia
{
    /**
     * @brief  c_fun_base_t template class
     * this class is the base class of all function class.
     * 
     * using. how to create a function class.
     * 
     * 1. inherit c_fun_base_t class
     * 2. add "using keyword"
     *      public:
     *      using _venezia::_c_fun_base::operator();
     *      using _venezia::c_fun_base_t<your class namse>::operator=;
     * 3. overload _default_forward() function.
     * 
     * example> If you design the unit function,(mathematically y=x) 
     *      It is as like below
     *      class cf_unit : public _venezia::c_fun_base_t<cf_unit>
     *      {
     *          public:
     *          using _venezia::_c_fun_base::operator();
     *          using _venezia::c_fun_base_t<cf_unit>::operator=;
     *          protected:
     *          virtual _venezia::c_var_base _default_forward(const _venezia::c_var_base & x)
     *          {
     *              return x; //y=x function
     *          }
     *      };
     */
    template <typename  TT>
    class c_fun_base_t : public _c_fun_base
    {
        public:
        virtual ~c_fun_base_t(){}
        c_fun_base_t() : _c_fun_base()
        {}
        c_fun_base_t( const c_fun_base_t & f) : _c_fun_base(f)
        {}

        c_variable operator()(const c_variable & in_variable )
        {
            c_variable out = _c_fun_base::_forward(in_variable);
            out.set_creator(*this);
            return out;
        }

        _c_fun_base::type_ptr operator()() const
        {
            return _c_fun_base::operator()();
        }

        _c_fun_base::type_ptr operator()( const _c_fun_base::type_ptr & ptr_f_src) const
        {
            return _c_fun_base::operator()(ptr_f_src);
        }

        c_fun_base_t& operator=( const c_fun_base_t & f)
        {
            m_list_ptr_override_fun = f.m_list_ptr_override_fun;
            return *this;
        }

        c_fun_base_t& operator=( const _c_fun_base::type_ptr & ptr_f_src)
        {
            if(ptr_f_src){
                if(ptr_f_src->m_list_ptr_override_fun.empty()){
                    m_list_ptr_override_fun.push_back(ptr_f_src);
                }
                else{
                    m_list_ptr_override_fun = ptr_f_src->m_list_ptr_override_fun;
                }
            }
            return *this;
        }

        public:
            virtual _c_fun_base::type_ptr _get_raw_new_instance() const
            {
                _c_fun_base::type_ptr ptr_new_f( new TT() );
                return ptr_new_f;
            }

            virtual _c_fun_base::type_ptr _get_new_instance() const
            {
                _c_fun_base::type_ptr ptr_new_f( new TT() );
                ptr_new_f->m_list_ptr_override_fun = m_list_ptr_override_fun;
                return ptr_new_f;
            }
    };

}

