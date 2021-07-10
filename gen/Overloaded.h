//
// Created by pc on 08-06-21.
//

#ifndef GEN_OVERLOADED_H
#define GEN_OVERLOADED_H

// https://en.cppreference.com/w/cpp/utility/variant/visit
// helper type for the visitor #4
template <class... Ts>
struct Overloaded : Ts... {
    using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

#endif // GEN_OVERLOADED_H
