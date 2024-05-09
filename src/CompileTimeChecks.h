#pragma once

#include <type_traits>

// From Eric Niebler's talk "C++11 Library Design" at C++Now 2014
// https://youtu.be/zgOF4NrQllo?t=37m15s 
// https://github.com/boostcon/cppnow_presentations_2014/blob/master/files/cxx11-library-design.pdf (slides 37 and 38)

namespace detail
{
	template <typename T>
	std::false_type check_equality_comparable(T const& t, long);


	template <typename T>
	auto check_equality_comparable(T const& t, int)
		-> typename std::is_convertible<decltype(t == t), bool>::type;
}

template <typename T>
struct is_equality_comparible : decltype(detail::check_equality_comparable(std::declval<T const &>(), 1))
{};

template <typename T>
struct is_regular
	: std::integral_constant<bool,
							std::is_default_constructible<T>::value &&
							std::is_copy_constructible<T>::value &&
							std::is_move_constructible<T>::value &&
							std::is_copy_assignable<T>::value &&
							std::is_move_assignable<T>::value &&
							is_equality_comparible<T>::value> 
{};


//static_assert(std::is_default_constructible<T>::value, "default ctor");
//static_assert(std::is_copy_constructible<T>::value, "copy ctor");
//static_assert(std::is_move_constructible<T>::value, "move ctor");
//static_assert(std::is_copy_assignable<T>::value, "copy assign");
//static_assert(std::is_move_assignable<T>::value, "move assign");
//static_assert(is_equality_comparible<T>::value, "eqaulity compare");
