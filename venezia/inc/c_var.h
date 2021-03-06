#pragma once

#include <memory>

#include <utility>
#include <Eigen/Dense>
#include <inc/_c_fun.h>

namespace _venezia
{
    class c_var
    {
        public:
            typedef std::shared_ptr<c_var>  type_ptr;
            typedef std::pair<size_t,size_t>    type_size;
        protected:
            typedef std::shared_ptr<Eigen::MatrixXd>  type_ptr_mt;
        public:

        virtual bool empty(bool b_gradient = false) const
        {
            if(b_gradient){
                if( !m_ptr_mt_grad )
                    return true;
                else
                    return false;
            }
            else{
                if( !m_ptr_mt_data )
                    return true;
                else
                    return false;
            }
        }
        virtual void set_gradient(const c_var & v)
        {
            m_ptr_mt_grad = c_var::type_ptr_mt(new Eigen::MatrixXd(v.get()));
        }
        virtual void set_gradient(const Eigen::MatrixXd & v)
        {
            m_ptr_mt_grad = c_var::type_ptr_mt(new Eigen::MatrixXd(v));
        }
        virtual void set_creator( _venezia::_c_fun *p_creator)
        {
            m_p_creator = p_creator;
        }
        virtual bool backword()
        {
            bool b_result(false);
            do{
                if(!m_ptr_mt_data)
                    continue;
                //
                b_result = true;
                if(m_p_creator==nullptr)
                    continue;
                //
                c_var::type_size size_in = m_p_creator->get_size_of_input();
                if(size_in.first == 0 && size_in.second == 0){
                    continue;
                }
                if(!m_ptr_mt_grad){
                    m_ptr_mt_grad = c_var::type_ptr_mt(
                        new Eigen::MatrixXd(size_in.first,size_in.second)
                        );
                    m_ptr_mt_grad->fill(1);
                }

                c_var grad(*m_ptr_mt_grad);
                b_result = m_p_creator->backword((void *)&grad);
            }while(false);
            return b_result;
        }

        c_var get_gradient() const
        {
            if(m_ptr_mt_grad){
                return c_var(*m_ptr_mt_grad);
            }
            else{
                return c_var();
            }
        }

        public:
        c_var()
        {
            _ini();
        }

        c_var( const std::string & s_name)
        {
            _ini();
            m_s_name_for_debug = s_name;
        }

        c_var(bool b_error)
        {
            _ini();
            m_b_error = b_error;
        }

        c_var( int d )
        {
            m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
            *m_ptr_mt_data << d;
            m_b_error = false;
        }
        c_var( double d )
        {
            m_b_error = false;
            m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
            *m_ptr_mt_data << d;
        }

        c_var( const Eigen::MatrixXd & in )
        {
            m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(in));
            m_b_error = false;
        }

