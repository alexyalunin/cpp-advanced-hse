#pragma once
#include "impl.h"

#include <limits>
#include <utility>
#include <tuple>
#include <type_traits>
#include <utility>
#include <cstdint>

template <typename T, char h>
struct Add {
    using Added = void;
};

template <size_t pos, char... chars, char h>
struct Add<Tape<pos, chars...>, h> {
    using Added = Tape<pos, chars..., h>;
};

template <typename T>
struct PopFront {
    using Poped = void;
};

template <size_t pos>
struct PopFront<Tape<pos>> {
    using Poped = Error;
};

template <size_t pos, char h>
struct PopFront<Tape<pos, h>> {
    using Poped = std::pair<Char<h>, Tape<pos>>;
};

template <size_t pos, char h, char... chars>
struct PopFront<Tape<pos, h, chars...>> {
    using Poped = std::pair<Char<h>, Tape<pos, chars...>>;
};

template <typename T>
struct DecPos {
    using Decreased = void;
};

template <char... chars>
struct DecPos<Tape<0, chars...>> {
    using Decreased = Error;
};

template <size_t pos, char... chars>
struct DecPos<Tape<pos, chars...>> {
    using Decreased = Tape<pos - 1, chars...>;
};

template <char... chars>
struct GetSize {
    static const size_t kSize = 0;
};

template <char ch, char... chars>
struct GetSize<ch, chars...> {
    static const size_t kSize = GetSize<chars...>::kSize + 1;
};

template <char ch>
struct GetSize<ch> {
    static const size_t kSize = 0;
};

template <>
struct GetSize<> {
    static const size_t kSize = 0;
};

template <size_t a, size_t b, char... chars>
struct EqInc {
    using Ans = Tape<a + 1, chars...>;
};

template <size_t a, char... chars>
struct EqInc<a, a, chars...> {
    using Ans = Error;
};

template <typename T>
struct IncPos {
    using Increased = void;
};

template <size_t pos, char... chars>
struct IncPos<Tape<pos, chars...>> {
    using Increased = typename EqInc<pos, GetSize<chars...>::kSize, chars...>::Ans;
};

template <>
struct IncPos<Tape<0>> {
    using Increased = Error;
};

template <char ch, typename t>
struct Glue {
    using Glued = void;
};

template <char ch, size_t pos, char... chars>
struct Glue<ch, Tape<pos, chars...>> {
    using Glued = Tape<pos, ch, chars...>;
};

template <char ch>
struct Glue<ch, Error> {
    using Glued = Error;
};

template <char ch, size_t pos, char... chars>
struct Inc {
    using Increased = Tape<pos, ch + 1, chars...>;
};

template <size_t pos, char... chars>
struct Inc<std::numeric_limits<char>::max(), pos, chars...> {
    using Increased = Error;
};

template <size_t cur, size_t pos, char... chars>
struct IncFindChar {
    using Increased = void;
};

template <size_t cur, size_t pos, char c, char... chars>
struct IncFindChar<cur, pos, c, chars...> {
    using Increased =
        typename Glue<c, typename IncFindChar<cur + 1, pos, chars...>::Increased>::Glued;
};

template <size_t cur, char c, char... chars>
struct IncFindChar<cur, cur, c, chars...> {
    using Increased = typename Inc<c, cur, chars...>::Increased;
};

template <typename T>
struct IncCell {
    using Increased = void;
};

template <size_t pos, char... chars>
struct IncCell<Tape<pos, chars...>> {
    using Increased = typename IncFindChar<0, pos, chars...>::Increased;
};

template <char ch, size_t pos, char... chars>
struct Dec {
    using Decreased = Tape<pos, ch - 1, chars...>;
};

template <size_t pos, char... chars>
struct Dec<std::numeric_limits<char>::min(), pos, chars...> {
    using Decreased = Error;
};

template <size_t cur, size_t pos, char... chars>
struct DecFindChar {
    using Decreased = void;
};

template <size_t cur, size_t pos, char c, char... chars>
struct DecFindChar<cur, pos, c, chars...> {
    using Decreased =
        typename Glue<c, typename DecFindChar<cur + 1, pos, chars...>::Decreased>::Glued;
};

template <size_t cur, char c, char... chars>
struct DecFindChar<cur, cur, c, chars...> {
    using Decreased = typename Dec<c, cur, chars...>::Decreased;
};

template <typename T>
struct DecCell {
    using Decreased = void;
};

template <size_t pos, char... chars>
struct DecCell<Tape<pos, chars...>> {
    using Decreased = typename DecFindChar<0, pos, chars...>::Decreased;
};

/// Return a Tape with h appended to Tape.
template <char h, class FakeTape>
using Produce = typename Add<FakeTape, h>::Added;

/// Return a pair type (Char<c>, Tape) with first (leftmost) char popped from Tape.
/// If Tape was empty, return Error.
template <class FakeTape>
using Consume = typename PopFront<FakeTape>::Poped;

/// Return Tape with incremented pos.
/// If pos falls out of bounds, return Error.
template <class FakeTape>
using IncrementPos = typename IncPos<FakeTape>::Increased;

/// Return Tape with decremented pos.
/// Is pos falls out of bounds, return Error.
template <class FakeTape>
using DecrementPos = typename DecPos<FakeTape>::Decreased;

/// Return Tape with pos'th char incremented.
/// If pos'th char would overflow, return Error.
/// Keep in mind that "char" needn't be signed on all platforms.
template <class FakeTape>
using IncrementCell = typename IncCell<FakeTape>::Increased;

/// Return Tape with pos'th char decremented.
/// If pos'th char would underflow, return Error.
template <class FakeTape>
using DecrementCell = typename DecCell<FakeTape>::Decreased;
