#pragma once
#include <iostream>
#include "agent.h"

class pool {
public:
	int poolsize;
	int lifespan = -1;
	float startx, starty;
	std::vector<agent> genepool;
	pool() { startx = 0; starty = 0; poolsize = 0; }
	pool(float sx, float sy, int psize) {
		poolsize = psize;
		startx = sx;
		starty = sy;
		}
	void populate() {
		for (int i = 0; i < poolsize; i++) {
			genepool.push_back(agent(startx, starty, lifespan));
		}
	}
};