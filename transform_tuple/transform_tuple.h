#pragma once

#include <tuple>
#include <type_traits>
#include <utility>
#include <cstdint>

template <class F, class... Args>
using TransformResult =
    decltype(std::make_tuple(std::declval<F>()(std::declval<Args>())...));  /// 1

template <class F, class... Types>
concept invocable_on_all_types = requires {
    (std::declval<F>()(std::declval<Types>()), ...);
};  /// 2

template <class F, class... Args, size_t... Indexes>
constexpr TransformResult<F, Args...> TransformTupleImpl(F&& functor,
                                                         const std::tuple<Args...>& args,
                                                         std::index_sequence<Indexes...>) {
    return std::make_tuple(functor(std::get<Indexes>(args))...);
}

template <class F, class... Args>
constexpr TransformResult<F, Args...> TransformTuple(
    F&& functor, const std::tuple<Args...>& args) requires invocable_on_all_types<F, Args...> {
    return TransformTupleImpl(std::forward<F>(functor), args,
                              std::index_sequence_for<Args...>{});  /// 3
}

template <class F, class T>
constexpr auto VoidIgnore(F&& functor, const T& obj) {
    if constexpr (std::is_same_v<decltype(functor(obj)), void>) {
        return std::make_tuple();
    } else {
        return std::make_tuple(functor(obj));
    }
}

template <class F, class... Args, size_t... Indexes>
constexpr auto TransformReduceTupleImpl(F&& functor, const std::tuple<Args...>& args,
                                        std::index_sequence<Indexes...>) {
    return std::tuple_cat(VoidIgnore(std::forward<F>(functor), std::get<Indexes>(args))...);
}

template <class F, class... Args>
constexpr auto TransformReduceTuple(
    F&& functor, const std::tuple<Args...>& args) requires invocable_on_all_types<F, Args...> {
    return TransformReduceTupleImpl(std::forward<F>(functor), args,
                                    std::index_sequence_for<Args...>{});  /// 4
}