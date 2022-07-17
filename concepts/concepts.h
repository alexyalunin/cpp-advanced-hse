#include <type_traits>
#include <iterator>

template <class P, class T>
concept Predicate = std::is_same_v<bool, decltype(std::declval<P>()(std::declval<T>()))>;

template <class Derived, class Base>
concept derived_from = std::is_base_of_v<Base, Derived>&&
    std::is_convertible_v<const volatile Derived*, const volatile Base*>;

template <class I>
concept Random_access_iterator = derived_from<typename std::iterator_traits<I>::iterator_category,
                                              std::random_access_iterator_tag>;

template <class T>
concept Indexable = requires {
    requires std::is_convertible_v<decltype(T::iterator), decltype(std::begin(std::declval<T>()))>;
    requires std::is_convertible_v<decltype(T::iterator), decltype(std::end(std::declval<T>()))>;
    requires Random_access_iterator<decltype(std::begin(std::declval<T>()))>;
    requires Random_access_iterator<decltype(std::end(std::declval<T>()))>;
}
|| requires {
    std::declval<T>()[std::declval<size_t>()];
    requires !std::is_same_v<void, decltype(std::declval<T>()[std::declval<size_t>()])>;
};