template <bool B, class T, class F>
struct conditional {
    typedef T type;
};

template <class T, class F>
struct conditional<false, T, F> {
    typedef F type;
};

template <int N, int L = 0, int R = 46341, bool F = (L == R - 1)>
struct Sqrt {
    static const int value =
        conditional<((R + L) / 2) * ((R + L) / 2) >= N, Sqrt<N, L, (R + L) / 2>,
                    Sqrt<N, (R + L) / 2, R> >::type::value;
};

template <int N, int L, int R>
struct Sqrt<N, L, R, true> {
    static const int value = R;
};