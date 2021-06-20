#pragma once
#include <memory>
#include <list>
#include <string>
#include <typeinfo>

#include <inc/c_variable.h>

namespace _venezia
{
    /**
     * @brief interface
     * Don't use this class in external
     * 
     */
    class c_fun_base
    {
        public:
            typedef std::shared_ptr<c_fun_base>     type_ptr;

        public:
            c_fun_base()
            {}
            virtual ~c_fun_base()
            {}

            c_fun_base( const c_fun_base & f)
            {
                m_list_ptr_override_fun = f.m_list_ptr_override_fun;
            }

            c_variable operator()(const c_variable & in_variable )
            {
                return _forward(in_variable);
            }

            c_fun_base operator()()
            {
                return *_get_new_instance();
            }
            c_fun_base operator()( c_fun_base & f)
            {
                f.m_list_ptr_override_fun.push_back(_get_new_instance());
                return f;
            }

        protected:
            c_variable _forward(const c_variable  & in_variable)
            {
                c_variable out_var;
                do{
                    if(m_list_ptr_override_fun.empty()){
                        out_var = _default_forward(in_variable);
                        continue;
                    }

                    out_var = in_variable;
                    std::for_each( m_list_ptr_override_fun.begin(),m_list_ptr_override_fun.end(),[&](c_fun_base::type_ptr &ptr_fun){
                        if(ptr_fun){
                            out_var = ptr_fun->_forward(out_var);
                        }
                    });
                    out_var = _default_forward(out_var);

                }while(false);
                return  out_var;
            }

            virtual c_variable _default_forward(const c_variable  & data)
            {
                return data;
            };
            
            virtual type_ptr _get_new_instance()
            {
                type_ptr ptr_f(nullptr);
                return ptr_f;
            }
        public:
            std::list<c_fun_base::type_ptr> m_list_ptr_override_fun;
    };

    template <typename  TT>
    class c_fun : public c_fun_base
    {
        public:
        virtual ~c_fun(){}
        c_fun& operator=( const c_fun & f)
        {
            m_list_ptr_override_fun = f.m_list_ptr_override_fun;
            return *this;
        }
        c_fun_base& operator=( const c_fun_base & f)
        {
            m_list_ptr_override_fun = f.m_list_ptr_override_fun;
            return *this;
        }
        protected:
            virtual type_ptr _get_new_instance()
            {
                return c_fun_base::type_ptr( new TT() );
            }
    };

}

