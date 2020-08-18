//
// Created by georgios on 18.08.20.
//

#ifndef FIZZBUZZ_RULES_HPP
#define FIZZBUZZ_RULES_HPP

#include "util/strlit.hpp"
#include <optional>
#include <tuple>

namespace fizzbuzz {


    /// A rule which replaces any number that is divisible
    /// by three by "Fizz". For any other number an empty
    /// string is returned.
    template<size_t number>
    struct fizz_rule_t {
        static constexpr auto get_replacement_word() {
            if constexpr(number % 3==0 && number != 0) {
                return util::StrLit("Fizz");
            } else {
                return util::StrLit("");
            }
        }
    };

    /// A rule which replaces any number that is divisible
    /// by three by "Fizz". For any other number an empty
    /// string is returned.
    template<size_t number>
    struct buzz_rule_t {
        static constexpr auto get_replacement_word() {
            if constexpr(number % 5==0 && number!=0) {
                return util::StrLit("Buzz");
            } else {
                return util::StrLit("");
            }
        }
    };

    /// Applies all given rules to the number num and concatenate
    /// the strings for all rules in the order in which they are given.
    /// Result is stored in the member replacement. The member replacement
    /// can be an empty string if no rules wanted to replace the number.
    template<size_t num, template<size_t> typename Rule, template<size_t...> typename ...Rules>
    struct apply_rules_to_number {

        //constexpr static size_t number = num;
        constexpr static util::StrLit replacement_word = Rule<num>::get_replacement_word() + apply_rules_to_number<num, Rules...>::replacement_word;
    };

    ///specialization of above for single rule to single number
    template<size_t num, template<size_t> typename Rule>
    struct apply_rules_to_number<num,Rule> {

        //constexpr static size_t number = num;
        constexpr static util::StrLit replacement_word = Rule<num>::get_replacement_word();
    };

    ///extract the result from applying a set of rules to a single number
    ///(either number or StrList) at compile time
    template<size_t number, template<size_t> typename ...Rules>
    struct count_aloud_with_rules {
        constexpr static auto result() {
            constexpr util::StrLit replacement_word = apply_rules_to_number<number, Rules...>::replacement_word;
            if constexpr (replacement_word.is_empty()) {
                return number;
            }
            else {
                return replacement_word;
            }
        }
    };

    /// produce the sequence of counts (or replacements) corresponding to the
    /// given index sequence
    template<template<size_t> typename ...Rules,size_t... numbers>
    constexpr auto fizzbuzz_count_numbers(std::index_sequence<numbers...>)
    {
        static_assert(sizeof...(numbers)>0, "Numbers must be > 0");
        return std::tuple{count_aloud_with_rules<numbers,Rules...>::result()... };
        //caveat:
        //this here:     return std::tuple{ (count_aloud_with_rules<numbers,Rules...>::result(),...) };
        //will produce wrong results because it uses the comma operator and thus only returns the last result!!
    }
}

#endif //FIZZBUZZ_RULES_HPP
