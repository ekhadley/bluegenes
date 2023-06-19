#pragma once
#include <iostream>

class vec2d {
private:
	float mag = 0;
	float angle = 0;
	bool truemag = false;
	bool trueangle = false;
public:
	float x, y;
	vec2d() { x = 0; y = 0; }
	vec2d(float xx, float yy) {
		x = xx; y = yy;
	}
	void add(float valx, float valy) {
		x += valx;
		y += valy;
		truemag = false;
		trueangle = false;
	}
	void scale(float s) {
		truemag = false;
		x *= s;
		y *= s;
	}
	float getMag() {
		if (!truemag){
			truemag = true;
			mag = sqrt(x*x + y*y);
		}
		return mag;
	}
	float getAngle() {
		if (!trueangle) {
			trueangle = true;
			angle = atan2(y, x);
		}
		return angle;
	}
	
};