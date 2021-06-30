#pragma once
#include <memory>
#include <list>
#include <string>
#include <typeinfo>

#include <inc/c_var_base.h>

namespace _venezia
{
    /**
     * @brief interface
     * Don't use this class in external.
     * this class must be used in c_fun_base_t template class only.
     * 
     */
    class _c_fun_base
    {
        public:
            typedef std::shared_ptr<_c_fun_base>     type_ptr;

        public:
            _c_fun_base()
            {}
            virtual ~_c_fun_base()
            {}

            _c_fun_base( const _c_fun_base & f)
            {
                //deep copy
                std::for_each( f.m_list_ptr_override_fun.cbegin(),f.m_list_ptr_override_fun.cend(),[&](const _c_fun_base::type_ptr &ptr_fun){
                    if(ptr_fun){
                        m_list_ptr_override_fun.push_back(_c_fun_base::type_ptr(
                            new _c_fun_base(*ptr_fun)
                        ));
                    }
                });
            }

            c_var_base operator()(const c_var_base & in_variable )
            {
                return _forward(in_variable);
            }

            _c_fun_base::type_ptr operator()() const
            {
                _c_fun_base::type_ptr ptr_new_fun(_get_new_instance());
                return ptr_new_fun;
            }

            _c_fun_base::type_ptr operator()( const _c_fun_base::type_ptr & ptr_f_src) const
            {
                _c_fun_base::type_ptr ptr_new_fun(_get_raw_new_instance());

                if(ptr_new_fun){
                    if(ptr_f_src){
                        if(ptr_f_src->m_list_ptr_override_fun.empty()){
                            ptr_new_fun->m_list_ptr_override_fun.push_back(ptr_f_src);
                        }
                        else{
                            ptr_new_fun->m_list_ptr_override_fun = ptr_f_src->m_list_ptr_override_fun;
                        }
                        ptr_new_fun->m_list_ptr_override_fun.push_back(_get_raw_new_instance());
                    }
                }
                return ptr_new_fun;
            }

            c_var_base backword(const c_var_base & in_variable)
            {
                return _default_backward(in_variable,m_var_in);
            }

            std::string info()
            {
                std::string s_info;
                s_info += '[';
                s_info +=typeid(this).name();
                s_info += "][";

                s_info += std::to_string(m_list_ptr_override_fun.size());

                s_info += ']';
                return s_info;
            }

            bool is_overload()
            {
                return !m_list_ptr_override_fun.empty();
            }
        protected:
            virtual c_var_base _forward(const c_var_base  & x)
            {
                do{
                    m_var_in = x;

                    if(m_list_ptr_override_fun.empty()){
                        m_var_out = _default_forward(x);
                        continue;
                    }

                    m_var_out = x;
                    std::for_each( m_list_ptr_override_fun.begin(),m_list_ptr_override_fun.end(),[&](_c_fun_base::type_ptr &ptr_fun){
                        if(ptr_fun){
                            m_var_out = ptr_fun->_forward(m_var_out);
                        }
                    });

                }while(false);
                return  m_var_out;
            }

            virtual c_var_base _default_forward(const c_var_base  & data)
            {
                return data;
            };
            virtual c_var_base _default_backward(const c_var_base  & gy,const c_var_base  & x)
            {
                return c_var_base(1);
            };


        public:
            virtual _c_fun_base::type_ptr _get_raw_new_instance() const
            {
                _c_fun_base::type_ptr ptr_f(nullptr);
                return ptr_f;
            }
            virtual _c_fun_base::type_ptr _get_new_instance() const
            {
                _c_fun_base::type_ptr ptr_f(nullptr);
                return ptr_f;
            }

        public:
            std::list<_c_fun_base::type_ptr> m_list_ptr_override_fun;
            c_var_base m_var_in,m_var_out;
    };

}

