#pragma once

#include <inc/c_variable.h>

namespace _venezia
{
    /**
     * @brief ths base class of all function 
     * this is virtual class
     * 
     */
    class c_function
    {
        public:
            _venezia::c_variable operator()(const _venezia::c_variable & in_variable )
            {
                Eigen::MatrixXd x(in_variable.get());
                Eigen::MatrixXd y = _forward(x);
                return  _venezia::c_variable(y);
            }

        protected:
            virtual Eigen::MatrixXd _forward(const Eigen::MatrixXd & data) = 0;

    };
}