#include <iostream>
#include <chrono>
#include "render.h"
#include "pool.h"

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    double mouseX, mouseY;
    int w = 2000; int h = 1200;
    char name[] = "grrrr";
    renderer r(w, h, name);
    environment env(w, h, 35, h-35, 5000, 3000);
    env.addWall(1000, 800);

    int i = 0;
    while (1){
        start = std::chrono::high_resolution_clock::now();
        r.clear();
        
        r.drawEnv(env);
        env.step();

        r.swapBuffers();
        r.pollEvents(mouseX, mouseY);
        if (r.shouldClose()) { break; }

        float spf = (std::chrono::high_resolution_clock::now() - start).count() / 1000000000.0;
        if (i % 100 == 0) { std::cout << "fps: " << 1.0 / spf << std::endl; i = 0; }
        i++;
    }
    r.kill();
    return 0;
}
