#include <iostream>
#include <chrono>
#include "render.h"
#include "pool.h"

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    int w = 2000; int h = 1200;
    char name[] = "grrrr";
    renderer r(w, h, name);
    //environment env(w, h, w/2, h/2, 35, h-35, 5000, 2000);
    environment env(w, h, 30000, 1500, 1);

	if (env.drawMode) {
		bool goalPlaced = false;
		while (env.drawMode) {
			r.clear();

			if (r.leftPressed) {
				vec2d mpos = r.getMousePos();
				env.addWall(mpos.x, mpos.y);
			}
			if (!goalPlaced && r.rightPressed) {
				goalPlaced = true;
				vec2d mpos = r.getMousePos();
				env.gpos.x = mpos.x;
				env.gpos.y = mpos.y;
				while (r.rightPressed) {
					r.clear();
					r.drawWalls(env);
					env.gpos.x = mpos.x;
					env.gpos.y = mpos.y;
					r.drawGoal(env);
					r.swapBuffers();
					r.pollEvents();
				}
			}
			if (goalPlaced && r.rightPressed) {
				vec2d mpos = r.getMousePos();
				env.startpos.x = mpos.x;
				env.startpos.y = mpos.y;
				env.p.startpos = env.startpos;
				env.p.populate();
				env.drawMode = false;
			}

			r.drawWalls(env);
			if (goalPlaced) { r.drawGoal(env); }

			r.swapBuffers();
			r.pollEvents();
		}
	}

    while (!r.shouldClose()){
        while (!env.allDead){
			start = std::chrono::high_resolution_clock::now();
			r.clear();
			
			r.drawEnv(env);
			env.step();

			r.swapBuffers();
			r.pollEvents();
			if (r.shouldClose()) { break; }
			float spf = (std::chrono::high_resolution_clock::now() - start).count() / 1000000000.0;
			std::cout << "fps: " << 1.0 / spf << std::endl;
        }
        env.reset(500, .3, .01);
    }
    r.kill();
    return 0;
}
