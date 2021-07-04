#pragma once

#include <memory>

#include <utility>
#include <Eigen/Dense>

namespace _venezia
{
    class c_fun;

    class _c_var
    {
        public:
        virtual bool empty() = 0;
        virtual void set_gradient(const _c_var & v) = 0;
        virtual void set_creator( _venezia::c_fun *p_creator) = 0;
        virtual bool backword() = 0;
    };
}