#pragma once

#include <utility>

#include <Eigen/Dense>

namespace _venezia
{
    /**
     * @brief ths base class of all variable 
     * 
     */
    class c_variable
    {
        public:
        c_variable() : m_b_last_error(false){}
        c_variable( const Eigen::MatrixXd & matrix_data ) : m_b_last_error(false)
        {
            m_matrix_data = matrix_data;
        }
        c_variable(EIGEN_DEFAULT_DENSE_INDEX_TYPE n_row, EIGEN_DEFAULT_DENSE_INDEX_TYPE n_col) :
            m_b_last_error(false),
            m_matrix_data(n_row,n_col)
        {
        }
        c_variable( const c_variable& rhs )
        {
            m_matrix_data = rhs.get();
            m_b_last_error = rhs.m_b_last_error;
        }

        ~c_variable(){}

        void set(const Eigen::MatrixXd & matrix_data)
        {
            m_matrix_data = matrix_data;
        }

        void set(size_t n_row, size_t n_col, int n_val)
        {
            do{
                if( n_row >= m_matrix_data.rows()){
                    continue;
                }
                if( n_col >= m_matrix_data.cols()){
                    continue;
                }
                m_matrix_data(n_row,n_col) = n_val;
            }while(false);
        }
        void set(size_t n_row, size_t n_col, long n_val)
        {
            do{
                if( n_row >= m_matrix_data.rows()){
                    continue;
                }
                if( n_col >= m_matrix_data.cols()){
                    continue;
                }
                m_matrix_data(n_row,n_col) = n_val;
            }while(false);
        }
        void set(size_t n_row, size_t n_col, float n_val)
        {
            do{
                if( n_row >= m_matrix_data.rows()){
                    continue;
                }
                if( n_col >= m_matrix_data.cols()){
                    continue;
                }
                m_matrix_data(n_row,n_col) = n_val;
            }while(false);
        }
        void set(size_t n_row, size_t n_col, double n_val)
        {
            do{
                if( n_row >= m_matrix_data.rows()){
                    continue;
                }
                if( n_col >= m_matrix_data.cols()){
                    continue;
                }
                m_matrix_data(n_row,n_col) = n_val;
            }while(false);
        }

        Eigen::MatrixXd get() const
        {
            return m_matrix_data;
        }

        Eigen::MatrixXd operator()() const
        {
            return m_matrix_data;
        }

        bool error() const
        {
            return m_b_last_error;
        }

        typedef std::pair<size_t,size_t>    type_size;
        c_variable::type_size size() const
        {
            return std::make_pair(m_matrix_data.rows(),m_matrix_data.cols());
        }


        /*
        * matrix operators
        */
        c_variable& operator=(const c_variable& rhs)
        {
            m_matrix_data = rhs.get();
            m_b_last_error = rhs.m_b_last_error;
            return *this;
        }

        c_variable& operator=(const Eigen::MatrixXd& rhs)
        {
            m_matrix_data = rhs;
            return *this;
        }

        c_variable& operator+=(const c_variable& rhs)
        {
            m_matrix_data += rhs.get();
            return *this;
        }
        c_variable& operator-=(const c_variable& rhs)
        {
            m_matrix_data -= rhs.get();
            return *this;
        }

        friend c_variable operator+( c_variable lhs, const c_variable& rhs )
        {
            lhs += rhs;
            return lhs;
        }
        friend c_variable operator-( c_variable lhs, const c_variable& rhs )
        {
            lhs -= rhs;
            return lhs;
        }

        /*
        * multiply each item of matrix
        */
        c_variable& operator*=(const c_variable& rhs)
        {
            do{
                size_t n_row(0),n_col(0);

                if(m_matrix_data.rows() > rhs.get().rows()){
                    n_row = rhs.get().rows();
                }
                else{
                    n_row = m_matrix_data.rows();
                }

                if(m_matrix_data.cols() > rhs.get().cols()){
                    n_col = rhs.get().cols();
                }
                else{
                    n_col = m_matrix_data.cols();
                }

                Eigen::MatrixXd matrix_new(n_row,n_col);

                for( size_t i=0; i<n_row; i++){
                    for( size_t j=0; j<n_col; j++){
                        matrix_new(i,j) = m_matrix_data(i,j) * rhs.get()(i,j);
                    }
                }//end for

                m_matrix_data = matrix_new;
            }while(false);
            return *this;
        }

        friend c_variable operator*( c_variable lhs, const c_variable& rhs )
        {
            lhs *= rhs;
            return lhs;
        }

