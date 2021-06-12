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
                _venezia::c_variable y = _forward(in_variable);
                return  y;
            }

        protected:
            virtual _venezia::c_variable _forward(const _venezia::c_variable  & data) = 0;

    };
}