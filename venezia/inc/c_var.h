#pragma once

#include <memory>

#include <utility>
#include <Eigen/Dense>
#include <inc/c_fun.h>

namespace _venezia
{
    class c_var : public _c_var
    {
        public:
            typedef std::shared_ptr<c_var>  type_ptr;
            typedef std::pair<size_t,size_t>    type_size;
        protected:
            typedef std::shared_ptr<Eigen::MatrixXd>  type_ptr_mt;
        public:
        c_var()
        {
            m_ptr_mt_data=nullptr;
            m_ptr_mt_grad = nullptr;
        }
        c_var( int d )
        {
            m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
            *m_ptr_mt_data << d;
        }
        c_var( double d )
        {
            m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(1,1));
            *m_ptr_mt_data << d;
        }

        c_var( const Eigen::MatrixXd & in )
        {
            m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(in));
        }

        c_var(EIGEN_DEFAULT_DENSE_INDEX_TYPE n_row, EIGEN_DEFAULT_DENSE_INDEX_TYPE n_col)
        {
            m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(n_row,n_col));
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
            if(rhs.m_ptr_mt_grad){
                m_ptr_mt_grad = c_var::type_ptr_mt(new Eigen::MatrixXd(*(rhs.m_ptr_mt_grad)));
            }
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
        bool empty_gradient()
        {
            if( !m_ptr_mt_grad )
                return true;
            else
                return false;
        }

        void set(const Eigen::MatrixXd & data)
        {
            m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(data));
        }

        void set(size_t n_row, size_t n_col, int n_val)
        {
            m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(n_row,n_col));
           (*m_ptr_mt_data)(n_row,n_col) = n_val;
        }
        void set(size_t n_row, size_t n_col, long n_val)
        {
            m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(n_row,n_col));
           (*m_ptr_mt_data)(n_row,n_col) = n_val;
        }
        void set(size_t n_row, size_t n_col, float n_val)
        {
           m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(n_row,n_col));
           (*m_ptr_mt_data)(n_row,n_col) = n_val;
        }
        void set(size_t n_row, size_t n_col, double n_val)
        {
           m_ptr_mt_data = c_var::type_ptr_mt(new Eigen::MatrixXd(n_row,n_col));
           (*m_ptr_mt_data)(n_row,n_col) = n_val;
        }

        virtual void set_gradient(const c_var & v)
        {
            if(v.m_ptr_mt_grad){
                m_ptr_mt_grad = c_var::type_ptr_mt(new Eigen::MatrixXd(*(v.m_ptr_mt_grad)));
            }
            else{
                m_ptr_mt_grad = v.m_ptr_mt_grad;
            }
        }

        Eigen::MatrixXd get() const
        {
            if(m_ptr_mt_data)
                return *m_ptr_mt_data;
            else
                Eigen::MatrixXd();
        }
        c_var::type_size size() const
        {
            if(m_ptr_mt_data)
                return std::make_pair(m_ptr_mt_data->rows(),m_ptr_mt_data->cols());
            else
                return std::make_pair(0,0);
        }

        std::string string()
        {
            if(m_ptr_mt_data){
                std::stringstream ss;
                ss << *m_ptr_mt_data;
                return ss.str();
            }
            else{
                return std::string();
            }
        }

        virtual bool empty()
        {
            if( !m_ptr_mt_data )
                return true;
            else
                return false;
        }

        virtual void set_creator( _venezia::c_fun *p_creator)
        {
            m_p_creator = p_creator;
        }

        virtual bool backword()
        {
            bool b_result(false);
            do{
                if(m_p_creator==nullptr)
                    continue;
                if(!m_ptr_mt_data)
                    continue;
                if(!m_ptr_mt_grad)
                    continue;
                //
                b_result = m_p_creator->backword(*m_ptr_mt_grad);
            }while(false);
            return b_result;
        }
        protected:
            c_var::type_ptr_mt m_ptr_mt_data,m_ptr_mt_grad;
            _venezia::c_fun *m_p_creator = nullptr;

    };
}