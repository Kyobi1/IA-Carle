#ifndef POOLSERVICE_H
#define POOLSERVICE_H

#include <functional>
#include <memory>
#include <vector>
#include <mutex>
#include <algorithm>
#include <iterator>

template <class Pooled, class Init>
class PoolService {
private:
	using del = std::function<void(const Pooled*)>;
	using parent = std::unique_ptr<Pooled, del>;
	struct PooledPtr : public parent { //pour que les méchants gens me sabotent pas mon code avec ptr.reset(ptr.get())
		PooledPtr() = default;
		PooledPtr(Pooled* p, del del) : parent{ p, del } {};
		void reset() { parent::reset(nullptr); }
		Pooled* release() { parent::reset(nullptr); return nullptr; }

	};
public:
	using pooled_ptr = PooledPtr;
	class NoneAvailable {};
	class NegativeIncreaseSize {};
private:
	using pointer = std::unique_ptr<Pooled>;
	using pool = std::vector<pointer>;
	using iterator = typename pool::iterator;
	using sizetype = int;
	iterator beginAvailable;

	Init init;

	pool pool_;
	std::mutex mutex;
public:

	template<class ... Args>
	PoolService(int poolSize, const Init& reset, const Args& ... args) : init{ reset }
	{
		for (int i = 0; i < poolSize; ++i)
		{
			pool_.emplace_back(std::make_unique<Pooled>(args...));
		}
		beginAvailable = std::begin(pool_);
	};

	template <class CustomInit>
	pooled_ptr request(const CustomInit& init) {

		if (beginAvailable == std::end(pool_)) throw NoneAvailable{};

		del del = [this](const Pooled* obj) {
			std::lock_guard<std::mutex> lock{ mutex };
			auto it = std::find_if(std::begin(pool_), beginAvailable, [&obj](const pointer& ptr) { return ptr.get() == obj; });
			if (it != std::end(pool_))
				std::iter_swap(it, --beginAvailable);
		};

		std::lock_guard<std::mutex> lock{ mutex };
		auto p = beginAvailable++->get();
		init(*p);
		return pooled_ptr{ p, del };
	}

	pooled_ptr request() {

		if (beginAvailable == std::end(pool_)) throw NoneAvailable{};

		del del = [this](const Pooled* obj) {
			std::lock_guard<std::mutex> lock{ mutex };
			auto it = std::find_if(std::begin(pool_), beginAvailable, [&obj](const pointer& ptr) { return ptr.get() == obj; });
			if (it != std::end(pool_))
				std::iter_swap(it, --beginAvailable);
		};
		std::lock_guard<std::mutex> lock{ mutex };
		auto p = beginAvailable++->get();
		init(*p);
		return pooled_ptr{ p, del };
	}

	template<class ... CtorArgs>
	void increaseSize(sizetype sizeIncr,const CtorArgs& ... args)
	{
		if (sizeIncr < 0) throw NegativeIncreaseSize{};
		for (sizetype i = 0; i < sizeIncr; ++i)
		{
			pool_.emplace_back(std::make_unique<Pooled>(args...));
		}
	}


};

#endif // !POOLSERVICE_H
