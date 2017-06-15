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
vec3 findEndPoint(vec3 point, float offset);
vec3 findMidPoints(vec3 start, vec3 end, float offset);
vec3 getBezier(curvePoints points, double a, double b);

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
	iterations = 5;			//number of curve iterations
	float offset = 1.0;		//offset for midpoints - Y distance from centre
	int nPoints = 50;		//number of points
	int div;
	vec3 bezierResults;

	//resize start, mid, and end points
	startPoint.resize(iterations);
	midPoint.resize(iterations);
	endPoint.resize(iterations);

	//Initial control points
	startPoint[0] = vec3(-4.0, 0.0, 0.0);								//A
	endPoint[0] = vec3(4.0, 0.0, 0.0);									//C
	midPoint[0] = findMidPoints(startPoint[0], endPoint[0], 2.0);		//B

	//resize points - this is a matrix of points for each iterations
	points.resize(iterations);
	for (int p = 0; p < (int)points.size(); p++) {
		points[p].resize(nPoints);
	}

	curvePts.resize(iterations);

	//Points on curve
	double xx, yy, zz;
	curvePoints curve_temp;
	div = 3;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_LINE_STRIP);

	for (int j = 0; j < iterations; j++) {
		//variable
		double a, b, c, d;
		a = 1.0;
		b = 1.0 - a;
		//div = j + 3;

		curvePts[j].resize(div);
		
		//find mid points
		if (j > 0) {
			if (j % 2 == 0) {
				//upwards curve
				//set the next points
				startPoint[j] = endPoint[j - 1];
				endPoint[j] = findEndPoint(startPoint[j - 1], -2.0);
				midPoint[j] = findMidPoints(startPoint[j], endPoint[j], offset * (j+2));
			}
			else {
				//downwards curve
				//set the next points
				startPoint[j] = endPoint[j - 1];
				endPoint[j] = findEndPoint(startPoint[j - 1], 2.0);
				midPoint[j] = findMidPoints(startPoint[j], endPoint[j], -offset * (j+2));
			}
		}

		//find the points in the curve and draw
		for (int i = 0; i < nPoints; i++) {
			//cout << i << endl;
			//get the points of bezier curve
			curve_temp.start = startPoint[j];
			curve_temp.mid = midPoint[j];
			curve_temp.end = endPoint[j];

			bezierResults = getBezier(curve_temp, a, b);
			points[j][i] += bezierResults;

			//cout << points[j][i].x << " " << points[j][i].y << " " << points[j][i].z << endl;

			//draw
			//glVertex3d(bezierResults.x, bezierResults.y, bezierResults.z);

			//change variable
			a -= 1.0 / nPoints;
			b = 1.0 - a;
		}

		if (j == iterations - 1) {
			div = div / 2 - 1;
			points[j].erase(points[j].begin() + (nPoints / 2 + 3), points[j].begin() + nPoints);
		}

		a = 1.0;
		b = 1.0 - a;

		if (j == 0) {
			for (int i = 0; i < nPoints; i++) {
				//cout << i << endl;
				//get the points of bezier curve
				curve_temp.start = vec3(-0.1, -0.5, 0.0);
				curve_temp.end = startPoint[j];
				curve_temp.mid = findMidPoints(curve_temp.start, curve_temp.end, -0.5);

				bezierResults = getBezier(curve_temp, a, b);
				//points[j][i] += bezierResults;

				//draw
				glVertex3d(bezierResults.x, bezierResults.y, bezierResults.z);

				//change variable
				a -= 1.0 / nPoints;
				b = 1.0 - a;
			}
		}

		int k_ = 0;
		float offset2;

		for (int k = 0; k < div; k++) {
			c = 1.0;
			d = 1.0 - c;

			//find points from the big curve to become new start & end points of new curves
			int nextPt = (int)(points[j].size() * (k + 1) / div) - 1;
			//cout << j << " " << k_ << endl;
			//cout << nextPt << endl;
			curvePts[j][k].start = points[j][k_];
			curvePts[j][k].end = points[j][nextPt];

			k_ = nextPt;
			
			//find mid points of each curve
			if (k % 2 == 0) {
				offset2 = -0.4;
			}
			else {
				offset2 = 0.4;
			}
			curvePts[j][k].mid = findMidPoints(curvePts[j][k].start, curvePts[j][k].end, offset2);

			//find points of each new curve
			//loop through number of points, count x y z
			for (int m = 0; m < nPoints; m++) {
				curve_temp.start = curvePts[j][k].start;
				curve_temp.mid = curvePts[j][k].mid;
				curve_temp.end = curvePts[j][k].end;

				bezierResults = getBezier(curve_temp, c, d);

				//draw
				glVertex3d(bezierResults.x, bezierResults.y, bezierResults.z);

				//change variables
				c -= 1.0 / nPoints;
				d = 1.0 - c;
			}
		}

		div += 2;
	}

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

vec3 getBezier(curvePoints points, double a, double b) {
	vec3 start = points.start;
	vec3 mid = points.mid;
	vec3 end = points.end;

	float xx = start.x*a*a + mid.x * 2 * a*b + end.x*b*b;
	float yy = start.y*a*a + mid.y * 2 * a*b + end.y*b*b;
	float zz = start.z*a*a + mid.z * 2 * a*b + end.z*b*b;

	return vec3(xx, yy, zz);
}