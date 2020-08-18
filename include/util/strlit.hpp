//
// Created by georgios on 18.08.20.
//

#ifndef FIZZBUZZ_CONSTEXPR_STRING_HPP
#define FIZZBUZZ_CONSTEXPR_STRING_HPP

#include <ostream>
#include <utility>
#include <array>
#include <cassert>

namespace util {

    template<size_t N>
    struct StrLit;

    template<size_t N1, size_t N2, size_t ...Is1, size_t ...Is2>
    constexpr StrLit<N1 + N2> concat_impl(const StrLit<N1> & s1, std::index_sequence<Is1...>, const StrLit<N2> & s2, std::index_sequence<Is2...>);

    /// A compile time constant string which owns its data and can be
    /// contatenated. The type StrLit<N> contains N+1 characters where
    /// the last character is always null. Ideas taken from:
    /// https://akrzemi1.wordpress.com/2017/06/28/compile-time-string-concatenation/
    template<size_t N>
    struct StrLit {

        /// create a new string from a character array
        /// the string is guaranteed to be zero terminated even if it means
        /// overwriting the last given character
        /// if NDEBUG is not defined, a non zero terminated character array will
        /// also produce a compile time error
        constexpr StrLit(const char (&str)[N + 1]) : StrLit(str, std::make_index_sequence<N>{}) {}

        /// concatenate two strings
        template<size_t M>
        constexpr StrLit<N + M> operator+(const StrLit<M> & rhs) const {
            return concat_impl(*this,std::make_index_sequence<N>{},rhs, std::make_index_sequence<M>{});
        }

        /// access element of string at given index
        constexpr char operator[](size_t idx) const {
            assert(idx < N+1);
            return _str[idx];
        }

        ///number of characters in string (excluding terminating 0 char)
        constexpr size_t character_count () const {
            return N;
        }

        constexpr size_t is_empty() const {
            return N==0;
        }

        //todo: this cannot be assigned to constexpr variable (even if I declare it constexpr)... why?
        const char * c_str() const {
            return _str.data();
        }

        ///get the underlying array including zero terminating final character
        constexpr const std::array<char,N+1>& data() const {
            return _str;
        }

    private:
        // implementation helper for the public constructor
        // uses and index range to copy the elements from the character array to the internal array
        template<size_t ...Is>
        constexpr StrLit(const char (&str)[N + 1], std::index_sequence<Is...>) : _str{str[Is]..., '\0'} {
            static_assert(sizeof...(Is)==N); //make sure the usage is probably correct
            //if NDEBUG is not defined will fail at compile time or runtime depending on whehter constexpr
            assert(str[N]=='\0');
        }

    private:
        // the array of characters including terminating null
        std::array<char,N+1> _str;
    };

/// helper function for the + operator
    template<size_t N1, size_t N2, size_t ...Is1, size_t ...Is2>
    constexpr StrLit<N1 + N2> concat_impl(const StrLit<N1> & s1, std::index_sequence<Is1...>, const StrLit<N2> & s2, std::index_sequence<Is2...>)
    {
        return StrLit<N1 + N2>((const char[]){s1.data()[Is1]..., s2.data()[Is2]..., '\0'});
    }

//! Deduction guide
    template<size_t N>
    StrLit(const char(&str)[N]) -> StrLit<N - 1>;

}

namespace util::detail {
/// implementation helper for ostream << operator
    template<size_t N, size_t ...Is>
    std::ostream &stream_impl(std::ostream &os, const util::StrLit<N> &str, std::index_sequence<Is...>) {
        ((os << (str.data()[Is])), ...);
        return os;
    }
}
///operator << for ostreams
template<size_t N>
std::ostream& operator<<(std::ostream & os, const util::StrLit<N> &str) {
    return util::detail::stream_impl(os,str, std::make_index_sequence<N+1>{});
}

#endif //FIZZBUZZ_CONSTEXPR_STRING_HPP
