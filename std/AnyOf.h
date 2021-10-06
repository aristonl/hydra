#pragma once

#include <std/Concepts.h>
#include <std/Find.h>
#include <std/Iterator.h>

namespace std {

    template<typename TEndIterator, IteratorPairWith<TEndIterator> TIterator>
    constexpr bool any_of(
            TIterator const& begin,
            TEndIterator const& end,
            auto const& predicate)
    {
        return find_if(begin, end, predicate) != end;
    }

    template<IterableContainer Container>
    constexpr bool any_of(Container&& container, auto const& predicate)
{
    return any_of(container.begin(), container.end(), predicate);
}

}

using std::any_of;