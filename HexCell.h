#ifndef HEXCELL_H
#define HEXCELL_H

#include <vector>


enum EHexCellDirection;
struct STileInfo;
class Logger;

struct HexCell {
	int q;
	int r;

	HexCell() = default;
	HexCell(int q, int r);
	HexCell(const STileInfo& info);

	friend bool operator==(HexCell const& a, HexCell const& b);
	friend bool operator!=(HexCell const& a, HexCell const& b);
	friend HexCell operator+(HexCell const& a, HexCell const& b);
	friend bool operator<(HexCell const& a, HexCell const& b);

	int distanceTo(HexCell const& dist) const;
	std::vector<HexCell> getNeighbors() const;

	EHexCellDirection directionTo(HexCell const& other) const;
	HexCell neighborFromDirection(EHexCellDirection const& direction) const;
	static EHexCellDirection oppositeDirection(EHexCellDirection const& direction);

	void debug(Logger& log) const;

private:

	static const std::vector<HexCell> neighbors;
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

#endif //HEXCELL_H