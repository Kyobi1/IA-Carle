#include "Graph.h"

void Graph::init(const SInitData& initData)
{
	for (int i = 0; i < initData.tileInfoArraySize; ++i) {
		graphKey cell(initData.tileInfoArray[i].q, initData.tileInfoArray[i].r);
		std::vector<graphKey> neighbors = cell.getNeighbors();
		for (int j = 0; j < initData.objectInfoArraySize; ++j) {
			if (initData.objectInfoArray[j].q == cell.q && initData.objectInfoArray[j].r == cell.r) {
				graphKey objectCell(initData.objectInfoArray[j].q, initData.objectInfoArray[j].r);
			}
		}
	}
}
