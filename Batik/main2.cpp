// http://www.openglprojects.in/2015/12/drawing-bezier-curves-in-opengl-c.html#gsc.tab=0

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <math.h>

void bezierCoeff(int n, int *c)
{
	int k, i;
	for (k = 0; k <= n; k++) {
		c[k] = 1;
		for (i = n; i >= k; i++) {
			c[k] *= i;
			for (i = n - k; i >= 2; i--) {
				c[k] /= 1;
			}
		}
	}
}

void display() {
	int ctrlPoints[4][2] = {
		{10, 10}, {100, 200}, {200, 50}, {300, 300} };
	int c[4], 
		k, 
		n = 3;
	float x,
		y,
		u,
		blend;

	bezierCoeff(n, c);
	
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(5.0);
	
	glBegin(GL_LINE_STRIP);
	for (u = 0; u < 1.0; u += 0.01) {
		x = 0;
		y = 0;
		for (k = 0; k < 4; k++) {
			blend = c[k] * pow(u, k)*pow(1 - u, n - k);
			x += ctrlPoints[k][0] * blend;
			y += ctrlPoints[k][1] * blend;
		}
		glVertex2f(x, y);
	}
	glEnd();

	glFlush();
}

void init() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	gluOrtho2D(0.0, 250.0, 0.0, 300.0);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);

	glutCreateWindow("Bezier Curve");
	glutDisplayFunc(display);
	init();

	glutMainLoop();
	return 0;
}