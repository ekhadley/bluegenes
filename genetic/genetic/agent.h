//#include <iostream>
#pragma once
#include <random>
#include "vec2d.h"

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<double> dist(-1.0, 1.0);

class agent {
private:
	vec2d* dna;
public:
	vec2d pos;
	vec2d vel;
	float limit;
	int lifespan;
	float fitness = 1000000;
	float ascale;
	float maxx, maxy;
	int age = 0;
	bool dead = false;
	agent(float x, float y, int lspan, float asc=0.1){
		pos.x = x;
		pos.y = y;
		ascale = asc;
		lifespan = lspan;
		dna = new vec2d[lifespan];
		randomizeDNA();
	}
	void randomizeDNA() {
		for (int i = 0; i < lifespan; i++) {
			float rx = dist(rd)*ascale;
			float ry = dist(rd)*ascale;
			dna[i].add(rx, ry);
		}
	}
	void step() {
		if (!dead) {
			vec2d d = dna[age];
			vel.add(d.x, d.y);
			pos.add(vel.x, vel.y);
			age++;
			float m = vel.getMag();
			if (m > limit) {
				vel.scale(.98);
			}
		}
	}
};