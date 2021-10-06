#pragma once

#include <std/Concepts.h>
#include <std/Find.h>
#include <std/Iterator.h>

namespace std {

    template<typename TEndIterator, IteratorPairWith<TEndIterator> TIterator>
    constexpr bool all_of(
            TIterator const& begin,
            TEndIterator const& end,
            auto const& predicate)
    {
        constexpr auto negated_predicate = [](auto const& pred) {
            return [&](auto const& elem) { return !pred(elem); };
        };
        return !(find_if(begin, end, negated_predicate(predicate)) != end);
    }

    template<IterableContainer Container>
    constexpr bool all_of(Container&& container, auto const& predicate)
{
    return all_of(container.begin(), container.end(), predicate);
}

}

using std::all_of;