        /*
        * scala operator
        */
       //int
        c_variable& operator=(const int& rhs)
        {
            m_matrix_data.fill(rhs);
            return *this;
        }
        c_variable& operator+=(const int& rhs)
        {
            Eigen::MatrixXd r(m_matrix_data);
            r.fill(rhs);
            m_matrix_data += r;
            return *this;
        }
        friend c_variable operator+( c_variable lhs, const int& rhs )
        {
            lhs += rhs;
            return lhs;
        }
        friend c_variable operator+( const int& lhs, c_variable rhs )
        {
            rhs += lhs;
            return rhs;
        }

        c_variable& operator-=(const int& rhs)
        {
            Eigen::MatrixXd r(m_matrix_data);
            r.fill(rhs);
            m_matrix_data -= r;
            return *this;
        }
        friend c_variable operator-( c_variable lhs, const int& rhs )
        {
            lhs -= rhs;
            return lhs;
        }
        friend c_variable operator-( const int& lhs, c_variable rhs )
        {
            rhs -= lhs;
            return rhs;
        }
 
        c_variable& operator*=(const int& rhs)
        {
            m_matrix_data *= rhs;
            return *this;
        }
        friend c_variable operator*( c_variable lhs, const int& rhs )
        {
            lhs *= rhs;
            return lhs;
        }
        friend c_variable operator*( const int& lhs, c_variable rhs )
        {
            rhs *= lhs;
            return rhs;
        }

        c_variable& operator/=(const int& rhs)
        {
            m_matrix_data /= rhs;
            return *this;
        }
        friend c_variable operator/( c_variable lhs, const int& rhs )
        {
            lhs /= rhs;
            return lhs;
        }
        friend c_variable operator/( const int& lhs, c_variable rhs )
        {
            rhs /= lhs;
            return rhs;
        }

        //long
        c_variable& operator=(const long& rhs)
        {
            m_matrix_data.fill(rhs);
            return *this;
        }
        c_variable& operator+=(const long& rhs)
        {
            Eigen::MatrixXd r(m_matrix_data);
            r.fill(rhs);
            m_matrix_data += r;
            return *this;
        }
        friend c_variable operator+( c_variable lhs, const long& rhs )
        {
            lhs += rhs;
            return lhs;
        }
        friend c_variable operator+( const long& lhs, c_variable rhs )
        {
            rhs += lhs;
            return rhs;
        }

        c_variable& operator-=(const long& rhs)
        {
            Eigen::MatrixXd r(m_matrix_data);
            r.fill(rhs);
            m_matrix_data -= r;
            return *this;
        }
        friend c_variable operator-( c_variable lhs, const long& rhs )
        {
            lhs -= rhs;
            return lhs;
        }
        friend c_variable operator-( const long& lhs, c_variable rhs )
        {
            rhs -= lhs;
            return rhs;
        }
 
        c_variable& operator*=(const long& rhs)
        {
            m_matrix_data *= rhs;
            return *this;
        }
        friend c_variable operator*( c_variable lhs, const long& rhs )
        {
            lhs *= rhs;
            return lhs;
        }
        friend c_variable operator*( const long& lhs, c_variable rhs )
        {
            rhs *= lhs;
            return rhs;
        }

        c_variable& operator/=(const long& rhs)
        {
            m_matrix_data /= rhs;
            return *this;
        }
        friend c_variable operator/( c_variable lhs, const long& rhs )
        {
            lhs /= rhs;
            return lhs;
        }
        friend c_variable operator/( const long& lhs, c_variable rhs )
        {
            rhs /= lhs;
            return rhs;
        }

        //float
        c_variable& operator=(const float& rhs)
        {
            m_matrix_data.fill(rhs);
            return *this;
        }
        c_variable& operator+=(const float& rhs)
        {
            Eigen::MatrixXd r(m_matrix_data);
            r.fill(rhs);
            m_matrix_data += r;
            return *this;
        }
        friend c_variable operator+( c_variable lhs, const float& rhs )
        {
            lhs += rhs;
            return lhs;
        }
        friend c_variable operator+( const float& lhs, c_variable rhs )
        {
            rhs += lhs;
            return rhs;
        }

        c_variable& operator-=(const float& rhs)
        {
            Eigen::MatrixXd r(m_matrix_data);
            r.fill(rhs);
            m_matrix_data -= r;
            return *this;
        }
        friend c_variable operator-( c_variable lhs, const float& rhs )
        {
            lhs -= rhs;
            return lhs;
        }
        friend c_variable operator-( const float& lhs, c_variable rhs )
        {
            rhs -= lhs;
            return rhs;
        }
 
