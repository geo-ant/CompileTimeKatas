//
// Created by georgios on 18.08.20.
//

#ifndef FIZZBUZZ_PRINT_TUPLES_HPP
#define FIZZBUZZ_PRINT_TUPLES_HPP

#include <ostream>
#include <tuple>
#include <utility>



namespace util::detail
{
    //helper function to pipe the individual elements of the
    //tuple to the ostream with newlines in between
    template<typename ...Ts, size_t ...Is>
    std::ostream & println_tuple_impl(std::ostream& os, std::tuple<Ts...> tuple, std::index_sequence<Is...>)
    {
        static_assert(sizeof...(Is)==sizeof...(Ts),"Indices must have same number of elements as tuple types!");
        static_assert(sizeof...(Ts)>0, "Cannot pipe empty tuple to output");

        constexpr auto last = sizeof...(Is)-1;
        // since I want to also print newlines it is less useful to use a fold expression here
        // this is why I use the comma operator method
        return ((os << std::get<Is>(tuple) << (Is != last ? "\r\n" : "")),...);
    }
}

/// overload the << operator for ostream to print a tuple to a stream
/// see for reference: https://stackoverflow.com/questions/6245735/pretty-print-stdtuple
template<typename ...Ts>
std::ostream & operator<<(std::ostream& os, const std::tuple<Ts...> & tuple) {
    return util::detail::println_tuple_impl(os, tuple, std::index_sequence_for<Ts...>{});
}

#endif //FIZZBUZZ_PRINT_TUPLES_HPP
