#pragma once
#include <Eigen/Dense>

namespace _venezia
{
    class _c_fun
    {
        public:
        virtual bool backword(void *p_dy) = 0;
        virtual std::pair<size_t,size_t> get_size_of_input() = 0;
    };
}