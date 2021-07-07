#pragma once
#include <Eigen/Dense>

namespace _venezia
{
    class _c_fun
    {
        public:
        virtual bool backword(const Eigen::MatrixXd & mt_dy) = 0;
    };
}