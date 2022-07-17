#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <initializer_list>
#include <numeric>
struct BaseBase {
public:
};

template <typename T>
struct Base : public BaseBase {
public:
    T& Interface() {
        return static_cast<T&>(*this);
    }
    const T& Interface() const {
        return static_cast<const T&>(*this);
    }
};

template <typename L, typename R>
struct Glue : public Base<Glue<L, R>> {
    using Left = L;
    using Right = R;
    Glue(const L& l, const R& r) : left(l), right(r) {
    }
    const L& left;
    const R& right;
};

template <typename T>
struct GetCount {
    static const int kCount;
};

template <typename L, typename R>
struct GetCount<Glue<L, R>> {
    static const int kCount = GetCount<L>::kCount + GetCount<R>::kCount;
};

template <typename T, typename Filler>
constexpr void Fill(T* ptr, const Filler& filler) {
}

template <class T>
class Matrix : public Base<Matrix<T>> {
public:
    Matrix() = default;

    Matrix(size_t rows, size_t columns) {
        matrix.resize(rows, std::vector<T>(columns));
    }

    Matrix(const Matrix<T>& other) {
        matrix = other.matrix;
    }

    Matrix(const std::vector<std::vector<T>>& other) {
        matrix = other;
    }

    Matrix(const std::initializer_list<std::vector<T>>& other) {
        matrix = other;
    }

    Matrix<T>& operator=(const Matrix<T>& other) {
        matrix = other.matrix;
        return *this;
    }

    T& operator()(size_t i, size_t j) {
        return matrix[i][j];
    }
    const T& operator()(size_t i, size_t j) const {
        return matrix[i][j];
    }

    friend Matrix<T> operator+(const Matrix<T>& left, const Matrix<T>& right) {
        if (left.Rows() != right.Rows() || left.Columns() != right.Columns()) {
            throw std::invalid_argument("wrong dim in sum");
        }
        Matrix<T> res = left;
        for (size_t i = 0; i < left.Rows(); ++i) {
            for (size_t j = 0; j < left.Columns(); ++j) {
                res(i, j) += right(i, j);
            }
        }
        return res;
    }
    friend Matrix<T> operator-(const Matrix<T>& left, const Matrix<T>& right) {
        if (left.Rows() != right.Rows() || left.Columns() != right.Columns()) {
            throw std::invalid_argument("wrong dim in sub");
        }
        Matrix<T> res = left;
        for (size_t i = 0; i < left.Rows(); ++i) {
            for (size_t j = 0; j < left.Columns(); ++j) {
                res(i, j) -= right(i, j);
            }
        }
        return res;
    }

    [[nodiscard]] size_t Rows() const {
        return matrix.size();
    }

    [[nodiscard]] size_t Columns() const {
        if (matrix.empty()) {
            return 0;
        }
        return matrix[0].size();
    }

    template <class L, class R>
    Matrix(const Glue<L, R>& tree) {
        const int size = GetCount<Glue<L, R>>::kCount;
        const Matrix<T>* operands[size];
        Fill(&operands[size - 1], tree);
        for (int i = 0; i < size - 1; ++i) {
            if (operands[i]->Columns() != operands[i + 1]->Rows()) {
                throw std::runtime_error("Wrong dim in mul");
            }
        }
        std::vector<std::vector<int>> dp(size, std::vector<int>(size, INT32_MAX));
        std::vector<std::vector<int>> edges(size, std::vector<int>(size, 0));
        for (int i = 0; i < size; ++i) {
            dp[i][i] = 0;
        }
        for (int len = 2; len <= size; ++len) {
            for (int first = 0; first <= size - len; ++first) {
                int last = first + len - 1;
                for (int edge = first; edge < last; ++edge) {
                    int sum = dp[first][edge] + dp[edge + 1][last] +
                              operands[first]->Rows() * operands[edge]->Columns() *
                                  operands[last]->Columns();
                    if (dp[first][last] > sum) {
                        dp[first][last] = sum;
                        edges[first][last] = edge;
                    }
                }
            }
        }
        matrix = RightMult(0, size - 1, edges, operands);
    }

    static std::vector<std::vector<T>> Multiply(const std::vector<std::vector<T>>& left,
                                                const std::vector<std::vector<T>>& right) {
        std::vector<std::vector<T>> res(left.size(), std::vector<T>(right[0].size(), 0));
        for (size_t i = 0; i < left.size(); ++i) {
            for (size_t j = 0; j < right[0].size(); ++j) {
                for (size_t k = 0; k < right.size(); ++k) {
                    res[i][j] += left[i][k] * right[k][j];
                }
            }
        }
        return res;
    }

    friend std::vector<std::vector<T>> RightMult(int i, int j,
                                                 const std::vector<std::vector<int>>& edges,
                                                 const Matrix<T>** buffer) {
        if (i == j) {
            return (*(buffer + i))->matrix;
        }
        return Multiply(RightMult(i, edges[i][j], edges, buffer),
                        RightMult(edges[i][j] + 1, j, edges, buffer));
    }
    std::vector<std::vector<T>> matrix;
};

template <typename T>
constexpr void Fill(const Matrix<T>** ptr, const Matrix<T>& filler) {
    *ptr = &filler;
}

template <typename L, typename T>
constexpr void Fill(const Matrix<T>** ptr, const Glue<L, Matrix<T>>& filler) {
    *ptr = &filler.right;
    Fill(ptr - 1, filler.left);
}

template <typename T>
struct GetCount<Matrix<T>> {
    static const int kCount = 1;
};

template <typename L, typename R>
Glue<L, R> operator*(const Base<L>& left, const Base<R>& right) {
    return Glue(left.Interface(), right.Interface());
}