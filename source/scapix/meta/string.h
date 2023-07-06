/*
	scapix/meta/string.h

	Copyright (c) 2019-2023 Boris Rasin (boris@scapix.com)
*/

#ifndef SCAPIX_META_STRING_H
#define SCAPIX_META_STRING_H

// Three different implementations for SCAPIX_META_STRING() - using string literal as template parameter:
//
// 1. C++20 class template arguments (fast compile).
// 2. GNU C++ extension "string literal operator templates", supported by gcc and clang in any mode with [#pragma GCC diagnostic ignored "-Wgnu-string-literal-operator-template"] (fast compile).
// 3. BOOST_METAPARSE_STRING (slow compile).
//
// Huge difference in compile speed when using many meta strings, for example headers generated by scapix_java.

#if __cpp_nontype_template_args >= 201911L || (__cplusplus >= 202002L && __clang_major__ >= 12) || (__cplusplus >= 201709L && __GNUC__ >= 9)

#include <utility>
#include <scapix/core/fixed_string.h>

namespace scapix::meta {

template <char... Chars>
struct string
{
	using type = string;
};

template <class S>
struct c_str;

template <char... Cs>
struct c_str<string<Cs...>>
{
	inline static constexpr char value[sizeof...(Cs) + 1] = {Cs..., 0};
};

template <typename T>
constexpr auto& c_str_v = c_str<T>::value;

template <fixed_string S>
struct string_type
{
	template <typename T>
	struct impl;

	template <std::size_t... I>
	struct impl<std::index_sequence<I...>>
	{
		using type = string<S.content[I]...>;
	};

	using type = typename impl<std::make_index_sequence<S.size()>>::type;
};

#define SCAPIX_META_STRING(s) scapix::meta::string_type<s>::type

} // namespace scapix::meta

#elif defined(__GNUG__) || defined(__clang__)

namespace scapix::meta {

template <char... Chars>
struct string
{
	using type = string;
};

template <class S>
struct c_str;

template <char... Cs>
struct c_str<string<Cs...>>
{
	inline static constexpr char value[sizeof...(Cs) + 1] = {Cs..., 0};
};

template <typename T>
constexpr auto& c_str_v = c_str<T>::value;

#define SCAPIX_META_STRING(s) decltype(s##_scapix_meta_string)

} // namespace scapix::meta

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wgnu-string-literal-operator-template"

template <typename Char, Char... Chars>
constexpr scapix::meta::string<Chars...> operator "" _scapix_meta_string(); // undefined

#pragma GCC diagnostic pop

#else // BOOST_METAPARSE

#define BOOST_METAPARSE_LIMIT_STRING_SIZE 64
#include <boost/metaparse/string.hpp>

namespace scapix::meta {

template <char... Chars>
using string = boost::metaparse::string<Chars...>;

template <typename T>
constexpr auto& c_str_v = boost::mpl::c_str<T>::value;

#define SCAPIX_META_STRING(s) BOOST_METAPARSE_STRING(s)

} // namespace scapix::meta

#endif // BOOST_METAPARSE

namespace scapix::meta {

template <typename ...Strings>
struct concat;

template <typename ...Strings>
using concat_t = typename concat<Strings...>::type;

template <char ...Chars>
struct concat<string<Chars...>> : string<Chars...> {};

template <char ...Chars1, char ...Chars2>
struct concat<string<Chars1...>, string<Chars2...>> : string<Chars1..., Chars2...> {};

template <typename String1, typename String2, typename ...Strings>
struct concat<String1, String2, Strings...> : concat_t<concat_t<String1, String2>, Strings...> {};

} // namespace scapix::meta

#endif // SCAPIX_META_STRING_H
