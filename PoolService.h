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
public:
	using del = std::function<void(const Pooled*)>;
private:
	using parent = std::unique_ptr<Pooled, del>;
public:
	struct PooledPtr : public parent { //pour que les méchants gens me sabotent pas mon code avec ptr.reset(ptr.get())
		PooledPtr() = default;
		PooledPtr(Pooled* p, del del) : parent{ p, del } {};
		void reset() { parent::reset(nullptr); }
	private:
		Pooled* release() { parent::reset(nullptr); return nullptr; }

	};
	using pooled_ptr = PooledPtr;
	class NoneAvailable {};
private:
	using pointer = std::unique_ptr<Pooled>;
	using pool = std::vector<pointer>;
	using iterator = typename pool::iterator;
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

	constexpr bool resize(size_t newSize)
	{
		static_assert(newSize > pool_.size());
		for (int i = 0; i < newSize - pool_.size(); ++i)
		{
			pool_.emplace_back(std::make_unique<Pooled>(args...));
		}
		return true;
	}


};

#endif // !POOLSERVICE_H
