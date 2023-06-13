#pragma once
#include <iostream>
#include "agent.h"
#include "pool.h"

class environment {
public:
	int width, height, wallsize, poolsize, goalsize;
	bool* wallmask;
	pool p;
	std::vector<vec2d> walls;
	vec2d gpos;
	environment(int w, int h, float gx, float gy, int psize, int lifespan, int wsize = 34, int gsize = 25) {
		width = w;
		height = h;
		poolsize = psize;
		gpos.x = gx;
		gpos.y = gy;
		goalsize = gsize;
		p.startx = w/2;
		p.starty = h/2;
		p.poolsize = psize;
		p.lifespan = lifespan;
		p.populate();
		wallsize = wsize;
		wallmask = new bool[w * h];
		for (int i = 0; i < w * h; i++) {
			wallmask[i] = false;
		}
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
	void kill(agent& a) {
		a.dead = true;
		float dx = a.pos.x - gpos.x;
		float dy = a.pos.y - gpos.y;
		a.fitness = sqrt(dx*dx + dy*dy);
	}
	void step() {
		for (int i = 0; i < p.genepool.size(); i++) {
			agent& a = p.genepool.at(i);
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
				if ((dx*dx + dy*dy) < goalsize*goalsize) { kill(a); }
			}
		}
	}
};

