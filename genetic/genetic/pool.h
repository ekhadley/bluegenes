#pragma once
#include <iostream>
#include "agent.h"

class pool {
public:
	std::vector<agent> genepool;
	vec2d startpos;
	int poolSize;
	int lifespan = -1;
	float limit = 1;
	pool() { startpos.x = 0; startpos.y = 0; poolSize = 0;}
	pool(float sx, float sy, int psize) {
		poolSize = psize;
		startpos.x = sx;
		startpos.y = sy;
		}
	void populate() {
		for (int i = 0; i < poolSize; i++) {
			genepool.push_back(agent(startpos.x, startpos.y, lifespan, limit));
		}
	}
	int* sorted() {
		int* ranks = new int[poolSize];
		for (int i = 0; i < poolSize; i++) { ranks[i] = i; }
		iSort(ranks);
		return ranks;
	}
private:
	void iSort(int* ranks) {
		int tmp, rank;
		int best, worst = 0;
		int end = poolSize - 1;
		for (int i = 0; i < poolSize/2; i++) {
			best = i;
			worst = i;
			for (int j = i; j < end-i; j++) {
				rank = ranks[j];
				agent a = genepool.at(rank);
				if (a < genepool.at(ranks[best])) { best = j; }
				if (a > genepool.at(ranks[worst])) { worst = j; }
			}
			tmp = ranks[i];
			ranks[i] = ranks[best];
			ranks[best] = tmp;
			if (worst == i) { worst = best; }
			tmp = ranks[end-i];
			ranks[end-i] = ranks[worst];
			ranks[worst] = tmp;
		}
	}
};