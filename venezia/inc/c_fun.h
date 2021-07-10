#pragma once

#include <typeinfo>
#include <memory>
#include <list>

#include <inc/_c_fun.h>
#include <inc/c_var.h>

namespace _venezia
{
   
    class c_fun : public _c_fun
    {
        protected:
            virtual const _venezia::c_var& _default_forward(const _venezia::c_var & p_in) = 0;
            virtual const _venezia::c_var& _default_backward(const _venezia::c_var  & gy,const _venezia::c_var  & x) =0 ;
            virtual void *_new_instance()=0;

        public:
            typedef std::shared_ptr<c_fun>  type_ptr;
            typedef std::list<c_fun::type_ptr>  type_list_ptr;
            typedef std::shared_ptr<c_fun::type_list_ptr>  type_ptr_list_ptr;
        public:
            const _venezia::c_fun::type_ptr_list_ptr operator()(const _venezia::c_fun::type_ptr_list_ptr & ptr_list_ptr)
            {
                c_fun::type_ptr_list_ptr ptr_tmp(ptr_list_ptr);
                if(!ptr_tmp){
                    ptr_tmp = c_fun::type_ptr_list_ptr(new c_fun::type_list_ptr());
                }
                if(m_list_ptr_overload.empty()){
                    ptr_tmp->push_back(c_fun::type_ptr((c_fun*)_new_instance()));
                }
                else{
                    std::copy(m_list_ptr_overload.begin(),m_list_ptr_overload.end(),std::back_inserter(*ptr_tmp));
                }
                return ptr_tmp;
            }

            _venezia::c_fun::type_ptr_list_ptr operator()()
            {
                c_fun::type_ptr_list_ptr  ptr_list_ol(new c_fun::type_list_ptr());
                if(m_list_ptr_overload.empty()){
                    ptr_list_ol->push_back(c_fun::type_ptr((c_fun*)_new_instance()));
                }
                else{
                    *ptr_list_ol = m_list_ptr_overload;
                }
                return ptr_list_ol;
            }

            _venezia::c_var operator()(const _venezia::c_var & in)
            {
                m_y.set_error(false);

                if(in.empty()){
                    _venezia::c_var y(true);
                    return y;//error case
                }
                else{
                    m_x = in;

                    if(m_list_ptr_overload.empty()){
                        m_y = _default_forward(in);
                    }
                    else{
                        _venezia::c_var tmp(in);
                        for( auto ptr_item : m_list_ptr_overload){
                            if(ptr_item){
                                tmp = (*ptr_item)(tmp);
                                if(tmp.error()){
                                    break;
                                }
                            }
                        }//end for
                        m_y = tmp;
                    }
                    m_y.set_creator(this);
                    return m_y;
                }
            }
            _venezia::c_fun& operator=(const _venezia::c_fun::type_ptr_list_ptr & ptr_list_ptr)
            {
                if(ptr_list_ptr)
                    m_list_ptr_overload = *ptr_list_ptr;
                return *this;
            }

            virtual bool backword(const Eigen::MatrixXd & mt_dy)
            {
                bool b_result(false);
                do{
                    if( m_x.empty() ){
                        continue;
                    }
                    _venezia::c_var dy(mt_dy);
                    _venezia::c_var dx = _default_backward(dy,m_x);
                    m_x.set_gradient(dx);
                    b_result = m_x.backword();
                }while(false);
                return b_result;
            }

            std::string string(bool b_insert_enter=false)
            {
                std::string s_info;
                s_info = "name = ";
                s_info += typeid(*this).name();
                if(b_insert_enter){
                    s_info = +"\n";
                }
                else{
                    s_info = +" , ";
                }

                s_info += " x = ";
                s_info += m_x.string();
                if(b_insert_enter){
                    s_info = +"\n";
                }
                else{
                    s_info = +" , ";
                }
                s_info += " y = ";
                s_info += m_y.string();
                return s_info;
            }

        protected:
            _venezia::c_var m_x,m_y;  
            type_list_ptr m_list_ptr_overload;
    };
}