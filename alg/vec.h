//
// Created by pc on 12-07-21.
//

#ifndef ALG_VEC_H
#define ALG_VEC_H

#include <cassert>
#include <vector>

namespace alg::vec {

    template <typename T>
    std::vector<T> concatenate(const std::vector<T>& a, const std::vector<T>& b) {
        std::vector<T> result = a;
        result.reserve(a.size() + b.size());
        std::copy(b.begin(), b.end(), std::advance(result.begin(), a.size()));
        return result;
    }

    template <typename T>
    void append(std::vector<T>& a, const std::vector<T>& b) {
        assert(&a != &b);
        a.reserve(a.size() + b.size());
        std::copy(b.begin(), b.end(), std::back_inserter(a));
    }

} // namespace alg::vec

#endif // ALG_VEC_H
