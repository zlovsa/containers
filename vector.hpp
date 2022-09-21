#pragma once
#include <sstream>
#include "traits.hpp"
#include "reverse_iterator.hpp"

namespace ft
{
	template <class T, class A = std::allocator<T> >
	class vector;
}

template <class T, class A>
class ft::vector
{
public:
	typedef typename A::value_type value_type;
	typedef typename A::reference reference;
	typedef typename A::const_reference const_reference;
	typedef typename A::difference_type difference_type;
	typedef typename A::size_type size_type;

	class const_iterator
	{
	public:
		typedef typename A::difference_type difference_type;
		typedef typename A::value_type value_type;
		typedef typename A::const_reference reference;
		typedef typename A::const_pointer pointer;
		typedef std::random_access_iterator_tag iterator_category;

		const_iterator(T *ptr = NULL) : _ptr(ptr) {}
		const_iterator(const const_iterator &other) : _ptr(other._ptr) {}
		~const_iterator() {}

		const_iterator &operator=(const const_iterator &other) { _ptr = other._ptr; return *this; }
		bool operator==(const const_iterator &other) const { return _ptr == other._ptr; }
		bool operator!=(const const_iterator &other) const { return _ptr != other._ptr; }
		bool operator<(const const_iterator &other) const { return _ptr < other._ptr; }
		bool operator>(const const_iterator &other) const { return _ptr > other._ptr; }
		bool operator<=(const const_iterator &other) const { return _ptr <= other._ptr; }
		bool operator>=(const const_iterator &other) const { return _ptr >= other._ptr; }

		const_iterator &operator++() { ++_ptr; return (*this); }
		const_iterator operator++(int) { const_iterator t(*this); ++_ptr; return t; }
		const_iterator &operator--() { --_ptr; return (*this); }
		const_iterator operator--(int) { const_iterator t(*this); --_ptr; return t; }
		const_iterator &operator+=(size_type n) { _ptr += n; return *this; }
		const_iterator operator+(size_type n) const { return const_iterator(_ptr + n); }
		friend const_iterator operator+(size_type n, const const_iterator &other) { return const_iterator(other._ptr + n); }
		const_iterator &operator-=(size_type n) { _ptr -= n; return *this; }
		const_iterator operator-(size_type n) const { return iterator(_ptr - n); }
		difference_type operator-(const_iterator const &other) const { return _ptr - other._ptr; }

		reference operator*() const { return *_ptr; }
		pointer operator->() const { return _ptr; }
		reference operator[](size_type n) const { return *(_ptr + n); }

	protected:
		T *_ptr;
	};

	class iterator : public const_iterator
	{
	private:
		typedef const_iterator cit;
	public:
		typedef typename A::difference_type difference_type;
		typedef typename A::value_type value_type;
		typedef typename A::reference reference;
		typedef typename A::pointer pointer;
		typedef std::random_access_iterator_tag iterator_category;

		iterator(T *ptr = NULL) : cit(ptr) {}
		iterator(const iterator &other) : cit(other) {}
		~iterator() {}

		iterator &operator=(const iterator &other) { cit::operator=(other); return *this; }
		iterator &operator++() { cit::operator++(); return (*this); }
		iterator operator++(int) { iterator t(*this); cit::operator++(0); return t; }
		iterator &operator--() { cit::operator--(); return (*this); }
		iterator operator--(int) { iterator t(*this); cit::operator--(0); return t; }
		iterator &operator+=(difference_type n) { cit::operator+=(n); return *this; }
		iterator operator+(difference_type n) const { return iterator(cit::_ptr + n); }
		friend iterator operator+(difference_type n, const iterator &other) { return iterator(other._ptr + n); }
		iterator &operator-=(difference_type n) { cit::operator-=(n); return *this; }
		iterator operator-(difference_type n) const { return iterator(cit::_ptr - n); }
		difference_type operator-(const_iterator const &other) const { return cit::operator-(other); }

		reference operator*() const { return *cit::_ptr; }
		pointer operator->() const { return cit::_ptr; }
		reference operator[](size_type n) const { return *(cit::_ptr + n); }
	};

	iterator begin() { return iterator(_values); }
	const_iterator begin() const { return const_iterator(_values); }
	iterator end() { return iterator(_values + _size); }
	const_iterator end() const { return const_iterator(_values + _size); }

	typedef typename ft::reverse_iterator<iterator> reverse_iterator;
	typedef typename ft::reverse_iterator<const_iterator> const_reverse_iterator;

