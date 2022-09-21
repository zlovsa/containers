#pragma once
#include <iostream>
#include "pair.hpp"
#include "reverse_iterator.hpp"

namespace ft
{
	template <class Key, class Compare = std::less<Key>,
			  class Allocator = std::allocator<Key> >
	class rbtree;
}

template <class Key, class Compare, class Allocator>
class ft::rbtree
{
public:
	enum nodecolor { BLACK, RED };
	typedef struct node	{
		node(Key key) : color(RED),
						parent(NULL),
						left(NULL),
						right(NULL),
						key(key) {}
		nodecolor color;
		struct node *parent;
		struct node *left;
		struct node *right;
		Key key;
		struct node *next()
		{
			struct node *n = this;
			if (right)
			{
				n = right;
				while (n->left)
					n = n->left;
			}
			else
			{
				while (n->parent && n == n->parent->right)
					n = n->parent;
				n = n->parent;
			}
			return n;
		}
		struct node *prev()
		{
			struct node *n = this;
			if (left)
			{
				n = left;
				while (n->right)
					n = n->right;
			}
			else
			{
				while (n->parent && n == n->parent->left)
					n = n->parent;
				n = n->parent;
			}
			return n;
		}
	} rbnode;
	typedef std::size_t size_type;

	class const_iterator
	{
	public:
		typedef typename Allocator::difference_type difference_type;
		typedef typename Allocator::value_type value_type;
		typedef typename Allocator::const_reference reference;
		typedef typename Allocator::const_pointer pointer;
		typedef std::bidirectional_iterator_tag iterator_category;

		const_iterator() : _node(NULL), _rbt(NULL) {}
		const_iterator(const rbtree<value_type, Compare> &rbt,
					   typename rbtree<value_type, Compare>::rbnode *node = NULL) : _node(node),
																					_rbt(&rbt) {}
		const_iterator(const const_iterator &other) : _node(other._node),
													  _rbt(other._rbt) {}
		~const_iterator() {}

		const_iterator &operator=(const const_iterator &other)
		{
			_node = other._node;
			_rbt = other._rbt;
			return *this;
		}
		bool operator==(const const_iterator &other) const { return (_node == other._node); }
		bool operator!=(const const_iterator &other) const { return (_node != other._node); }
		bool operator<(const const_iterator &other) const
		{
			return ((_node && !other._node) || (_comp(_node, other._node)));
		}
		bool operator>(const const_iterator &other) const
		{
			return ((!_node && other._node) || (_comp(other._node, _node)));
		}
		bool operator<=(const const_iterator &other) const
		{
			return (!(*this > other));
		}
		bool operator>=(const const_iterator &other) const
		{
			return (!(*this < other));
		}

		const_iterator &operator++()
		{
			_node = _node ? _node->next() : _node;
			return (*this);
		}
		const_iterator operator++(int)
		{
			const_iterator t(*this);
			_node = _node ? _node->next() : _node;
			return t;
		}
		const_iterator &operator--()
		{
			if (_node)
				_node = _node->prev();
			else
				_node = _rbt ? _rbt->mostright() : _node;
			return (*this);
		}
		const_iterator operator--(int)
		{
			const_iterator t(*this);
			if (_node)
				_node = _node->prev();
			else
				_node = _rbt ? _rbt->mostright() : _node;
			return t;
		}

		const reference operator*() const { return _node->key; }
		pointer operator->() const { return &_node->key; }
		reference operator[](size_type n) const
		{
			typename rbtree<value_type, Compare>::rbnode *c = _node;
			while (n-- && c)
				c = c->next();
			return c->key;
		}
		typename rbtree<value_type, Compare>::rbnode *_node;

	protected:
		const rbtree<value_type, Compare> *_rbt;
		Compare _comp;
	};

	class iterator : public const_iterator
	{
	private:
		typedef const_iterator cit;

	public:
		typedef typename Allocator::difference_type difference_type;
		typedef typename Allocator::value_type value_type;
		typedef typename Allocator::reference reference;
		typedef typename Allocator::pointer pointer;
		typedef std::bidirectional_iterator_tag iterator_category;

