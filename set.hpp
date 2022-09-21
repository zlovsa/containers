#pragma once
#include "rbtree.hpp"

namespace ft
{
	template <class Key, class Compare = std::less<Key>,
			  class Allocator = std::allocator<Key> >
	class set;
}

template <class Key, class Compare, class Allocator>
class ft::set
{
public:
	typedef Key key_type;
	typedef Key value_type;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef Compare key_compare;
	typedef Compare value_compare;
	typedef Allocator allocator_type;
	typedef value_type &reference;
	typedef const value_type &const_reference;
	typedef typename Allocator::pointer pointer;
	typedef typename Allocator::const_pointer const_pointer;

	typedef typename rbtree<value_type, value_compare>::const_iterator iterator;
	typedef typename rbtree<value_type, value_compare>::const_iterator const_iterator;
	typedef typename rbtree<value_type, value_compare>::const_reverse_iterator reverse_iterator;
	typedef typename rbtree<value_type, value_compare>::const_reverse_iterator const_reverse_iterator;

	iterator begin() { return _rbt.begin(); }
	const_iterator begin() const { return _rbt.begin(); }
	iterator end() { return _rbt.end(); }
	const_iterator end() const { return _rbt.end(); }
	reverse_iterator rbegin() { return _rbt.rbegin(); }
	const_reverse_iterator rbegin() const { return _rbt.rbegin(); }
	reverse_iterator rend() { return _rbt.rend(); }
	const_reverse_iterator rend() const { return _rbt.rend(); }

	set(){}
	explicit set(const Compare &comp,
				 const Allocator &alloc = Allocator()) : _alloc(alloc),
														 _comp(comp) {}
	template <class InputIt>
	set(InputIt first, InputIt last,
		const Compare &comp = Compare(),
		const Allocator &alloc = Allocator()) : _alloc(alloc),
												_comp(comp)
	{
		while (first != last)
		{
			insert(*first);
			++first;
		}
	}
	set(const set &other) : _rbt(other._rbt) {}
	~set(){}
	set &operator=(const set &other)
	{
		if (this == &other)
			return (*this);
		_rbt = other._rbt;
		return (*this);
	}
	allocator_type get_allocator() const { return _alloc; }

	bool empty() const { return _rbt.empty(); }
	size_type size() const { return _rbt.size(); }
	size_type max_size() const { return _rbt.max_size(); }
	void clear() { _rbt.clear(); }
	pair<iterator, bool> insert(const value_type &value)
	{
		pair<typename rbtree<value_type, value_compare>::rbnode *, bool> p = _rbt.insert(value, NULL);
		return make_pair(iterator(_rbt, p.first), p.second);
	}
	iterator insert(iterator hint, const value_type &value)
	{
		pair<typename rbtree<value_type, value_compare>::rbnode *, bool> p = _rbt.insert(value, hint._node);
		return iterator(_rbt, p.first);
	}
	template <class InputIt>
	void insert(InputIt first, InputIt last)
	{
		while (first != last)
		{
			insert(*first);
			++first;
		}
	}
	void erase(iterator pos) { _rbt.erase(pos); }
	void erase(iterator first, iterator last)
	{
		while (first != last)
		{
			iterator t = first;
			++t;
			erase(first);
			first = t;
		}
	}
	size_type erase(const key_type &key)
	{
		iterator pos = find(key);
		if (pos == end())
			return 0;
		erase(pos);
		return 1;
	}
	void swap(set &other) { _rbt.swap(other._rbt); }

	size_type count(const Key &key) const { return (!!_rbt.find(key)); }
	iterator find(const Key &key) { return iterator(_rbt, _rbt.find(key)); }
	const_iterator find(const Key &key) const { return const_iterator(_rbt, _rbt.find(key)); }
	pair<iterator, iterator> equal_range(const Key &key)
	{
		return make_pair(iterator(_rbt, _rbt.lower_bound(key)),
						 iterator(_rbt, _rbt.upper_bound(key)));
	}
	pair<const_iterator, const_iterator> equal_range(const Key &key) const
	{
		return make_pair(const_iterator(_rbt, _rbt.lower_bound(key)),
						 const_iterator(_rbt, _rbt.upper_bound(key)));
	}
	iterator lower_bound(const Key &key) { return iterator(_rbt, _rbt.lower_bound(key)); }
	const_iterator lower_bound(const Key &key) const { return const_iterator(_rbt, _rbt.lower_bound(key)); }
	iterator upper_bound(const Key &key) { return iterator(_rbt, _rbt.upper_bound(key)); }
	const_iterator upper_bound(const Key &key) const { return const_iterator(_rbt, _rbt.upper_bound(key)); }

	key_compare key_comp() const { return key_compare(); }
	value_compare value_comp() const { return value_compare(); }

private:
	Allocator _alloc;
	Compare _comp;
	rbtree<value_type, value_compare> _rbt;
};

template <class Key, class Compare, class Alloc>
bool operator==(const ft::set<Key, Compare, Alloc> &lhs,
				const ft::set<Key, Compare, Alloc> &rhs)
{
	if (lhs.size() != rhs.size())
		return false;
	typename ft::set<Key, Compare, Alloc>::const_iterator itl = lhs.begin();
	typename ft::set<Key, Compare, Alloc>::const_iterator itr = rhs.begin();
	while (itl != lhs.end() && *itl == *itr)
	{
		++itl;
		++itr;
	}
	return (itl == lhs.end());
}

template <class Key, class Compare, class Alloc>
bool operator!=(const ft::set<Key, Compare, Alloc> &lhs,
				const ft::set<Key, Compare, Alloc> &rhs) { return (!(lhs == rhs)); }

template <class Key, class Compare, class Alloc>
bool operator<(const ft::set<Key, Compare, Alloc> &lhs,
			   const ft::set<Key, Compare, Alloc> &rhs)
{
	typename ft::set<Key, Compare, Alloc>::const_iterator itl = lhs.begin();
	typename ft::set<Key, Compare, Alloc>::const_iterator itr = rhs.begin();
	while (itl != lhs.end() && itr != rhs.end() && *itl == *itr)
	{
		++itl;
		++itr;
	}
	return (itl == lhs.end() && itr != rhs.end()) || (itl != lhs.end() && itr != rhs.end() && *itl < *itr);
}

template <class Key, class Compare, class Alloc>
bool operator<=(const ft::set<Key, Compare, Alloc> &lhs,
				const ft::set<Key, Compare, Alloc> &rhs) { return (!(rhs < lhs)); }

template <class Key, class Compare, class Alloc>
bool operator>(const ft::set<Key, Compare, Alloc> &lhs,
			   const ft::set<Key, Compare, Alloc> &rhs) { return (rhs < lhs); }

template <class Key, class Compare, class Alloc>
bool operator>=(const ft::set<Key, Compare, Alloc> &lhs,
				const ft::set<Key, Compare, Alloc> &rhs) { return (!(lhs < rhs)); }

template <class Key, class Compare, class Alloc>
void swap(ft::set<Key, Compare, Alloc> &lhs,
		  ft::set<Key, Compare, Alloc> &rhs) { lhs.swap(rhs); }
