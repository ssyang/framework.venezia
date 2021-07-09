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

        public:
            _venezia::c_fun::type_list_ptr operator()(_venezia::c_fun::type_list_ptr & list_ptr)
            {
                if(m_list_ptr_overload.empty()){
                    list_ptr.push_back(c_fun::type_ptr((c_fun*)_new_instance()));
                    return c_fun::type_list_ptr();
                }
                else{
                    std::copy(m_list_ptr_overload.begin(),m_list_ptr_overload.end(),std::back_inserter(list_ptr));
                    return list_ptr;
                }
            }

            _venezia::c_fun::type_list_ptr operator()()
            {
                if(m_list_ptr_overload.empty()){
                    c_fun::type_list_ptr list_ol;
                    list_ol.push_back(c_fun::type_ptr((c_fun*)_new_instance()));
                    return c_fun::type_list_ptr();
                }
                else{
                    return m_list_ptr_overload;
                }
            }

            _venezia::c_var operator()(const _venezia::c_var & in)
            {
                if(in.empty()){
                    _venezia::c_var y(true);
                    return y;//error case
                }
                else{
                    m_x = in;
                    m_y = _default_forward(in);
                    m_y.set_creator(this);
                    return m_y;
                }
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