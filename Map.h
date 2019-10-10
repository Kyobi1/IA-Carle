#pragma once
#include <functional>


struct HexCell {
	int q;
	int r;
};

namespace std {
	template<>
	class hash<HexCell> {
	public:
		size_t operator()(const HexCell& s) const
		{
			size_t h1 = std::hash<int>()(s.q);
			size_t h2 = std::hash<int>()(s.r);
			return h1 ^ (h2 << 1);
		}
	};
}