#pragma once

#include "../../LibC/stddef.h"
#include "../../LibC/stdint.h"
namespace Utils
{
    template <class T>
    class container
    {

    public:
        using type = T;

        virtual T &get(size_t idx) = 0;
        virtual const T &get(size_t idx) const = 0;

        virtual size_t size() const { return 0; };
    };

}
