#include <iostream>


#include "fizzbuzz/fizzbuzz.hpp"
#include "util/print_tuples.hpp"
#include "util/strlit.hpp"


using namespace fizzbuzz;


int main() {
    std::cout << "FizzBuzz Result:" << std::endl;

    constexpr auto fizzbuzz_result = fizzbuzz_count_numbers<fizz_rule_t,buzz_rule_t>(std::make_index_sequence<100>{});
    std::cout << fizzbuzz_result << std::endl;

    return 0;
}
