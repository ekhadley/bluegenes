#include <iostream>
#include <GLFW/glfw3.h>
#include "agent.h"
#include "pool.h"
#include "environment.h"

class renderer {
public:
    int width, height;
    GLFWwindow* window;

    renderer(int w, int h, char* name) : width(w), height(h) {
        glfwInit();
        window = glfwCreateWindow(w, h, name, NULL, NULL);
        glfwMakeContextCurrent(window);
        glfwSetMouseButtonCallback(window, onClick);
        setClearColor(.1, .1, .1, 1);
        setLineWidth(1.5);
        setDrawColor(0.0f, .8f, 1.0f);
    }
    void drawAgent(agent a, float scale=1) {
        float nose = 15*scale;
        float tail = 0*scale;
        float wing = 12*scale;
        float x, y, nosex, nosey, tailx, taily, wing1x, wing1y, wing2x, wing2y;
        float ang = a.vel.getAngle();
        float pi = 3.14159;
        float wingang = pi*135.0/180.0;

        x = a.pos.x + nose*cos(ang);
        y = a.pos.y + nose*sin(ang);
		coordToScale(x, y, nosex, nosey);
        x = a.pos.x + wing*cos(ang-wingang);
        y = a.pos.y + wing*sin(ang-wingang);
		coordToScale(x, y, wing1x, wing1y);
        x = a.pos.x + tail*cos(-ang);
        y = a.pos.y + tail*sin(-ang);
		coordToScale(x, y, tailx, taily);
        x = a.pos.x + wing*cos(ang+wingang);
        y = a.pos.y + wing*sin(ang+wingang);
		coordToScale(x, y, wing2x, wing2y);
		
        //glfwMakeContextCurrent(window);

		if(a.dead){ setDrawColor(1, .1, .35); }
		else{ setDrawColor(0, 1, .5); }
		glBegin(GL_POLYGON);
		glVertex2f(nosex, nosey);
		glVertex2f(wing1x, wing1y);
		glVertex2f(tailx, taily);
		glVertex2f(wing2x, wing2y);
		glEnd();
		
        setDrawColor(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(nosex, nosey);
		glVertex2f(wing1x, wing1y);
		glVertex2f(tailx, taily);
		glVertex2f(wing2x, wing2y);
		glEnd();
        }

    void drawEnv(environment env){
        drawGoal(env);
        drawWalls(env);
        drawAgents(env);
    }

    void drawGoal(environment env) {
        setDrawColor(.9, 0, .5);
        setLineWidth(3);
		drawCircle(env.gpos.x, env.gpos.y, env.goalsize);
    }

    void drawAgents(environment env) {
        setDrawColor(0, 1, .5);
        setLineWidth(1.5);
        for (int i = 0; i < env.p.poolSize; i++) {
            agent a = env.agentAt(i);
            drawAgent(a);
        }
    }

    void drawWalls(environment env) {
        setDrawColor(.5, 0, .9);
        for (int i = 0; i < env.walls.size(); i++) {
            vec2d w = env.walls.at(i);
            drawCircle(w.x, w.y, env.wallsize, true);
        }
    }

    void onClick(GLFWwindow* win, int button, int action, int mods) {
        
    }

    void coordToScale(float coordX, float coordY, float& scaleX, float& scaleY) {
        scaleX = 2.0*coordX/width - 1;
        scaleY = 2.0*coordY/height - 1;
    }
    bool shouldClose() { return glfwWindowShouldClose(window); }
    void setLineWidth(int w) {
        //glfwMakeContextCurrent(window);
        glLineWidth(w);
    }
    void setDrawColor(float r, float g, float b) {
        //glfwMakeContextCurrent(window);
        glColor3f(r, g, b);
    }
    void setClearColor(float r, float g, float b, float a) {
        //glfwMakeContextCurrent(window);
        glClearColor(r, g, b, a);
    }
    void swapBuffers() { glfwSwapBuffers(window); }
    void pollEvents(double& mouseX, double& mouseY) {
        glfwPollEvents();
        glfwGetCursorPos(window, &mouseX, &mouseY);
    }
    void clear() { glClear(GL_COLOR_BUFFER_BIT); }
    void kill() { glfwTerminate(); }
    void drawCircle(float posx, float posy, float radius, bool fill = false, int numpts=32) {
        if (fill) { glBegin(GL_POLYGON); }
        else { glBegin(GL_LINE_LOOP); }
        float ang, x, y, sx, sy;
        for (int i = 0; i < numpts; i++) {
            ang =  2*3.14159*i/numpts;
            x = posx + radius * cos(ang);
            y = posy + radius * sin(ang);
            coordToScale(x, y, sx, sy);
            glVertex2f(sx, sy);
        }
        glEnd();
    }
};