	reverse_iterator rbegin() { return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
	reverse_iterator rend()	{ return reverse_iterator(begin());	}
	const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

	vector() : _capacity(0),
			   _size(0),
			   _values(NULL) {}
	explicit vector(size_type count) : _capacity(count),
									   _size(count),
									   _values(_alloc.allocate(count)) {}
	vector(const vector &other) : _capacity(0),
								  _size(0),
								  _values(NULL) { *this = other; }
	template <class InputIt>
	vector(InputIt first, InputIt last,
		   const A &alloc = std::allocator<T>()) : _alloc(alloc),
												   _capacity(0),
												   _size(0),
												   _values(NULL) { assign(first, last); }
	~vector()
	{
		for (size_type i = 0; i < _size; i++)
			_alloc.destroy(_values + i);
		if (_values)
			_alloc.deallocate(_values, _capacity);
	}
	vector &operator=(const vector &other)
	{
		if (this == &other)
			return *this;
		clear();
		reserve(other._size);
		_size = other._size;
		for (size_type i = 0; i < _size; i++)
			_values[i] = other._values[i];
		return *this;
	}
	A get_allocator() const { return _alloc; }
	T *data() { return _values;	}
	T const *data() const { return _values; }
	size_type size() const { return _size; }
	size_type capacity() const { return _capacity; }
	size_type max_size() const { return _alloc.max_size(); }
	T &front() { return _values[0]; }
	T const &front() const { return _values[0]; }
	T &back() { return _values[_size - 1]; }
	T const &back() const { return _values[_size - 1]; }
	T &operator[](size_type pos) { return _values[pos]; }
	T const &operator[](size_type pos) const { return _values[pos]; }
	T &at(size_type pos)
	{
		if (pos < _size)
			return _values[pos];
		std::stringstream ss;
		ss << "vector::_M_range_check: __n (which is " << pos
		   << ") >= this->size() (which is " << _size << ")";
		throw std::out_of_range(ss.str());
	}
	T const &at(size_type pos) const
	{
		if (pos < _size)
			return _values[pos];
		std::stringstream ss;
		ss << "vector::_M_range_check: __n (which is " << pos
		   << ") >= this->size() (which is " << _size << ")";
		throw std::out_of_range(ss.str());
	}
	bool empty() const { return _size == 0; }
	void reserve(size_type new_cap)
	{
		if (new_cap <= _capacity)
			return;
		new_cap = std::max(new_cap, _size * 2);
		T *_new_values = _alloc.allocate(new_cap);
		for (size_type i = 0; i < _size; i++)
			_new_values[i] = _values[i];
		if (_values)
			_alloc.deallocate(_values, _capacity);
		_values = _new_values;
		_capacity = new_cap;
	}
	void clear()
	{
		for (size_type i = 0; i < _size; i++)
			_alloc.destroy(_values + i);
		_size = 0;
	}
	void push_back(const T &value)
	{
		if (_size == _capacity)
			reserve(_size ? _size * 2 : 1);
		_values[_size++] = value;
	}
	void pop_back() { _size--; }
	void resize(size_type count, T value = T())
	{
		reserve(count);
		while (_size < count)
			_values[_size++] = value;
		_size = count;
	}
	void swap(vector &other)
	{
		std::swap(_values, other._values);
		std::swap(_size, other._size);
		std::swap(_capacity, other._capacity);
	}
	void assign(size_type count, const T &value)
	{
		clear();
		resize(count, value);
	}
	template <class InputIt>
	void assign(InputIt first,
				typename enable_if<!is_integral<InputIt>::value, InputIt>::type last)
	{
		clear();
		size_type t = std::distance(first, last);
		reserve(t);
		_size = t;
		int i = 0;
		while (first != last)
			_values[i++] = *first++;
	}
	iterator insert(iterator position, const T &val)
	{
		size_type idx = std::distance(begin(), position);
		reserve(_size + 1);
		for (size_type i = _size; i > idx; i--)
			_values[i] = _values[i - 1];
		_size++;
		_values[idx] = val;
		return iterator(_values + idx);
	}
	void insert(iterator position, size_type n, const T &val)
	{
		size_type idx = std::distance(begin(), position);
		reserve(_size + n);
		for (size_type i = _size; i > idx; i--)
			_values[i - 1 + n] = _values[i - 1];
		_size += n;
		for (size_type i = 0; i < n; i++)
			_values[idx + i] = val;
	}
	template <class InputIt>
	void insert(iterator position, InputIt first,
				typename enable_if<!is_integral<InputIt>::value, InputIt>::type last)
	{
		size_type idx = std::distance(begin(), position);
		size_type n = std::distance(first, last);
		reserve(_size + n);
		for (size_type i = _size; i > idx; i--)
			_values[i - 1 + n] = _values[i - 1];
		_size += n;
		for (size_type i = 0; i < n; i++)
			_values[idx + i] = *first++;
	}
	iterator erase(iterator pos)
	{
		size_type idx = pos - begin();
		for (size_type i = idx; i < _size - 1; i++)
			_values[i] = _values[i + 1];
		_size--;
		return iterator(_values + idx);
	}
	iterator erase(iterator first, iterator last)
	{
		size_type idx = first - begin();
		size_type n = last - first;
		for (size_type i = idx; i < _size - n; i++)
			_values[i] = _values[i + n];
		_size -= n;
		return iterator(_values + idx);
	}

private:
	A _alloc;
	size_type _capacity;
	size_type _size;
	T *_values;
};

template <typename T, typename A>
bool operator==(const ft::vector<T, A> &lhs, const ft::vector<T, A> &rhs)
{
	unsigned sz = lhs.size();
	if (sz != rhs.size())
		return false;
	unsigned i = 0;
	while (i < sz && lhs[i] == rhs[i])
		i++;
	return i == sz;
}

template <typename T, typename A>
bool operator!=(const ft::vector<T, A> &lhs,
				const ft::vector<T, A> &rhs) {	return !(lhs == rhs); }

template <typename T, typename A>
bool operator<(const ft::vector<T, A> &lhs, const ft::vector<T, A> &rhs)
{
	unsigned i = 0, lsz = lhs.size(), rsz = rhs.size();
	while (i < lsz && i < rsz && lhs[i] == rhs[i])
		i++;
	return (i == lsz && i < rsz) || (i < lsz && i < rsz && lhs[i] < rhs[i]);
}

template <typename T, typename A>
bool operator>=(const ft::vector<T, A> &lhs,
				const ft::vector<T, A> &rhs) { return !(lhs < rhs); }

template <typename T, typename A>
bool operator>(const ft::vector<T, A> &lhs,
			   const ft::vector<T, A> &rhs) { return (rhs < lhs); }

template <typename T, typename A>
bool operator<=(const ft::vector<T, A> &lhs,
				const ft::vector<T, A> &rhs) { return !(rhs < lhs); }

template <typename T, typename A>
void swap(ft::vector<T, A> &lhs,
		  ft::vector<T, A> &rhs) { lhs.swap(rhs); }