        c_variable& operator*=(const float& rhs)
        {
            m_matrix_data *= rhs;
            return *this;
        }
        friend c_variable operator*( c_variable lhs, const float& rhs )
        {
            lhs *= rhs;
            return lhs;
        }
        friend c_variable operator*( const float& lhs, c_variable rhs )
        {
            rhs *= lhs;
            return rhs;
        }

        c_variable& operator/=(const float& rhs)
        {
            m_matrix_data /= rhs;
            return *this;
        }
        friend c_variable operator/( c_variable lhs, const float& rhs )
        {
            lhs /= rhs;
            return lhs;
        }
        friend c_variable operator/( const float& lhs, c_variable rhs )
        {
            rhs /= lhs;
            return rhs;
        }

        //double
        c_variable& operator=(const double& rhs)
        {
            m_matrix_data.fill(rhs);
            return *this;
        }
        c_variable& operator+=(const double& rhs)
        {
            Eigen::MatrixXd r(m_matrix_data);
            r.fill(rhs);
            m_matrix_data += r;
            return *this;
        }
        friend c_variable operator+( c_variable lhs, const double& rhs )
        {
            lhs += rhs;
            return lhs;
        }
        friend c_variable operator+( const double& lhs, c_variable rhs )
        {
            rhs += lhs;
            return rhs;
        }

        c_variable& operator-=(const double& rhs)
        {
            Eigen::MatrixXd r(m_matrix_data);
            r.fill(rhs);
            m_matrix_data -= r;
            return *this;
        }
        friend c_variable operator-( c_variable lhs, const double& rhs )
        {
            lhs -= rhs;
            return lhs;
        }
        friend c_variable operator-( const double& lhs, c_variable rhs )
        {
            rhs -= lhs;
            return rhs;
        }
 
        c_variable& operator*=(const double& rhs)
        {
            m_matrix_data *= rhs;
            return *this;
        }
        friend c_variable operator*( c_variable lhs, const double& rhs )
        {
            lhs *= rhs;
            return lhs;
        }
        friend c_variable operator*( const double& lhs, c_variable rhs )
        {
            rhs *= lhs;
            return rhs;
        }

        c_variable& operator/=(const double& rhs)
        {
            m_matrix_data /= rhs;
            return *this;
        }
        friend c_variable operator/( c_variable lhs, const double& rhs )
        {
            lhs /= rhs;
            return lhs;
        }
        friend c_variable operator/( const double& lhs, c_variable rhs )
        {
            rhs /= lhs;
            return rhs;
        }

        // matirix multiplication
        c_variable mul(const Eigen::MatrixXd &rhs)
        {
            c_variable result;
            do{
                result.m_b_last_error = true;
                size_t n_row(0),n_col(0);

                if(m_matrix_data.rows() == 0 ){
                    continue;
                }
                if(m_matrix_data.cols() == 0 ){
                    continue;
                }
                if(rhs.rows() == 0 ){
                    continue;
                }
                if(rhs.cols() == 0 ){
                    continue;
                }

                if(m_matrix_data.cols() == rhs.rows()){
                    //(M1xN) * (NxM2) = (M1xM2)
                    result = m_matrix_data*rhs;
                    result.m_b_last_error = false;
                    continue;
                }

                if(m_matrix_data.rows() == 1){
                    if(rhs.rows() ==1 ){
                        if(m_matrix_data.cols() == rhs.cols() ){
                            //(1xN) * (1xN) -> (1xN) * (Nx1) = (1X1)
                            result = m_matrix_data * rhs.transpose();
                            result.m_b_last_error = false;
                            continue;
                        }
                        //(1xN) * (1xM) , N != M
                        continue;
                    }
                    //(1xN) * (MxN)
                    continue;
                }

                if(rhs.rows() ==1 ){
                    if(m_matrix_data.cols() == rhs.cols() ){
                        //(MxN) * (1xN) -> (MxN) * (Nx1) = (Mx1)
                        result = m_matrix_data * rhs.transpose();
                        result.m_b_last_error = false;
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
        c_variable mul(const c_variable &rhs)
        {
            return mul(rhs.get());
        }
        private:
            Eigen::MatrixXd m_matrix_data;
            bool m_b_last_error;

            Eigen::MatrixXd m_matrix_grad;
    };
}