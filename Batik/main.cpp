// http://www.glprogramming.com/red/chapter12.html

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
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

GLfloat ctrlPoints[3][3];

void init(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glShadeModel(GL_FLAT);
	//glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 3, &ctrlPoints[0][0]);
	//glEnable(GL_MAP1_VERTEX_3);
}

void display(void) {
	//Control points
	double	Ax = -4.0, Ay = 0.0, Az = 0.0,
			Bx = 0.0, By = 3.0, Bz = 0.0,
			Cx = 4.0, Cy = 0.0, Cz = 0.0;

	//Points on curve
	double x, y, z;

	//variable
	double a = 1.0;
	double b = 1.0 - a;

	//number of points
	int nPoints = 50;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_LINE_STRIP);

	//find the points in the curve and draw
	for (int i = 0; i <= nPoints; i++) {
		//get the points
		x = Ax*a*a + Bx * 2 * a*b + Cx*b*b;
		y = Ay*a*a + By * 2 * a*b + Cy*b*b;
		z = Az*a*a + Bz * 2 * a*b + Cz*b*b;

		//cout << x << ", " << y << ", " << z << endl;

		//draw
		glVertex3d(x, y, z);

		//change variable
		a -= 1.0 / nPoints;
		b = 1.0 - a;
	}

	glEnd();
	glFlush();

	/* WITH EVALUATORS */
	//GLfloat startPoint[3] = { -4.0, -3.0, 0.0 };
	//GLfloat midPoint[3] = { 0.0, 0.0, 0.0 };
	//GLfloat endPoint[3] = { 4.0, -3.0, 0.0 };

	////bezier curve using evaluators
	//glClear(GL_COLOR_BUFFER_BIT);
	//glColor3f(1.0, 1.0, 1.0);
	//glBegin(GL_LINE_STRIP);
	//for (int i = 0; i <= 50; i++) {
	//	glEvalCoord1f((GLfloat)i / 50.0);
	//}
	//glEnd();

	////displays control points as dots
	//glPointSize(5.0);
	//glColor3f(1.0, 1.0, 0.0);
	//glBegin(GL_POINTS);
	//for (int i = 0; i < 3; i++) {
	//	glVertex3fv(&ctrlPoints[i][0]);
	//}
	//glEnd();

	//glFlush();

	/* ORIGINAL */
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