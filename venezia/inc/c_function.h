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
                //deep copy
                m_list_ptr_override_fun.clear();

                std::for_each( f.m_list_ptr_override_fun.cbegin(),f.m_list_ptr_override_fun.cend(),[&](const c_fun_base::type_ptr &ptr_fun){
                    if(ptr_fun){
                        m_list_ptr_override_fun.push_back(c_fun_base::type_ptr(
                            new c_fun_base(*ptr_fun)
                        ));
                    }
                });
            }

            c_variable operator()(const c_variable & in_variable )
            {
                return _forward(in_variable);
            }

            c_fun_base::type_ptr operator()() const
            {
                c_fun_base::type_ptr ptr_new_fun(_get_new_instance());
                std::cout << "." <<ptr_new_fun->m_list_ptr_override_fun.size() << ".";
                return ptr_new_fun;
            }

            c_fun_base::type_ptr operator()( const c_fun_base::type_ptr & ptr_f_src) const
            {
                c_fun_base::type_ptr ptr_new_fun(_get_raw_new_instance());

                if(ptr_new_fun){
                    if(ptr_f_src){
                        if(ptr_f_src->m_list_ptr_override_fun.empty()){
                            ptr_new_fun->m_list_ptr_override_fun.push_back(ptr_f_src);
                        }
                        else{
                            ptr_new_fun->m_list_ptr_override_fun = ptr_f_src->m_list_ptr_override_fun;
                        }
                        ptr_new_fun->m_list_ptr_override_fun.push_back(_get_raw_new_instance());
                        std::cout << "_" <<ptr_new_fun->m_list_ptr_override_fun.size() << "_";
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
                    //out_var = _default_forward(out_var);

                }while(false);
                return  out_var;
            }

            virtual c_variable _default_forward(const c_variable  & data)
            {
                return data;
            };
        public:
            virtual c_fun_base::type_ptr _get_raw_new_instance() const
            {
                c_fun_base::type_ptr ptr_f(nullptr);
                return ptr_f;
            }
            virtual c_fun_base::type_ptr _get_new_instance() const
            {
                c_fun_base::type_ptr ptr_f(nullptr);
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
        c_fun() : c_fun_base()
        {}
        c_fun( const c_fun & f) : c_fun_base(f)
        {}

        c_fun& operator=( const c_fun & f)
        {
            //deep copy
            m_list_ptr_override_fun = f.m_list_ptr_override_fun;
            return *this;
        }

        c_fun& operator=( const c_fun_base::type_ptr & ptr_f_src)
        {
            //deep copy
            if(ptr_f_src){
                if(ptr_f_src->m_list_ptr_override_fun.empty()){
                    m_list_ptr_override_fun.push_back(ptr_f_src);
                }
                else{
                    m_list_ptr_override_fun = ptr_f_src->m_list_ptr_override_fun;
                }
            }
            std::cout << "="<< typeid(TT).name()<< m_list_ptr_override_fun.size() <<"=";
            return *this;
        }

        public:
            virtual c_fun_base::type_ptr _get_raw_new_instance() const
            {
                c_fun_base::type_ptr ptr_new_f( new TT() );
                std::cout << "(new "<< typeid(TT).name() <<")";
                return ptr_new_f;
            }

            virtual c_fun_base::type_ptr _get_new_instance() const
            {
                c_fun_base::type_ptr ptr_new_f( new TT() );
                ptr_new_f->m_list_ptr_override_fun = m_list_ptr_override_fun;

                std::cout << "{new "<< typeid(TT).name()<< ptr_new_f->m_list_ptr_override_fun.size() <<"}";
                return ptr_new_f;
            }
    };

}

