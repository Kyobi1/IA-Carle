#ifndef PRIORITY_HEAP_H
#define PRIORITY_HEAP_H
#include <vector>
#include <algorithm>

template<class T1>
class PriorityHeap {
	std::vector<T1*> tree;

public:
	PriorityHeap& operator+=(T1& elem)
	{
		tree.push_back(&elem);
		std::push_heap(std::begin(tree), std::end(tree), [](T1* a, T1* b) { 
			return *b < *a; 
			});

		return *this;
	}

	PriorityHeap& operator-=(T1& elem)
	{
		auto result = std::find(std::begin(tree), std::begin(tree), &elem);
		size_t k = std::distance(std::begin(tree), result);
		std::pop_heap(std::begin(tree) + k, std::end(tree), [](T1* a, T1* b) { return *b < *a; });
		tree.pop_back();

		return *this;
	}

	T1& smallestElement()
	{
		return *tree.front();
	};

	size_t count() const {
		return tree.size();
	}

	template<class T2>
	auto find(T2 const& elem)
	{
		return std::find_if(std::begin(tree), std::end(tree), [elem](auto& node) { return &node == &elem; });
	}

	auto find(T1 const& elem)
	{
		return std::find(std::begin(tree), std::end(tree), &elem);
	}

	auto begin()
	{
		return tree.begin();
	}

	auto end()
	{
		return tree.end();
	}

	void clear()
	{
		tree.clear();
	};

	bool empty()
	{
		return !tree.size();
	}


};

#endif // !PRIORITY_HEAP_H