		iterator() : cit() {}
		iterator(const rbtree<value_type, Compare> &rbt,
				 typename rbtree<value_type, Compare>::rbnode *node = NULL) : cit(rbt, node) {}
		iterator(const iterator &other) : cit(other) {}
		~iterator() {}

		iterator &operator=(const iterator &other)
		{
			cit::operator=(other);
			return *this;
		}
		iterator &operator++()
		{
			cit::operator++();
			return (*this);
		}
		iterator operator++(int)
		{
			iterator t(*this);
			cit::operator++(0);
			return t;
		}
		iterator &operator--()
		{
			cit::operator--();
			return (*this);
		}
		iterator operator--(int)
		{
			iterator t(*this);
			cit::operator--(0);
			return t;
		}

		reference operator*() const { return cit::_node->key; }
		pointer operator->() const { return &cit::_node->key; }
		reference operator[](size_type n) const { return *(cit::base() + n); }
	};

	rbtree() : _root(NULL){}
	~rbtree(){ free_node(_root); }
	rbtree(const rbtree &other) { _root = copy_node(other._root); }
	rbtree &operator=(const rbtree &other)
	{
		if(this == &other)
			return (*this);
		free_node(_root);
		_root = copy_node(other._root);
		return (*this);
	}
	pair<rbnode*, bool> insert(Key key, rbnode* hint)
	{
		rbnode *c = _root;
		if (hint)
		{
			rbnode *hnxt = hint->next();
			if(_comp(hint->key,key) && (!hnxt || _comp(key,hnxt->key)))
				c = hint->right ? hnxt : hint;
		}
		while (c)
			if (_comp(key, c->key))
			{
				if (!c->left)
					break;
				c = c->left;
			}
			else if (_comp(c->key, key))
			{
				if (!c->right)
					break;
				c = c->right;
			}
			else
				return make_pair(c, false);
		rbnode *n = _node_alloc.allocate(1);
		new (n) rbnode(key);
		n->parent = c;
		if (c && _comp(key, c->key))
			c->left = n;
		else if (c && _comp(c->key, key))
			c->right = n;
		insert_case1(n);
		_root = get_root(n);
		return make_pair(n, true);
	}
	void swap(rbtree &other) { std::swap(_root, other._root); }
	rbnode *mostleft() const
	{
		rbnode *n = _root;
		while (n && n->left)
			n = n->left;
		return n;
	}
	rbnode *mostright() const
	{
		rbnode *n = _root;
		while (n && n->right)
			n = n->right;
		return n;
	}
	const rbnode *getRoot() const { return _root; }
	rbnode *getRoot() { return _root; }
	void erase(const_iterator pos)
	{
		if (pos._node)
			delnode(pos._node);
	}
	void delnode(rbnode *n)
	{
		if(n->left && n->right)
		{
			rbnode *p = n->parent;
			rbnode *nl = n->left;
			rbnode *nr = n->right;
			rbnode *s = n->next();
			rbnode *sp = s->parent;
			rbnode *sr = s->right;
			if(p && n==p->left)
				p->left = s;
			else if(p && n==p->right)
				p->right = s;
			s->parent = p;
			s->left = nl;
			if(nl)
				nl->parent = s;
			if(s==nr)
			{
				s->right = n;
				n->parent = s;
			}else{
				s->right = nr;
				nr->parent = s;
				sp->left = n;
				n->parent = sp;
			}
			n->right = sr;
			if(sr)
				sr->parent = n;
			n->left = NULL;
			_root = get_root(s);
		}
		delete_one_child(n);
	}
	rbnode *find(Key key) const
	{
		rbnode *n = _root;
		while (n)
			if (_comp(key, n->key))
				n = n->left;
			else if (_comp(n->key, key))
				n = n->right;
			else
				break;
		return n;
	}
	bool empty() const { return _root == NULL; }
	std::size_t size() const { return size(_root); }
	void clear()
	{
		free_node(_root);
		_root = NULL;
	}
	rbnode *lower_bound(Key key) const
	{
		rbnode *n = _root, *p = NULL;
		while (n)
			if (_comp(key, n->key))
			{
				p = n;
				n = n->left;
			}
			else if (_comp(n->key, key))
				n = n->right;
			else {
				p = n;
				break;
			}
		return p;
	}
	rbnode *upper_bound(Key key) const
	{
		rbnode *n = _root, *p = NULL;
		while (n)
			if (_comp(key, n->key))
			{
				p = n;
				n = n->left;
			}
			else
				n = n->right;
		return p;
	}
	std::size_t max_size() const { return _node_alloc.max_size(); }
	iterator begin() { return iterator(*this, mostleft()); }
	const_iterator begin() const { return const_iterator(*this, mostleft()); }
	iterator end() { return iterator(*this); }
	const_iterator end() const { return const_iterator(*this); }
	typedef typename ft::reverse_iterator<iterator> reverse_iterator;
	typedef typename ft::reverse_iterator<const_iterator> const_reverse_iterator;
	reverse_iterator rbegin() { return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

private:
	rbnode *_root;
	Compare _comp;
	typename Allocator::template rebind<rbnode>::other _node_alloc;

	rbnode *copy_node(rbnode *n)
	{
		if (!n)
			return n;
		rbnode *r = _node_alloc.allocate(1);
		new (r) rbnode(n->key);
		r->parent = NULL;
		r->left = copy_node(n->left);
		if(r->left)
			r->left->parent = r;
		r->right = copy_node(n->right);
		if (r->right)
			r->right->parent = r;
		r->color = n->color;
		return r;
	}
	rbnode *grandparent(rbnode *n)
	{
		if ((n != NULL) && (n->parent != NULL))
			return n->parent->parent;
		else
			return NULL;
	}
	rbnode *uncle(rbnode *n)
	{
		rbnode *g = grandparent(n);
		if (g == NULL)
			return NULL;
		if (n->parent == g->left)
			return g->right;
		else
			return g->left;
	}
	void rotate_left(rbnode *n)
	{
		rbnode *pivot = n->right;
		pivot->parent = n->parent;
		if (n->parent != NULL)
		{
			if (n->parent->left == n)
				n->parent->left = pivot;
			else
				n->parent->right = pivot;
		} else
			_root = pivot;
		n->right = pivot->left;
		if (pivot->left != NULL)
			pivot->left->parent = n;
		n->parent = pivot;
		pivot->left = n;
	}
	void rotate_right(rbnode *n)
	{
		rbnode *pivot = n->left;
		pivot->parent = n->parent;
		if (n->parent != NULL)
		{
			if (n->parent->left == n)
				n->parent->left = pivot;
			else
				n->parent->right = pivot;
		} else
			_root = pivot;
		n->left = pivot->right;
		if (pivot->right != NULL)
			pivot->right->parent = n;
		n->parent = pivot;
		pivot->right = n;
	}
	void insert_case5(rbnode *n)
	{
		rbnode *g = grandparent(n);
		n->parent->color = BLACK;
		g->color = RED;
		if ((n == n->parent->left) && (n->parent == g->left))
			rotate_right(g);
		else
			rotate_left(g);
	}
	void insert_case4(rbnode *n)
	{
		rbnode *g = grandparent(n);
		if ((n == n->parent->right) && (n->parent == g->left))
		{
			rotate_left(n->parent);
			n = n->left;
		}
		else if ((n == n->parent->left) && (n->parent == g->right))
		{
			rotate_right(n->parent);
			n = n->right;
		}
		insert_case5(n);
	}
	void insert_case3(rbnode *n)
	{
		rbnode *u = uncle(n), *g;
		if ((u != NULL) && (u->color == RED))
		{
			n->parent->color = BLACK;
			u->color = BLACK;
			g = grandparent(n);
			g->color = RED;
			insert_case1(g);
		}
		else
			insert_case4(n);
	}
	void insert_case2(rbnode *n)
	{
		if (n->parent->color == BLACK)
			return;
		else
			insert_case3(n);
	}
	void insert_case1(rbnode *n)
	{
		if (n->parent == NULL)
			n->color = BLACK;
		else
			insert_case2(n);
	}
	void print_tree(const rbnode *root, int lvl)
	{
		if (!root)
			return;
		print_tree(root->right, lvl + 1);
		for (int i = 0; i < lvl * 4; i++)
			std::cout << " ";
		std::cout << root->key << (root->color == RED ? "r" : "b") << std::endl;
		print_tree(root->left, lvl + 1);
	}
	rbnode *sibling(rbnode *n)
	{
		if (n == n->parent->left)
			return n->parent->right;
		else
			return n->parent->left;
	}
	void replace_node(node *n, node *child)
	{
		if(child)
			child->parent = n->parent;
		if (n->parent)
		{
			if (n == n->parent->left)
				n->parent->left = child;
			else
				n->parent->right = child;
		}else
			_root = child;
	}
	void delete_case6(rbnode *n)
	{
		rbnode *s = sibling(n);
		s->color = n->parent->color;
		n->parent->color = BLACK;
		if (n == n->parent->left)
		{
			s->right->color = BLACK;
			rotate_left(n->parent);
		}
		else
		{
			s->left->color = BLACK;
			rotate_right(n->parent);
		}
	}
	void delete_case5(rbnode *n)
	{
		rbnode *s = sibling(n);

		if (s->color == BLACK)
		{	
			if ((n == n->parent->left) &&
				(s->right->color == BLACK) &&
				(s->left->color == RED))
			{
				s->color = RED;
				s->left->color = BLACK;
				rotate_right(s);
			} else if ((n == n->parent->right) &&
					  (s->left->color == BLACK) &&
					  (s->right->color == RED))
			{
				s->color = RED;
				s->right->color = BLACK;
				rotate_left(s);
			}
		}
		delete_case6(n);
	}
	void delete_case4(rbnode *n)
	{
		rbnode *s = sibling(n);
		if ((n->parent->color == RED) &&
			(s->color == BLACK) &&
			(!s->left || s->left->color == BLACK) &&
			(!s->right || s->right->color == BLACK))
		{
			s->color = RED;
			n->parent->color = BLACK;
		}
		else
			delete_case5(n);
	}
	void delete_case3(rbnode *n)
	{
		rbnode *s = sibling(n);
		if ((n->parent->color == BLACK) &&
			(s->color == BLACK) &&
			(!s->left || s->left->color == BLACK) &&
			(!s->right || s->right->color == BLACK))
		{
			s->color = RED;
			delete_case1(n->parent);
		}
		else
			delete_case4(n);
	}
	void delete_case2(rbnode *n)
	{
		rbnode *s = sibling(n);
		if (s->color == RED)
		{
			n->parent->color = RED;
			s->color = BLACK;
			if (n == n->parent->left)
				rotate_left(n->parent);
			else
				rotate_right(n->parent);
		}
		// std::cout << "----------- rotates ----------" << std::endl;
		// print_tree(_root, 0);
		delete_case3(n);
	}
	void delete_case1(rbnode *n)
	{
		if (n->parent != NULL)
			delete_case2(n);
	}
	void delete_one_child(rbnode *n)
	{
		rbnode *child = n->right ? n->right : n->left;
		replace_node(n, child);
		// std::cout << "----------- replace_node ----------" << std::endl;
		// print_tree(_root, 0);
		if (n->color == BLACK && child)
		{
			if (child->color == RED)
				child->color = BLACK;
			else
				delete_case1(child);
		}
		n->~rbnode();
		_node_alloc.deallocate(n, 1);
	}
	rbnode *get_root(rbnode *n)
	{
		while (n && n->parent)
			n = n->parent;
		return n;
	}
	std::size_t size(rbnode *n) const
	{
		if (!n)
			return 0;
		return size(n->left) + 1 + size(n->right);
	}
	void free_node(rbnode *n)
	{
		if (!n)
			return;
		free_node(n->left);
		free_node(n->right);
		n->~rbnode();
		_node_alloc.deallocate(n, 1);
	}
};