        c_var(EIGEN_DEFAULT_DENSE_INDEX_TYPE n_row, EIGEN_DEFAULT_DENSE_INDEX_TYPE n_col)
        {
            m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(n_row,n_col));
            m_b_error = false;
        }
        c_var( const c_var& rhs )
        {
            *this = rhs;
        }

        virtual ~c_var(){}

        c_var& operator=(const c_var &rhs)
        {
            if(rhs.m_ptr_mt_data){
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(*(rhs.m_ptr_mt_data)));
            }
            else{
                m_ptr_mt_data.reset();
            }
            if(rhs.m_ptr_mt_grad){
                m_ptr_mt_grad = c_var::type_ptr_mt(new Eigen::MatrixXd(*(rhs.m_ptr_mt_grad)));
            }
            else{
                m_ptr_mt_grad.reset();
            }

            m_b_error = rhs.m_b_error;
            m_p_creator = rhs.m_p_creator;
            return *this;
        }
        ////////////////////////////
        /*
        * matrix operators
        */
        c_var& operator=(const Eigen::MatrixXd& rhs)
        {
            m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(rhs));
            return *this;
        }

        c_var& operator+=(const c_var& rhs)
        {
            if(m_ptr_mt_data){
                *m_ptr_mt_data += rhs.get();
            }
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(*(rhs.m_ptr_mt_data)));
            }
            return *this;
        }
        c_var& operator-=(const c_var& rhs)
        {
            if(m_ptr_mt_data){
                *m_ptr_mt_data -= rhs.get();
            }
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(-(*(rhs.m_ptr_mt_data))));
            }
            return *this;
        }

        friend c_var operator+( c_var lhs, const c_var& rhs )
        {
            lhs += rhs;
            return lhs;
        }
        friend c_var operator-( c_var lhs, const c_var& rhs )
        {
            lhs -= rhs;
            return lhs;
        }

        /*
        * multiply each item of matrix
        */
        c_var& operator*=(const c_var& rhs)
        {
            do{
                size_t n_row(0),n_col(0);

                if(!m_ptr_mt_data)
                    continue;
                if(!rhs.m_ptr_mt_data)
                    continue;

                if(m_ptr_mt_data->rows() > rhs.get().rows()){
                    n_row = rhs.get().rows();
                }
                else{
                    n_row = m_ptr_mt_data->rows();
                }

                if(m_ptr_mt_data->cols() > rhs.get().cols()){
                    n_col = rhs.get().cols();
                }
                else{
                    n_col = m_ptr_mt_data->cols();
                }

                Eigen::MatrixXd matrix_new(n_row,n_col);

                for( size_t i=0; i<n_row; i++){
                    for( size_t j=0; j<n_col; j++){
                        matrix_new(i,j) = (*m_ptr_mt_data)(i,j) * rhs.get()(i,j);
                    }
                }//end for

                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(matrix_new));
            }while(false);
            return *this;
        }

        friend c_var operator*( c_var lhs, const c_var& rhs )
        {
            lhs *= rhs;
            return lhs;
        }

        /*
        * scala operator
        */
       //int
        c_var& operator=(const int& rhs)
        {
            if(m_ptr_mt_data)
                m_ptr_mt_data->fill(rhs);
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                *m_ptr_mt_data << rhs;
            }
            return *this;
        }
        c_var& operator+=(const int& rhs)
        {
            if(m_ptr_mt_data){
                Eigen::MatrixXd r(*m_ptr_mt_data);
                r.fill(rhs);
                (*m_ptr_mt_data) += r;
            }
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                *m_ptr_mt_data << rhs;
            }
            return *this;
        }
        friend c_var operator+( c_var lhs, const int& rhs )
        {
            lhs += rhs;
            return lhs;
        }
        friend c_var operator+( const int& lhs, c_var rhs )
        {
            rhs += lhs;
            return rhs;
        }

        c_var& operator-=(const int& rhs)
        {
            if(m_ptr_mt_data){
                Eigen::MatrixXd r(*m_ptr_mt_data);
                r.fill(rhs);
                (*m_ptr_mt_data) -= r;
            }
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                *m_ptr_mt_data << -rhs;
            }
            return *this;
        }
        friend c_var operator-( c_var lhs, const int& rhs )
        {
            lhs -= rhs;
            return lhs;
        }
        friend c_var operator-( const int& lhs, c_var rhs )
        {
            rhs -= lhs;
            return rhs;
        }
 
        c_var& operator*=(const int& rhs)
        {
            if(m_ptr_mt_data)
                (*m_ptr_mt_data) *= rhs;
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                *m_ptr_mt_data << 0;
            }
            return *this;
        }
        friend c_var operator*( c_var lhs, const int& rhs )
        {
            lhs *= rhs;
            return lhs;
        }
        friend c_var operator*( const int& lhs, c_var rhs )
        {
            rhs *= lhs;
            return rhs;
        }

        c_var& operator/=(const int& rhs)
        {
            if(m_ptr_mt_data)
                (*m_ptr_mt_data) /= rhs;
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                *m_ptr_mt_data << 0;
            }
            return *this;
        }
        friend c_var operator/( c_var lhs, const int& rhs )
        {
            lhs /= rhs;
            return lhs;
        }
        friend c_var operator/( const int& lhs, c_var rhs )
        {
            rhs /= lhs;
            return rhs;
        }

        //long
        c_var& operator=(const long& rhs)
        {
            if(m_ptr_mt_data)
                m_ptr_mt_data->fill(rhs);
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                *m_ptr_mt_data << rhs;
            }
            return *this;
        }
        c_var& operator+=(const long& rhs)
        {
            if(m_ptr_mt_data){
                Eigen::MatrixXd r(*m_ptr_mt_data);
                r.fill(rhs);
                (*m_ptr_mt_data) += r;
            }
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                *m_ptr_mt_data << rhs;
            }
            return *this;
        }
        friend c_var operator+( c_var lhs, const long& rhs )
        {
            lhs += rhs;
            return lhs;
        }
        friend c_var operator+( const long& lhs, c_var rhs )
        {
            rhs += lhs;
            return rhs;
        }

        c_var& operator-=(const long& rhs)
        {
            if(m_ptr_mt_data){
                Eigen::MatrixXd r(*m_ptr_mt_data);
                r.fill(rhs);
                (*m_ptr_mt_data) -= r;
            }
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                *m_ptr_mt_data << -rhs;
            }
            return *this;
        }
        friend c_var operator-( c_var lhs, const long& rhs )
        {
            lhs -= rhs;
            return lhs;
        }
        friend c_var operator-( const long& lhs, c_var rhs )
        {
            rhs -= lhs;
            return rhs;
        }
 
        c_var& operator*=(const long& rhs)
        {
            if(m_ptr_mt_data){
                Eigen::MatrixXd r(*m_ptr_mt_data);
                r.fill(rhs);
                (*m_ptr_mt_data) *= r;
            }
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                *m_ptr_mt_data << 0;
            }
            return *this;
        }
        friend c_var operator*( c_var lhs, const long& rhs )
        {
            lhs *= rhs;
            return lhs;
        }
        friend c_var operator*( const long& lhs, c_var rhs )
        {
            rhs *= lhs;
            return rhs;
        }

        c_var& operator/=(const long& rhs)
        {
            if(m_ptr_mt_data){
                if( rhs!=0){
                    (*m_ptr_mt_data) /= rhs;
                }
            }
            else{
                if( rhs != 0 ){
                    m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                    *m_ptr_mt_data << 0;
                }
            }
            return *this;
        }
        friend c_var operator/( c_var lhs, const long& rhs )
        {
            lhs /= rhs;
            return lhs;
        }
        friend c_var operator/( const long& lhs, c_var rhs )
        {
            rhs /= lhs;
            return rhs;
        }

        //float
        c_var& operator=(const float& rhs)
        {
            if(m_ptr_mt_data)
                m_ptr_mt_data->fill(rhs);
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                *m_ptr_mt_data << rhs;
            }
            return *this;
        }
        c_var& operator+=(const float& rhs)
        {
            if(m_ptr_mt_data){
                Eigen::MatrixXd r(*m_ptr_mt_data);
                r.fill(rhs);
                (*m_ptr_mt_data) += r;
            }
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                *m_ptr_mt_data << rhs;
            }
            return *this;
        }
        friend c_var operator+( c_var lhs, const float& rhs )
        {
            lhs += rhs;
            return lhs;
        }
        friend c_var operator+( const float& lhs, c_var rhs )
        {
            rhs += lhs;
            return rhs;
        }

        c_var& operator-=(const float& rhs)
        {
            if(m_ptr_mt_data){
                Eigen::MatrixXd r(*m_ptr_mt_data);
                r.fill(rhs);
                (*m_ptr_mt_data) -= r;
            }
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                *m_ptr_mt_data << -rhs;
            }
            return *this;
        }
        friend c_var operator-( c_var lhs, const float& rhs )
        {
            lhs -= rhs;
            return lhs;
        }
        friend c_var operator-( const float& lhs, c_var rhs )
        {
            rhs -= lhs;
            return rhs;
        }
 
        c_var& operator*=(const float& rhs)
        {
            if(m_ptr_mt_data){
                Eigen::MatrixXd r(*m_ptr_mt_data);
                r.fill(rhs);
                (*m_ptr_mt_data) *= r;
            }
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                *m_ptr_mt_data << 0;
            }
            return *this;
        }
        friend c_var operator*( c_var lhs, const float& rhs )
        {
            lhs *= rhs;
            return lhs;
        }
        friend c_var operator*( const float& lhs, c_var rhs )
        {
            rhs *= lhs;
            return rhs;
        }

        c_var& operator/=(const float& rhs)
        {
            if(m_ptr_mt_data){
                if( rhs!=0){
                    (*m_ptr_mt_data) /= rhs;
                }
            }
            else{
                if( rhs != 0 ){
                    m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                    *m_ptr_mt_data << 0;
                }
            }
            return *this;
        }
        friend c_var operator/( c_var lhs, const float& rhs )
        {
            lhs /= rhs;
            return lhs;
        }
        friend c_var operator/( const float& lhs, c_var rhs )
        {
            rhs /= lhs;
            return rhs;
        }

        //double
        c_var& operator=(const double& rhs)
        {
            if(m_ptr_mt_data)
                m_ptr_mt_data->fill(rhs);
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                *m_ptr_mt_data << rhs;
            }
            return *this;
        }
        c_var& operator+=(const double& rhs)
        {
            if(m_ptr_mt_data){
                Eigen::MatrixXd r(*m_ptr_mt_data);
                r.fill(rhs);
                (*m_ptr_mt_data) += r;
            }
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                *m_ptr_mt_data << rhs;
            }
            return *this;
        }
        friend c_var operator+( c_var lhs, const double& rhs )
        {
            lhs += rhs;
            return lhs;
        }
        friend c_var operator+( const double& lhs, c_var rhs )
        {
            rhs += lhs;
            return rhs;
        }

        c_var& operator-=(const double& rhs)
        {
            if(m_ptr_mt_data){
                Eigen::MatrixXd r(*m_ptr_mt_data);
                r.fill(rhs);
                (*m_ptr_mt_data) -= r;
            }
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                *m_ptr_mt_data << -rhs;
            }
            return *this;
        }
        friend c_var operator-( c_var lhs, const double& rhs )
        {
            lhs -= rhs;
            return lhs;
        }
        friend c_var operator-( const double& lhs, c_var rhs )
        {
            rhs -= lhs;
            return rhs;
        }
 
        c_var& operator*=(const double& rhs)
        {
            if(m_ptr_mt_data){
                Eigen::MatrixXd r(*m_ptr_mt_data);
                r.fill(rhs);
                (*m_ptr_mt_data) *= r;
            }
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                *m_ptr_mt_data << 0;
            }
            return *this;
        }
        friend c_var operator*( c_var lhs, const double& rhs )
        {
            lhs *= rhs;
            return lhs;
        }
        friend c_var operator*( const double& lhs, c_var rhs )
        {
            rhs *= lhs;
            return rhs;
        }

        c_var& operator/=(const double& rhs)
        {
            if(m_ptr_mt_data){
                if( rhs!=0){
                    (*m_ptr_mt_data) /= rhs;
                }
            }
            else{
                if( rhs != 0 ){
                    m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
                    *m_ptr_mt_data << 0;
                }
            }
            return *this;
        }
        friend c_var operator/( c_var lhs, const double& rhs )
        {
            lhs /= rhs;
            return lhs;
        }
        friend c_var operator/( const double& lhs, c_var rhs )
        {
            rhs /= lhs;
            return rhs;
        }

        const Eigen::MatrixXd operator()() const
        {
            if(m_ptr_mt_data){
                return *m_ptr_mt_data;
            }
            else{
                return Eigen::MatrixXd();
            }

        }
        // matirix multiplication
        c_var mul(const Eigen::MatrixXd &rhs)
        {
            c_var result;
            do{
                if(!m_ptr_mt_data)
                    continue;

                size_t n_row(0),n_col(0);

                if(m_ptr_mt_data->rows() == 0 ){
                    continue;
                }
                if(m_ptr_mt_data->cols() == 0 ){
                    continue;
                }
                if(rhs.rows() == 0 ){
                    continue;
                }
                if(rhs.cols() == 0 ){
                    continue;
                }

                if(m_ptr_mt_data->cols() == rhs.rows()){
                    //(M1xN) * (NxM2) = (M1xM2)
                    result = (*m_ptr_mt_data)*rhs;
                    continue;
                }

                if(m_ptr_mt_data->rows() == 1){
                    if(rhs.rows() ==1 ){
                        if(m_ptr_mt_data->cols() == rhs.cols() ){
                            //(1xN) * (1xN) -> (1xN) * (Nx1) = (1X1)
                            result = (*m_ptr_mt_data) * rhs.transpose();
                            continue;
                        }
                        //(1xN) * (1xM) , N != M
                        continue;
                    }
                    //(1xN) * (MxN)
                    continue;
                }

                if(rhs.rows() ==1 ){
                    if(m_ptr_mt_data->cols() == rhs.cols() ){
                        //(MxN) * (1xN) -> (MxN) * (Nx1) = (Mx1)
                        result = (*m_ptr_mt_data) * rhs.transpose();
                        continue;
                    }
                    //(MxN1) * (1xN2)
                    continue;
                }
                //(M1xN1) * (M2xN2)
                continue;
            }while(false);
            return result;
        }
        c_var mul(const c_var &rhs)
        {
            return mul(rhs.get());
        }

        ///////////////////////////
        void set(const Eigen::MatrixXd & data)
        {
            m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(data));
        }

        void set(size_t n_row, size_t n_col, int n_val)
        {
            m_b_error = false;
            if(m_ptr_mt_data){
                if( m_ptr_mt_data->rows()>n_row && m_ptr_mt_data->cols() > n_col ){
                    (*m_ptr_mt_data)(n_row,n_col) = n_val;
                }
                else{
                    m_b_error = true;
                }
            }
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(n_row,n_col));
                (*m_ptr_mt_data)(n_row,n_col) = n_val;
            }
        }
        void set(size_t n_row, size_t n_col, long n_val)
        {
            m_b_error = false;
            if(m_ptr_mt_data){
                if( m_ptr_mt_data->rows()>n_row && m_ptr_mt_data->cols() > n_col ){
                    (*m_ptr_mt_data)(n_row,n_col) = n_val;
                }
                else{
                    m_b_error = true;
                }
            }
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(n_row,n_col));
                (*m_ptr_mt_data)(n_row,n_col) = n_val;
            }
        }
        void set(size_t n_row, size_t n_col, float n_val)
        {
            m_b_error = false;
            if(m_ptr_mt_data){
                if( m_ptr_mt_data->rows()>n_row && m_ptr_mt_data->cols() > n_col ){
                    (*m_ptr_mt_data)(n_row,n_col) = n_val;
                }
                else{
                    m_b_error = true;
                }
            }
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(n_row,n_col));
                (*m_ptr_mt_data)(n_row,n_col) = n_val;
            }
        }
        void set(size_t n_row, size_t n_col, double n_val)
        {
            m_b_error = false;
            if(m_ptr_mt_data){
                if( m_ptr_mt_data->rows()>n_row && m_ptr_mt_data->cols() > n_col ){
                    (*m_ptr_mt_data)(n_row,n_col) = n_val;
                }
                else{
                    m_b_error = true;
                }
            }
            else{
                m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(n_row,n_col));
                (*m_ptr_mt_data)(n_row,n_col) = n_val;
            }
        }

        Eigen::MatrixXd get() const
        {
            if(m_ptr_mt_data)
                return *m_ptr_mt_data;
            else
                return Eigen::MatrixXd();
        }
        c_var::type_size size() const
        {
            if(m_ptr_mt_data)
                return std::make_pair(m_ptr_mt_data->rows(),m_ptr_mt_data->cols());
            else
                return std::make_pair(0,0);
        }

        std::string string(bool b_gradient = false)
        {
            std::stringstream ss;

            if(b_gradient){
                if(m_ptr_mt_grad){
                    ss << *m_ptr_mt_grad;
                    return ss.str();
                }
                else{
                    return std::string();
                }
            }
            else{
                if(m_ptr_mt_data){
                    ss << *m_ptr_mt_data;
                    return ss.str();
                }
                else{
                    return std::string();
                }
            }
        }
        bool error() const
        {
            return m_b_error;
        }
        void set_error(bool b_error)
        {
            m_b_error = b_error;
        }
        void set_name( const std::string & s_name)
        {
            m_s_name_for_debug = s_name;
        }

        protected:
        void _ini()
        {
            m_ptr_mt_data=nullptr;
            m_ptr_mt_grad = nullptr;
            m_b_error = false;
        }

        protected:
            c_var::type_ptr_mt m_ptr_mt_data,m_ptr_mt_grad;
            _venezia::_c_fun *m_p_creator = nullptr;
            bool m_b_error;
            std::string m_s_name_for_debug;

    };
}