#include "HexCell.h"
#include "Globals.h"
#include "Logger.h"
#include <string>
#include <sstream>

const std::vector<HexCell> HexCell::neighbors = std::vector<HexCell>{ // Order : W - NW - NE - E - SE - SW - CENTER
	HexCell{0, -1}, HexCell{-1, 0}, HexCell{-1, 1},
	HexCell{0, 1}, HexCell{1, 0}, HexCell{1, -1},
	HexCell{0, 0}
};

HexCell::HexCell(int q, int r) : q{ q }, r{ r } {}

HexCell::HexCell(const STileInfo& info) : q{ info.q }, r{ info.r } {}

bool operator==(const HexCell& a, const HexCell& b) {
	return a.q == b.q && a.r == b.r;
}

bool operator!=(const HexCell& a, const HexCell& b) {
	return !(a == b);
}

HexCell operator+(const HexCell& a, const HexCell& b) {
	return HexCell{ a.q + b.q, a.r + b.r };
}

bool operator<(const HexCell& a, const HexCell& b) {
	if (a.q == b.q) return a.r < b.r;
	return a.q < b.q;
}

int HexCell::distanceTo(const HexCell& dest) const {
	return (std::abs(q - dest.q) + std::abs(r - dest.r) + std::abs(-(r + q) + dest.r + dest.q)) / 2;
}

std::vector<HexCell> HexCell::getNeighbors() const {
	std::vector<HexCell> v;
	for (int dir = EHexCellDirection::W; dir != CENTER; ++dir) {
		v.push_back(*this + neighbors[dir]);
	}
	return v;
}

EHexCellDirection HexCell::directionTo(const HexCell& other) const {
	HexCell toFind{ other.q - q, other.r - r };
	auto it = std::find(std::begin(neighbors), std::end(neighbors), toFind);
	if (it != std::end(neighbors)) {
		return static_cast<EHexCellDirection>(std::distance(std::begin(neighbors), it));
	}
	else
	{
		throw std::exception("Pas une direction");
	}
}

HexCell HexCell::neighborFromDirection(EHexCellDirection const& direction) const
{
	return *this + neighbors[direction];
}

EHexCellDirection HexCell::oppositeDirection(EHexCellDirection const& direction)
{
	return static_cast<EHexCellDirection>((direction + 3) % 6);
}

bool HexCell::isOutOfBound(int nbColums, int nbRows) const
{
	if (q < 0 || q >= nbRows)
		return true;
	int offset = q / 2 + q % 2;
	return  r < - offset || r >= nbColums - offset;
}

void HexCell::debug(Logger& log) const
{
	std::stringstream ss;
	ss << "q : " << q << "\tr : " << r << std::endl;
	log.Log(ss.str());
}
