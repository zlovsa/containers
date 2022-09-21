#pragma once
#include "traits.hpp"

namespace ft
{
	template <typename Iter>
	class reverse_iterator;
}

template <typename Iter>
class ft::reverse_iterator
{
public:
	typedef typename iterator_traits<Iter>::difference_type difference_type;
	typedef typename iterator_traits<Iter>::value_type value_type;
	typedef typename iterator_traits<Iter>::reference reference;
	typedef typename iterator_traits<Iter>::pointer pointer;
	typedef typename iterator_traits<Iter>::iterator_category iterator_category;

	reverse_iterator(Iter iter) : _iter(iter) {}
	reverse_iterator() : _iter() {}
	reverse_iterator(const reverse_iterator &other) : _iter(other._iter) {}
	template <typename Iter1>
	reverse_iterator(const reverse_iterator<Iter1> &other) : _iter(other.base()) {}
	Iter base() const { return _iter; }
	reference operator*() const
	{
		Iter t = _iter;
		return *--t;
	}
	reverse_iterator operator+(difference_type n) const { return reverse_iterator(_iter - n); }
	reverse_iterator operator-(difference_type n) const { return reverse_iterator(_iter + n); }
	reverse_iterator &operator++()
	{
		--_iter;
		return (*this);
	}
	reverse_iterator operator++(int)
	{
		reverse_iterator t(*this);
		--_iter;
		return t;
	}
	reverse_iterator &operator--()
	{
		++_iter;
		return (*this);
	}
	reverse_iterator operator--(int)
	{
		reverse_iterator t(*this);
		++_iter;
		return t;
	}
	reverse_iterator &operator+=(difference_type n)
	{
		_iter -= n;
		return *this;
	}
	reverse_iterator &operator-=(difference_type n)
	{
		_iter += n;
		return *this;
	}
	pointer operator->() const { return &(operator*()); }
	reference operator[](difference_type n) const { return *(*this + n); }

private:
	Iter _iter;
};

template <typename Iter>
bool operator==(const ft::reverse_iterator<Iter> &x,
				const ft::reverse_iterator<Iter> &y) { return x.base() == y.base(); }
template <typename Iter>
bool operator<(const ft::reverse_iterator<Iter> &x,
			   const ft::reverse_iterator<Iter> &y) { return x.base() > y.base(); }
template <typename Iter>
bool operator!=(const ft::reverse_iterator<Iter> &x,
				const ft::reverse_iterator<Iter> &y) { return !(x.base() == y.base()); }
template <typename Iter>
bool operator>(const ft::reverse_iterator<Iter> &x,
			   const ft::reverse_iterator<Iter> &y) { return x.base() < y.base(); }
template <typename Iter>
bool operator<=(const ft::reverse_iterator<Iter> &x,
				const ft::reverse_iterator<Iter> &y) { return !(x > y); }
template <typename Iter>
bool operator>=(const ft::reverse_iterator<Iter> &x,
				const ft::reverse_iterator<Iter> &y) { return !(x < y); }
template <typename Iter>
typename ft::reverse_iterator<Iter>::difference_type operator-(
	const ft::reverse_iterator<Iter> &x,
	const ft::reverse_iterator<Iter> &y) { return y.base() - x.base(); }
template <typename Iter>
ft::reverse_iterator<Iter> operator+(
	typename ft::reverse_iterator<Iter>::difference_type n,
	const ft::reverse_iterator<Iter> &x) { return x.base() - n; }
template <typename IterL, typename IterR>
bool operator==(const ft::reverse_iterator<IterL> &x,
				const ft::reverse_iterator<IterR> &y) { return x.base() == y.base(); }
template <typename IterL, typename IterR>
bool operator<(const ft::reverse_iterator<IterL> &x,
			   const ft::reverse_iterator<IterR> &y) { return x.base() > y.base(); }
template <typename IterL, typename IterR>
bool operator!=(const ft::reverse_iterator<IterL> &x,
				const ft::reverse_iterator<IterR> &y) { return !(x == y); }
template <typename IterL, typename IterR>
bool operator>(const ft::reverse_iterator<IterL> &x,
			   const ft::reverse_iterator<IterR> &y) { return x.base() < y.base(); }
template <typename IterL, typename IterR>
bool operator<=(const ft::reverse_iterator<IterL> &x,
				const ft::reverse_iterator<IterR> &y) { return !(x > y); }
template <typename IterL, typename IterR>
bool operator>=(const ft::reverse_iterator<IterL> &x,
				const ft::reverse_iterator<IterR> &y) { return !(x < y); }
template <typename IterL, typename IterR>
typename ft::reverse_iterator<IterL>::difference_type operator-(
	const ft::reverse_iterator<IterL> &x,
	const ft::reverse_iterator<IterR> &y) { return y.base() - x.base(); }
