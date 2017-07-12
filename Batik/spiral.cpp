// http://www.glprogramming.com/red/chapter12.html

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <GL\glut.h>
#include <glm\glm.hpp>
#include <glm\fwd.hpp>

using namespace std;
using namespace glm;

//GLfloat ctrlPoints[4][3] = {
//	{-4.0, -3.0, 0.0}, {0.0, 0.0, 0.0},
//	{4.0, -3.0, 0.0}};

struct curvePoints {
	vec3 start;
	vec3 mid;
	vec3 end;
};

//Global variables
vector<vector<curvePoints>> curvePts;
vector<vec3> startPoint, midPoint, endPoint;
vector<vector<vec3>> points;
int iterations;

//Functions
void init(void);
void display(void);
void reshape(int w, int h);
void drawLittleSpiral(double a, double b);

//Main functions
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);                    // window size
	glutInitWindowPosition(100, 100);                // distance from the top-left screen
	glutCreateWindow("Test");    // message displayed on top bar window
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}

void init(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glShadeModel(GL_FLAT);
	//glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 3, &ctrlPoints[0][0]);
	//glEnable(GL_MAP1_VERTEX_3);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	drawLittleSpiral(0.3, 0.3);
	glBegin(GL_LINE_STRIP);
	glVertex2f(0.3, 0.3);
	glVertex2f(0.0, 0.0);
	glEnd();
	glFlush();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float edge = 10.0;

	if (w <= h) {
		glOrtho(-edge, edge, -edge*(GLfloat)h / (GLfloat)w,
			edge*(GLfloat)h / (GLfloat)w, -edge, edge);
	}
	else {
		glOrtho(-edge*(GLfloat)h / (GLfloat)h,
			edge*(GLfloat)w / (GLfloat)h, -edge, edge, -edge, edge);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void drawLittleSpiral(double a, double b) {
	//http://techtutorials95.blogspot.co.uk/2015/05/opengl-making-spirals.html

	double x, y, xx, yy;
	double theta = 0.0;
	double rt, rt2;
	
	rt = a * exp(b * theta);

	x = 1/rt * sin(theta);
	y = 1/rt * cos(theta);

	for (int i = 0; i < 500; i++) {
		glBegin(GL_LINES);
		theta = 0.025 * i;
		rt2 = a * exp(b * theta);

		xx = 1/rt2 * sin(theta);
		yy = 1/rt2 * cos(theta);

		glVertex2f((GLfloat)x, (GLfloat)y);
		glVertex2f(xx, yy);

		x = 1/rt2 * sin(theta);
		y = 1/rt2 * cos(theta);
		glEnd();
	}
}