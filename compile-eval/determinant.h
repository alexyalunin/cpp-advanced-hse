#pragma once

#include <array>

template <int N>
constexpr int determinant(const std::array<std::array<int, N>, N>& a) {
    int det = 0, sgn = 1;
    for (int i = 0; i < N; ++i) {
        std::array<std::array<int, N - 1>, N - 1> b{};
        for (int j = 0; j < N - 1; ++j) {
            for (int k = 0; k < N - 1; ++k) {
                (&std::get<0>((&std::get<0>(b))[j]))[k] = a[j + 1][k + (k >= i)];
            }
        }
        det += determinant<N - 1>(b) * a[0][i] * sgn;
        sgn = -sgn;
    }
    return det;
}

template <>
constexpr int determinant<1>(const std::array<std::array<int, 1>, 1>& a) {
    return a[0][0];
}

template <>
constexpr int determinant<2>(const std::array<std::array<int, 2>, 2>& a) {
    return a[0][0] * a[1][1] - a[0][1] * a[1][0];
}

template <>
constexpr int determinant<3>(const std::array<std::array<int, 3>, 3>& a) {
    return a[0][0] * a[1][1] * a[2][2] + a[0][1] * a[1][2] * a[2][0] + a[0][2] * a[1][0] * a[2][1] -
           a[0][2] * a[1][1] * a[2][0] - a[0][0] * a[1][2] * a[2][1] - a[0][1] * a[1][0] * a[2][2];
}