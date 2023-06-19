#pragma once
#include <iostream>
#include "agent.h"
#include "pool.h"


class environment {
public:
	int width, height, wallsize, poolSize, goalsize;
	bool* wallmask;
	bool allDead = false;
	bool drawMode = false;
	int numDead = 0;
	pool p;
	std::vector<vec2d> walls;
	vec2d gpos;
	vec2d startpos;
	environment(int w, int h, int startx, int starty, float gx, float gy, int psize, int lifespan, int wsize = 34, int gsize = 25) {
		width = w;
		height = h;
		poolSize = psize;
		gpos.x = gx;
		gpos.y = gy;
		goalsize = gsize;
		startpos.x = startx;
		startpos.y = starty;
		p.startpos = startpos;
		p.poolSize = psize;
		p.lifespan = lifespan;
		p.populate();
		wallsize = wsize;
		wallmask = new bool[w * h];
		for (int i = 0; i < w * h; i++) {
			wallmask[i] = false;
		}
	}
	environment(int w, int h, int psize, int lifespan, int wsize = 34, int gsize = 25) {
		drawMode = true;
		width = w;
		height = h;
		poolSize = psize;
		goalsize = gsize;
		p.startpos = startpos;
		p.poolSize = psize;
		p.lifespan = lifespan;
		p.populate();
		wallsize = wsize;
		wallmask = new bool[w * h];
		for (int i = 0; i < w * h; i++) {
			wallmask[i] = false;
		}
	}
	
	void kill(agent& a) {
		if (!a.dead) {
			a.dead = true;
			float dx = a.pos.x - gpos.x;
			float dy = a.pos.y - gpos.y;
			a.fitness = sqrt(dx * dx + dy * dy);
			numDead++;
			allDead = (numDead == poolSize);
		}
	}
	
	void step() {
		for (int i = 0; i < p.genepool.size(); i++) {
			agent& a = agentAt(i);
			a.step();
			if (!a.dead) {
				if (a.age == a.lifespan-1) { kill(a); }
				int rx = round(a.pos.x);
				int ry = round(a.pos.y);
				if ((rx < 0) || (rx >= width) || (ry < 0) || (ry >= height)) { kill(a); }
				else {
					if (wallmask[ry*width + rx]) { kill(a); }
				}
				float dx = a.pos.x - gpos.x;
				float dy = a.pos.y - gpos.y;
				if ((dx * dx + dy * dy) < goalsize * goalsize) { kill(a); a.fitness = a.age; }
			}
		}
	}
	
	void resetAgents() {
		for (int i = 0; i < poolSize; i++) {
			agent& a = agentAt(i);
			a.age = 0;
			a.pos = vec2d(startpos.x, startpos.y);
			a.vel = vec2d(0, 0);
			a.fitness = 1000000;
			a.dead = false;
		}
	}
	
	void reset(int parentCutoff, float inheritRate, float mutateRate){
		numDead = 0;
		allDead = false;
		int* ranks = p.sorted();
		updateAgents(ranks, parentCutoff, inheritRate, mutateRate);
		resetAgents();
	}
	
	void updateAgents(int* ranks, int parentCutoff, float inheritRate, float mutateRate) {
		for (int i = parentCutoff; i < poolSize; i++) {
			agent& a = agentAt(i);
			int pindex = udist(rd) * parentCutoff;
			agent& parent = agentAt(ranks[pindex]);
			a.inherit(parent, inheritRate);
			a.mutate(mutateRate);
		}
	}

	agent& agentAt(int i) {
		return p.genepool.at(i);
	}

	void addWall(float xx, float yy) {
		for (int i = 0; i < walls.size(); i++) {
			if (walls.at(i).x == xx and walls.at(i).y == yy) { return; }
		}
		walls.push_back(vec2d(xx, yy));
		for (float r=0; r < wallsize; r += 1) {
			float pi = 3.14159;
			if (r != 0) { float inc = 1 / r; }
			else { float inc = 0.5; }
			for (float ang = 0; ang < 2 * pi; ang += 1 / r) {
				float dx = r * cos(ang);
				float dy = r * sin(ang);
				int px = round(xx + dx);
				int py = round(yy + dy);
				wallmask[py*width + px] = true;
			}
		}
	}
};

