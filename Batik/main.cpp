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

//Global variables
vector<vec3> startPoint, midPoint, endPoint;
int iterations;

//Functions
void init(void);
void display(void);
void reshape(int w, int h);
vec3 findEndPoint(vec3 point, float offset);
vec3 findMidPoints(vec3 start, vec3 end, float offset);

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
	//number of curve iterations
	iterations = 2;
	float offset = 3.0;

	startPoint.resize(iterations);
	midPoint.resize(iterations);
	endPoint.resize(iterations);

	//number of points
	int nPoints = 50;

	//Initial control points
	startPoint[0] = vec3(-4.0, 0.0, 0.0);								//A
	endPoint[0] = vec3(4.0, 0.0, 0.0);									//C
	midPoint[0] = findMidPoints(startPoint[0], endPoint[0], offset);		//B

	//Points on curve
	double x, y, z;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_LINE_STRIP);

	for (int j = 0; j < iterations; j++) {
		//variable
		double a = 1.0;
		double b = 1.0 - a;

		if (j > 0) {
			//set the next points
			startPoint[j] = endPoint[j - 1];
			endPoint[j] = findEndPoint(startPoint[j - 1], 2.0);
			midPoint[j] = findMidPoints(startPoint[j], endPoint[j], -offset);
		}

		//find the points in the curve and draw
		for (int i = 0; i <= nPoints; i++) {
			//get the points of bezier curve
			x = startPoint[j].x*a*a + midPoint[j].x * 2 * a*b + endPoint[j].x*b*b;
			y = startPoint[j].y*a*a + midPoint[j].y * 2 * a*b + endPoint[j].y*b*b;
			z = startPoint[j].z*a*a + midPoint[j].z * 2 * a*b + endPoint[j].z*b*b;

			//cout << x << ", " << y << ", " << z << endl;

			//draw
			glVertex3d(x, y, z);

			//change variable
			a -= 1.0 / nPoints;
			b = 1.0 - a;
		}
	}

	glEnd();
	glFlush();

	/* TEST */
	//glClear(GL_COLOR_BUFFER_BIT);
	//glBegin(GL_POLYGON);
	//glVertex2f(0.0, 0.0);                    // bottom left
	//glVertex2f(0.5, 0.0);                    // bottom right
	//glVertex2f(0.5, 0.5);                    // top right
	//glVertex2f(0.0, 0.5);                    // top left
	//glEnd();
	//glFlush();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float edge = 8.0;

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

vec3 findEndPoint(vec3 point, float offset) {
	float x = point.x - offset;
	float y = point.y;
	float z = point.z;

	vec3 end(x, y, z);
	return end;
}

vec3 findMidPoints(vec3 start, vec3 end, float offset) {
	float x = (start.x + end.x) / 2;
	float y = ((start.y + end.y) / 2) + offset;
	float z = (start.z + end.z) / 2;

	vec3 mid(x, y, z);
	return mid;
}