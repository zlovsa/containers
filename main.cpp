/* **************************************************************************

Compile && run (WSL Ubuntu 20.04):

FT:
clang++ -Wall -Wextra -Werror -std=c++98 -pedantic main.cpp && ./a.out 123 > ft.txt

STL:
clang++ -Wall -Wextra -Werror -std=c++98 -pedantic -D DSTL main.cpp && ./a.out 123 > stl.txt

Check output:
diff ft.txt stl.txt

Check memory access (~2 times slower):
clang++ -Wall -Wextra -Werror -std=c++98 -pedantic -fsanitize=address main.cpp && ./a.out 123

Check leaks (~10 times slower):
clang++ -Wall -Wextra -Werror -std=c++98 -pedantic main.cpp && valgrind ./a.out 123

************************************************************************** */

#include <iostream>
#include <string>
#include <deque>
#include <ctime>
#include <list>
#ifdef DSTL //CREATE A REAL STL EXAMPLE
#define NMSP "STL"
#include <map>
#include <set>
#include <stack>
#include <vector>
namespace ft = std;
#else
#define NMSP "FT"
#include "map.hpp"
#include "set.hpp"
#include "stack.hpp"
#include "vector.hpp"
#endif

#include <stdlib.h>

#define MAX_RAM 4294967296
#define BUFFER_SIZE 4096
struct Buffer
{
	int idx;
	char buff[BUFFER_SIZE];
};

#define COUNT (MAX_RAM / (int)sizeof(Buffer))

template <typename T>
class MutantStack : public ft::stack<T>
{
public:
	MutantStack() {}
	MutantStack(const MutantStack<T> &src) { *this = src; }
	MutantStack<T> &operator=(const MutantStack<T> &rhs)
	{
		this->c = rhs.c;
		return *this;
	}
	~MutantStack() {}

	typedef typename ft::stack<T>::container_type::iterator iterator;

	iterator begin() { return this->c.begin(); }
	iterator end() { return this->c.end(); }
};

template <typename T>
void vec_print(ft::vector<T> const &vct, bool print_content = 1)
{
	std::cout << "size: " << vct.size() << " capacity: " << vct.capacity() << " max_size: " << vct.max_size() << std::endl;
	if (print_content)
	{
		typename ft::vector<T>::const_iterator it = vct.begin();
		typename ft::vector<T>::const_iterator ite = vct.end();
		std::cout << "Content is:" ;
		for (; it != ite; ++it)
			std::cout << *it << "; ";
		std::cout << std::endl;
	}
}

template <typename T>
void st_print(ft::stack<T> const &st, bool print_content = 1)
{
	std::cout << "size: " << st.size() << std::endl;
	if (print_content)
	{
		std::cout << "Content is: " << st.top() << std::endl;
	}
}

template <typename T>
std::string printPair(const T &iterator, bool nl = true, std::ostream &o = std::cout)
{
	o << "key: " << iterator->first << " | value: " << iterator->second;
	if (nl)
		o << std::endl;
	return ("");
}

template <typename T_MAP>
void map_print(T_MAP const &mp, bool print_content = 1)
{
	std::cout << "size: " << mp.size() << std::endl;
	std::cout << "max_size: " << mp.max_size() << std::endl;
	if (print_content)
	{
		typename T_MAP::const_iterator it = mp.begin(), ite = mp.end();
		std::cout << std::endl
				  << "Content is:" << std::endl;
		for (; it != ite; ++it)
			std::cout << "- " << printPair(it, false) << std::endl;
	}
}

