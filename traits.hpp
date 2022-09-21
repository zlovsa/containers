#pragma once

namespace ft
{

template <bool, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> {	typedef T type; };

template <class Iter>
struct iterator_traits
{
	typedef typename Iter::difference_type difference_type;
	typedef typename Iter::value_type value_type;
	typedef typename Iter::pointer pointer;
	typedef typename Iter::reference reference;
	typedef typename Iter::iterator_category iterator_category;
};

template <class T>
struct iterator_traits<T *>
{
	typedef std::ptrdiff_t difference_type;
	typedef T value_type;
	typedef T *pointer;
	typedef T &reference;
	typedef std::random_access_iterator_tag iterator_category;
};

template <class T, T v>
struct integral_constant{
	static const T value = v;
	typedef T value_type;
	typedef integral_constant type;
	operator value_type() const { return value; }
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template <class T> struct is_integral : false_type {};
template <> struct is_integral<bool> : true_type{};
template <> struct is_integral<char> : true_type{};
template <> struct is_integral<unsigned char> : true_type{};
template <> struct is_integral<signed char> : true_type{};
template <> struct is_integral<wchar_t> : true_type{};
template <> struct is_integral<short> : true_type{};
template <> struct is_integral<unsigned short> : true_type{};
template <> struct is_integral<int> : true_type{};
template <> struct is_integral<unsigned int> : true_type{};
template <> struct is_integral<long> : true_type{};
template <> struct is_integral<unsigned long> : true_type{};
//template <> struct is_integral<long long> : true_type{};
//template <> struct is_integral<unsigned long long> : true_type{};

}
