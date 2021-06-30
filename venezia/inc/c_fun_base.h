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

            c_var_base backword(const c_var_base & in_variable)
            {
                return _default_backward(in_variable,m_var_in);
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
            c_var_base _forward(const c_var_base  & x)
            {
                c_var_base out_var;
                do{
                    m_var_in = x;

                    if(m_list_ptr_override_fun.empty()){
                        out_var = _default_forward(x);
                        continue;
                    }

                    out_var = x;
                    std::for_each( m_list_ptr_override_fun.begin(),m_list_ptr_override_fun.end(),[&](_c_fun_base::type_ptr &ptr_fun){
                        if(ptr_fun){
                            out_var = ptr_fun->_forward(out_var);
                        }
                    });

                }while(false);
                return  out_var;
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
            c_var_base m_var_in;
    };

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
            //std::cout << "="<< typeid(TT).name()<< m_list_ptr_override_fun.size() <<"=";
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