template <typename T_SET>
void set_print(T_SET const &st, bool print_content = 1)
{
	std::cout << "size: " << st.size() << " max_size: " << st.max_size() << std::endl;
	if (print_content)
	{
		typename T_SET::const_iterator it = st.begin(), ite = st.end();
		std::cout << "Content is: ";
		for (; it != ite; ++it)
			std::cout << "; " << *it;
		std::cout << std::endl;
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: ./test seed" << std::endl;
		std::cerr << "Provide a seed please" << std::endl;
		std::cerr << "Count value:" << COUNT << std::endl;
		return 1;
	}
	std::cout << "------------- " NMSP " -------------" << std::endl;
	const int seed = atoi(argv[1]);
	srand(seed);
	const clock_t begin_time = clock();

	ft::vector<int> vec_def;
	vec_print(vec_def);
	ft::vector<int> vec_count(20);
	for (int i = 0; i < 20; i++)
		vec_count[i] = rand();
	vec_print(vec_count);
	const ft::vector<int> vec_copy(vec_count);
	vec_print(vec_copy);
	ft::vector<int> vec_iter(++(++vec_copy.begin()), --(--vec_copy.end()));
	vec_print(vec_iter);
	vec_def = vec_iter;
	vec_print(vec_def);
	std::allocator<int> alloc = vec_def.get_allocator();
	int *data_ptr = vec_count.data();
	std::cout << data_ptr[0] << std::endl;
	const int *data_ptr_const = vec_copy.data();
	std::cout << data_ptr_const[0] << std::endl;
	std::cout << vec_copy.size() << std::endl;
	std::cout << vec_iter.capacity() << std::endl;
	std::cout << vec_count.max_size() << std::endl;
	std::cout << vec_def.front() << std::endl;
	std::cout << vec_copy.front() << std::endl;
	std::cout << vec_def.back() << std::endl;
	std::cout << vec_copy.back() << std::endl;
	std::cout << vec_def[5] << std::endl;
	std::cout << vec_copy[5] << std::endl;
	std::cout << vec_def.at(6) << std::endl;
	std::cout << vec_copy.at(6) << std::endl;
	std::cout << vec_def.empty() << std::endl;
	vec_iter.reserve(500);
	vec_print(vec_iter);
	vec_iter.clear();
	vec_print(vec_iter);
	vec_iter.push_back(rand());
	vec_print(vec_iter);
	vec_iter.pop_back();
	vec_print(vec_iter);
	vec_iter.resize(20);
	vec_print(vec_iter);
	vec_iter.resize(40, rand());
	vec_print(vec_iter);
	vec_def.swap(vec_iter);
	vec_print(vec_iter);
	vec_print(vec_def);
	vec_def.assign(21, rand());
	vec_print(vec_def);
	vec_def.assign(vec_copy.begin(), vec_copy.end());
	vec_print(vec_def);
	vec_def.insert(vec_def.begin() + 5, rand());
	vec_print(vec_def);
	vec_def.insert(vec_def.begin() + 3, 3, rand());
	vec_print(vec_def);
	vec_def.insert(vec_def.begin() + 7, vec_copy.begin() + 2, vec_copy.begin() + 4);
	vec_print(vec_def);
	ft::vector<int>::iterator it = vec_def.erase(vec_def.begin());
	vec_print(vec_def);
	it = vec_def.erase(vec_def.end() - 2, vec_def.end());
	vec_print(vec_def);

	ft::stack<int> st_def;
	st_def.push(rand());
	st_def.push(rand());
	st_print(st_def);
	const ft::stack<int> st_copy(st_def);
	st_print(st_copy);
	st_def.push(rand());
	st_print(st_def);
	st_def = st_copy;
	st_print(st_def);
	std::cout << st_def.top() << std::endl;
	std::cout << st_copy.empty() << std::endl;
	std::cout << st_copy.size() << std::endl;
	st_def.pop();
	std::cout << st_def.top() << std::endl;
	st_print(st_def);

	ft::map<int, int> map_def;
	map_def[21] = rand();
	map_def[38] = rand();
	map_print(map_def);
	ft::map<int, int> map_iter(++map_def.begin(), map_def.end());
	map_print(map_iter);
	const ft::map<int, int> map_copy(map_def);
	map_print(map_copy);
	map_def = map_iter;
	map_print(map_def);
	std::cout << map_copy.at(21) << std::endl;
	std::cout << map_copy.empty() << std::endl;
	map_def.clear();
	map_print(map_def);
	map_def.insert(ft::make_pair(21, rand()));
	map_print(map_def);
	map_def.insert(map_def.begin(), ft::make_pair(31, rand()));
	map_print(map_def);
	map_iter.insert(map_def.begin(), map_def.end());
	map_print(map_iter);
	map_iter.erase(map_iter.begin());
	map_print(map_iter);
	map_iter.erase(++map_iter.begin(), map_iter.end());
	map_print(map_iter);
	map_iter.erase(map_iter.begin()->first);
	map_print(map_iter);
	map_iter.swap(map_def);
	map_print(map_iter);
	map_print(map_def);

	ft::set<int> set_def;
	set_def.insert(rand());
	set_def.insert(rand());
	set_print(set_def);
	ft::set<int> set_iter(++set_def.begin(), set_def.end());
	set_print(set_iter);
	const ft::set<int> set_copy(set_def);
	set_print(set_copy);
	set_def = set_iter;
	set_print(set_def);
	std::cout << set_copy.empty() << std::endl;
	set_def.clear();
	set_print(set_def);
	set_def.insert(rand());
	set_print(set_def);
	set_def.insert(set_def.begin(), rand());
	set_print(set_def);
	set_iter.insert(set_def.begin(), set_def.end());
	set_print(set_iter);
	set_iter.erase(set_iter.begin());
	set_print(set_iter);
	set_iter.erase(++set_iter.begin(), set_iter.end());
	set_print(set_iter);
	set_iter.erase(*set_iter.begin());
	set_print(set_iter);
	set_iter.swap(set_def);
	set_print(set_iter);
	set_print(set_def);

	ft::vector<std::string> vector_str;
	ft::vector<int> vector_int;
	ft::stack<int> stack_int;
	ft::vector<Buffer> vector_buffer;
	ft::stack<Buffer, std::deque<int> > stack_deq_buffer;
	ft::map<int, int> map_int;
	ft::set<int> set_int;

	for (int i = 0; i < COUNT; i++)
		vector_buffer.push_back(Buffer());

	for (int i = 0; i < COUNT; i++)
	{
		const int idx = rand() % COUNT;
		vector_buffer[idx].idx = 5;
	}
	ft::vector<Buffer>().swap(vector_buffer);

	try
	{
		for (int i = 0; i < COUNT; i++)
		{
			const int idx = rand() % COUNT;
			vector_buffer.at(idx);
			std::cerr << "Error: THIS VECTOR SHOULD BE EMPTY!!" << std::endl;
		}
	}
	catch (const std::exception &e)
	{
		//NORMAL ! :P
	}

	for (int i = 0; i < COUNT; ++i)
	{
		map_int.insert(ft::make_pair(rand(), rand()));
		set_int.insert(rand());
	}

	int sum = 0;
	for (int i = 0; i < 10000; i++)
	{
		int access = rand();
		sum += map_int[access];
	}
	std::cout << "should be constant with the same seed: " << sum << std::endl;

	{
		ft::map<int, int> copy = map_int;
	}
	MutantStack<char> iterable_stack;
	for (char letter = 'a'; letter <= 'z'; letter++)
		iterable_stack.push(letter);
	for (MutantStack<char>::iterator it = iterable_stack.begin(); it != iterable_stack.end(); it++)
	{
		std::cout << *it;
	}
	std::cout << std::endl;
	std::cout << float(clock() - begin_time) / CLOCKS_PER_SEC << " s" << std::endl;
	return (0);
}
