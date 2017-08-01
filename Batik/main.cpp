// http://www.glprogramming.com/red/chapter12.html

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

#define PI 3.14

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
vector<vector<int>> thueMorse;
int iterations, rows, cols, tmRandomX, tmRandomY, pattern;
double edge;

//Functions
void init(void);
void display(void);
void reshape(int w, int h);
vec3 findEndPoint(vec3 point, float offset);
vec3 findMidPoints(vec3 start, vec3 end, float offset);
vec3 findMidXPoints(vec3 start, vec3 end, float offset);
vec3 getBezier(curvePoints points, double a, double b);
void drawLittleSpiral(double a, double b);
void thueMorseAlgo(int r, int c); 
void megamendung(int type);
void kawung();
void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius);	/* https://gist.github.com/strife25/803118 */

//Main functions
int main(int argc, char** argv) {
	pattern = 1;
	
	rows = 5;
	cols = 4;
	edge = 40.0;

	thueMorseAlgo(edge + 10.0, edge + 10.0);

	srand(time(NULL));
	tmRandomX = rand() % (int)edge + 1;
	tmRandomY = rand() % (int)edge + 1;
	cout << tmRandomX << " " << tmRandomY << endl;

	/*for (int i = -edge; i <= edge; i += nextCol) {
		rowCount = 0;
		for (int j = -edge; j <= edge; j += nextRow) {
			cout << i << " " << j << endl;
			rowCount += 1;
		}
		colCount += 1;
	}
	cout << endl;*/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);                    // window size
	glutInitWindowPosition(100, 100);                // distance from the top-left screen
	glutCreateWindow("Test");    // message displayed on top bar window
	//init();
	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	glutMainLoop();
	
	return 0;
}

void init(void) {
	glClearColor(0.0, 0.27, 0.51, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glShadeModel(GL_FLAT);
	//glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 3, &ctrlPoints[0][0]);
	//glEnable(GL_MAP1_VERTEX_3);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//edge = 20.0;
	glOrtho(-edge, edge, -edge, edge, -edge, edge);
	//if (w <= h) {
	//	glOrtho(-edge, edge, -edge*(GLfloat)h / (GLfloat)w,
	//		edge*(GLfloat)h / (GLfloat)w, -edge, edge);
	//}
	//else {
	//	glOrtho(-edge*(GLfloat)h / (GLfloat)h,
	//		edge*(GLfloat)w / (GLfloat)h, -edge, edge, -edge, edge);
	//}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display(void) {
	init();
	
	if (pattern == 1) {
		/* megamendung test 2 */
		int nextRow = (edge * 2) / (rows - 1);
		int nextCol = (edge * 2) / (cols - 1);
		int rowCount, colCount, x3, y3, ty;
		vec3 color(1.0, 1.0, 1.0);
		float scaleX, scaleY;
		//cout << nextRow << endl << nextCol << endl;

		iterations = 4;			//randomize this
		colCount = 0;
		for (int i = 0; i < cols; i++) {
			rowCount = 0;
			x3 = -edge + (i * nextCol);			//not random

			scaleX = 2.0;	//random
			scaleY = 2.0;	//random
			for (int j = 0; j < rows; j++) {
				//random position
				/*int ii = rand() % ((int)edge * 2) + 1;
				int jj = rand() % ((int)edge * 2) + 1;
				x3 = -edge + ii;
				y3 = -edge + jj;
				cout << ii << " " << jj << endl;*/

				y3 = -edge + (j * nextRow);		//not random

				if (thueMorse[i + tmRandomX][j + tmRandomY] == 0) {
					ty = 2;
				}
				else if (thueMorse[i + tmRandomX][j + tmRandomY] == 1) {
					ty = 1;
				}
				cout << thueMorse[i + tmRandomX][j + tmRandomY] << " ";

				//cout << x3 << " " << y3 << endl;
				glPushMatrix();
				//for vertical patterns
				glRotated(0.0, 0.0, 0.0, 1.0);
				//move to new positions
				glTranslated((GLdouble)x3, (GLdouble)y3, 0.0);
				//scale to become smaller
				glScaled((GLdouble)scaleX, (GLdouble)scaleY, 1.0);
				//color the lines
				glColor3d((GLdouble)color.x, (GLdouble)color.y, (GLdouble)color.z);
				megamendung(ty);
				glPopMatrix();
				rowCount++;
			}
			cout << endl;
			colCount++;
		}

		/* megamendung test 1 */
		//to random: iterations, position, scale, rotation, color

		//int x2, y2;
		//int cloudN = 0;
		//int k = edge / 4;
		//int col = 0;
		//int row, ty;
		//vec3 color;				//randomize this

		////regular tiles
		//for (int i = -edge; i <= edge; i++) {
		//	row = 0;
		//	float scaleX = k/3;
		//	float scaleY = k/3;
		//	x2 = i + k;
		//	for (int j = -edge; j <= edge; j++) {
		//		y2 = j + k;
		//		cloudN++;
		//		if (cloudN % 2 == 0) {
		//			scaleX *= -1.0;
		//			//scaleY = -1.0;
		//		}
		//		else {
		//			scaleX *= 1.0;
		//			//scaleY = 1.0;
		//		}

		//		if (col % 2 > 0) {
		//			y2 -= k;	//different y position for each row
		//			color = vec3(0.0, 1.0, 0.0);
		//			//ty = 1;
		//		}
		//		else {
		//			color = vec3(1.0, 1.0, 0.0);
		//			//ty = 2;
		//		}

		//		if (row % 2 == 0) ty = 1;
		//		else if (row % 2 > 0) ty = 2;

		//		glPushMatrix();
		//		//for vertical patterns
		//		glRotated(0.0, 0.0, 0.0, 1.0);
		//		//move to new positions
		//		glTranslated((GLdouble)x2, (GLdouble)y2, 0.0);
		//		//scale to become smaller
		//		glScaled((GLdouble)scaleX, (GLdouble)scaleY, 1.0);
		//		//color the lines
		//		glColor3d((GLdouble)color.x, (GLdouble)color.y, (GLdouble)color.z);
		//		drawCloud(ty);
		//		glPopMatrix();

		//		j += k * 2;
		//		scaleY *= -1.0;
		//		row++;
		//	}
		//	i += k * 3;
		//	col++;
		//}
	}
	else if (pattern == 2) {
		/* kawung test */
		double resize = 10.0;
		for (double i = -edge; i <= edge; i += resize * 2) {
			for (double j = -edge; j <= edge; j += resize * 2) {
				glPushMatrix();
				glTranslated((GLdouble)i, (GLdouble)j, 1.0);
				glScaled(resize, resize, 1.0);
				kawung();
				glPopMatrix();
			}
		}
	}

	/* megamendung test 0 */
	/*glPushMatrix();
	glTranslated(-20.0, 20.0, 0.0);
	glScaled(-1.0, -1.0, 1.0);
	drawCloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 20.0, 0.0);
	glScaled(1.5, -1.5, 1.0);
	drawCloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(20.0, 20.0, 0.0);
	glScaled(-1.0, -1.0, 1.0);
	drawCloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-10.0, 10.0, 0.0);
	glScaled(-1.5, 1.5, 1.0);
	drawCloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(10.0, 10.0, 1.0);
	glScaled(1.5, 1.5, 1.0);
	drawCloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-20.0, 0.0, 0.0);
	glScaled(-1.0, 1.0, 1.0);
	drawCloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.0, 0.0);
	glScaled(1.5, 1.5, 1.0);
	drawCloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(20.0, 0.0, 0.0);
	glScaled(-1.0, 1.0, 1.0);
	drawCloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-10.0, -10.0, 1.0);
	glScaled(-1.5, -1.5, 1.0);
	drawCloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(10.0, -10.0, 1.0);
	glScaled(1.5, -1.5, 1.0);
	drawCloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-20.0, -20.0, 1.0);
	glScaled(-1.0, -1.0, 1.0);
	drawCloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, -20.0, 1.0);
	glScaled(1.5, -1.5, 1.0);
	drawCloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(20.0, -20.0, 1.0);
	glScaled(-1.0, -1.0, 1.0);
	drawCloud();
	glPopMatrix();*/

	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float edge = 20.0;

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

	//scales the image
	//use random numbers to create different types
	//glScalef(0.5, 0.5, 1.0);
}

void megamendung(int type) {
	vector<vec3> mendungPts;

	if (type == 1) {
		//iterations = 4;			//number of curve iterations
		double offset = 1.0;		//offset for midpoints - Y distance from centre
		int nPoints = 50;		//number of points
		int div;
		vec3 bezierResults;
		vec3 finalPoint;

		startPoint.resize(iterations);
		midPoint.resize(iterations);
		endPoint.resize(iterations);
		points.resize(iterations);
		for (int p = 0; p < (int)points.size(); p++) {
			points[p].resize(nPoints);
		}
		curvePts.resize(iterations);

		/*cout << "*" << c_ << endl;
		cout << startPoint.size() << " " << midPoint.size() << " " << endPoint.size() << endl;
		cout << points.size() << " " << curvePts.size() << endl;*/

		//Initial control points
		startPoint[0] = vec3(-4.0, 0.0, 0.0);								//A
		endPoint[0] = vec3(4.0, 0.0, 0.0);									//C
		midPoint[0] = findMidPoints(startPoint[0], endPoint[0], 2.0);		//B

		//Points on curve
		double xx, yy, zz;
		curvePoints curve_temp;
		div = 3;

		//glClear(GL_COLOR_BUFFER_BIT);
		//glColor3d(1.0, 1.0, 1.0);
		glLineWidth(1.5);
		//drawLittleSpiral(0.5, 0.5);
		//glBegin(GL_LINE_STRIP);

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
					midPoint[j] = findMidPoints(startPoint[j], endPoint[j], offset * (j + 2));
				}
				else {
					//downwards curve
					//set the next points
					startPoint[j] = endPoint[j - 1];
					endPoint[j] = findEndPoint(startPoint[j - 1], 2.0);
					midPoint[j] = findMidPoints(startPoint[j], endPoint[j], -offset * (j + 2));
				}
			}

			//get the points of bezier curve
			curve_temp.start = startPoint[j];
			curve_temp.mid = midPoint[j];
			curve_temp.end = endPoint[j];
			//find the points in the curve and draw

			//base curve
			for (int i = 0; i < nPoints; i++) {
				//cout << i << endl;
				bezierResults = getBezier(curve_temp, a, b);
				points[j][i] = bezierResults;

				//change variable
				a -= 1.0 / nPoints;
				b = 1.0 - a;
			}

			if (j == iterations - 1) {
				div = div / 2 - 1;
				points[j].erase(points[j].begin() + (nPoints / 2), points[j].begin() + nPoints);
			}

			a = 1.0;
			b = 1.0 - a;

			//add extra curve for the centre part
			if (j == 0) {
				vec3 spiralEnd1(3.9, -0.5, 0.0);
				vec3 spiralEnd2(3.9, 0.5, 0.0);
				vec3 spiralEnd3(3.8, -0.2, 0.0);

				//spiral
				double e = 1.0;
				double f = 1.0 - e;

				vec3 spirStart, spirEnd, spirMid;
				//1
				spirStart = startPoint[0] + spiralEnd3;			//spiralEnd3
				spirEnd = startPoint[0] + spiralEnd2;				//sspiralEnd2
				spirMid = findMidXPoints(spirStart, spirEnd, -0.8);

				curve_temp.start = spirStart;
				curve_temp.mid = spirMid;
				curve_temp.end = spirEnd;

				//get bezier
				for (int i = 0; i < nPoints; i++) {
					bezierResults = getBezier(curve_temp, e, f);
					mendungPts.push_back(bezierResults);

					//draw
					//glVertex3d(bezierResults.x, bezierResults.y, bezierResults.z);

					//change variables
					e -= 1.0 / nPoints;
					f = 1.0 - e;
				}

				//2
				e = 1.0;
				f = 1.0 - e;
				spirStart = startPoint[0] + spiralEnd2;			//spiralEnd2
				spirEnd = startPoint[0] + spiralEnd1;			//spiralEnd1
				spirMid = findMidXPoints(spirStart, spirEnd, 0.8);

				curve_temp.start = spirStart;
				curve_temp.mid = spirMid;
				curve_temp.end = spirEnd;

				//get bezier
				for (int i = 0; i < 50; i++) {
					bezierResults = getBezier(curve_temp, e, f);
					mendungPts.push_back(bezierResults);

					//draw
					//glVertex3d(bezierResults.x, bezierResults.y, bezierResults.z);

					//change variables
					e -= 1.0 / nPoints;
					f = 1.0 - e;
				}

				//curve
				curve_temp.start = spirEnd;
				curve_temp.end = startPoint[j];
				curve_temp.mid = findMidPoints(curve_temp.start, curve_temp.end, -0.5);

				for (int i = 0; i < nPoints; i++) {
					//get the points of bezier curve
					bezierResults = getBezier(curve_temp, a, b);
					mendungPts.push_back(bezierResults);
					//points[j][i] += bezierResults;

					//draw
					//glVertex3d(bezierResults.x, bezierResults.y, bezierResults.z);

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

				curve_temp.start = curvePts[j][k].start;
				curve_temp.mid = curvePts[j][k].mid;
				curve_temp.end = curvePts[j][k].end;

				//find points of each new curve
				//loop through number of points, count x y z
				for (int m = 0; m < nPoints; m++) {
					bezierResults = getBezier(curve_temp, c, d);
					mendungPts.push_back(bezierResults);

					//draw
					//glVertex3d(bezierResults.x, bezierResults.y, bezierResults.z);

					//change variables
					c -= 1.0 / nPoints;
					d = 1.0 - c;
				}

				//draw inwards spiral from final point
				if (j == iterations - 1 && k == div - 1) {
					double e = 1.0;
					double f = 1.0 - e;
					double endY1, endY2, offset3, offset4;

					if (iterations % 2 == 0) {
						endY1 = 0.8;
						endY2 = -0.6;
						offset3 = -0.6;
						offset4 = 0.8;
					}
					else {
						endY1 = -0.8;
						endY2 = 0.6;
						offset3 = 0.6;
						offset4 = -0.8;
					}

					//spiral
					vec3 spirStart, spirEnd, spirMid;
					spirStart = vec3(points[j][points[j].size() - 1].x, points[j][points[j].size() - 1].y, 0.0);
					spirEnd = vec3(points[j][points[j].size() - 1].x, points[j][points[j].size() - 1].y + endY1, 0.0);
					spirMid = findMidXPoints(spirStart, spirEnd, offset3);

					curve_temp.start = spirStart;
					curve_temp.mid = spirMid;
					curve_temp.end = spirEnd;

					//get bezier
					for (int i = 0; i < 50; i++) {
						/*curve_temp.start = spirStart;
						curve_temp.mid = spirMid;
						curve_temp.end = spirEnd;*/

						bezierResults = getBezier(curve_temp, e, f);
						mendungPts.push_back(bezierResults);

						//draw
						//glVertex3d(bezierResults.x, bezierResults.y, bezierResults.z);

						//change variables
						e -= 1.0 / nPoints;
						f = 1.0 - e;
					}

					//do one more time for smaller circle
					spirStart = spirEnd;
					spirEnd = vec3(spirStart.x + 0.1, spirStart.y + endY2, 0.0);
					spirMid = findMidXPoints(spirStart, spirEnd, offset4);

					e = 1.0;
					f = 1.0 - e;

					curve_temp.start = spirStart;
					curve_temp.mid = spirMid;
					curve_temp.end = spirEnd;

					//get bezier
					for (int i = 0; i < 50; i++) {
						/*curve_temp.start = spirStart;
						curve_temp.mid = spirMid;
						curve_temp.end = spirEnd;*/

						bezierResults = getBezier(curve_temp, e, f);
						mendungPts.push_back(bezierResults);

						//draw
						//glVertex3d(bezierResults.x, bezierResults.y, bezierResults.z);

						//change variables
						e -= 1.0 / nPoints;
						f = 1.0 - e;
					}

					/*glVertex3d(spirStart.x, spirStart.y, 0.0);
					glVertex3d(spirMid.x, spirMid.y, 0.0);
					glVertex3d(spirEnd.x, spirEnd.y, 0.0);*/
					//cout << points[j][k].x << " " << points[j][k].y << endl;
				}
			}

			div += 2;
		}
	}
	else if (type == 2) {
		//**cloud version 2
		int iterations2 = 7;
		int nPoints = 50;		//number of points
		int div;
		double offset, offset2;
		vec3 bezierResults;
		vec3 finalPoint;
		curvePoints curve_temp;

		startPoint.resize(iterations2);
		midPoint.resize(iterations2);
		endPoint.resize(iterations2);
		points.resize(iterations2);
		for (int p = 0; p < (int)points.size(); p++) {
			points[p].resize(nPoints);
		}
		curvePts.resize(iterations2);

		double p0 = 0.0;
		double p1 = 0.8;
		double p2 = 2.0;
		double p3 = 6.0;

		startPoint[0] = vec3(-p1, -0.7, 0.0);
		endPoint[0] = vec3(-p2, -0.5, 0.0);
		midPoint[0] = findMidPoints(startPoint[0], endPoint[0], -2.0);

		endPoint[1] = vec3(-p0, -0.5, 0.0);
		endPoint[2] = vec3(-p3, -0.5, 0.0);
		endPoint[3] = vec3(p3, -0.5, 0.0);
		endPoint[4] = vec3(p0, -0.5, 0.0);
		endPoint[5] = vec3(p2, -0.4, 0.0);
		endPoint[6] = vec3(p1, -0.7, 0.0);

		//glScaled(3.0, 3.0, 1.0);
		//glColor3d(1.0, 1.0, 1.0);
		//glLineWidth(2.0);
		//glBegin(GL_LINE_STRIP);

		for (int i = 0; i < iterations2; i++) {
			if (i < 2 || i > 4) offset = 1.0;
			else if (i == 3) offset = p3;
			else offset = 3.0;

			if (i > 0) {
				startPoint[i] = endPoint[i - 1];
			}

			if (i % 2 == 0) {
				midPoint[i] = findMidPoints(startPoint[i], endPoint[i], -offset);
			}
			else if (i % 2 >= 0) {
				offset += 1.0;
				midPoint[i] = findMidPoints(startPoint[i], endPoint[i], offset);
			}

			double a, b, c, d;
			a = 1.0;
			b = 1.0 - a;

			curve_temp.start = startPoint[i];
			curve_temp.mid = midPoint[i];
			curve_temp.end = endPoint[i];

			for (int j = 0; j < nPoints; j++) {
				bezierResults = getBezier(curve_temp, a, b);
				points[i][j] = bezierResults;

				if (i < 2 || i > 4) {
					mendungPts.push_back(bezierResults);
					//glVertex3d(bezierResults.x, bezierResults.y, bezierResults.z);
				}

				a -= 1.0 / nPoints;
				b = 1.0 - a;
			}

			if (i % 2 == 0) div = 4;
			else if (i % 2 > 0) div = 6;

			int k_ = 0;
			curvePts[i].resize(div);

			if (i > 1 && i < 5) {
				for (int k = 0; k < div; k++) {
					c = 1.0;
					d = 1.0 - c;

					//find points from the big curve to become new start & end points of new curves
					int nextPt = (int)(points[i].size() * (k + 1) / div) - 1;
					//cout << i << " " << k << endl;
					curvePts[i][k].start = points[i][k_];
					curvePts[i][k].end = points[i][nextPt];

					k_ = nextPt;

					//find mid points of each curve
					if (k % 2 == 0) {
						offset2 = -(p3 / 10);
					}
					else {
						offset2 = p3/10;
					}

					if (i == 4) {
						offset2 *= -1.0;
					}
					curvePts[i][k].mid = findMidPoints(curvePts[i][k].start, curvePts[i][k].end, offset2);

					curve_temp.start = curvePts[i][k].start;
					curve_temp.mid = curvePts[i][k].mid;
					curve_temp.end = curvePts[i][k].end;

					//find points of each new curve
					//loop through number of points, count x y z
					for (int m = 0; m < nPoints; m++) {
						bezierResults = getBezier(curve_temp, c, d);
						mendungPts.push_back(bezierResults);

						//draw
						//glVertex3d(bezierResults.x, bezierResults.y, bezierResults.z);

						//change variables
						c -= 1.0 / nPoints;
						d = 1.0 - c;
					}
				}
			}
		}
		//**end of cloud version 2
	}

	//cout << mendungPts.size << endl;
	glColor3d(0.5, 0.0, 0.3);
	//glLineWidth(2.0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < mendungPts.size(); i++) {
		glVertex2d(mendungPts[i].x, mendungPts[i].y);
	}
	glEnd();

	glColor3d(1.0, 1.0, 1.0);
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < mendungPts.size(); i++) {
		glVertex2d(mendungPts[i].x, mendungPts[i].y);
	}
	glEnd();

	//glEnd();
	glFlush();
}

void kawung() {
	double kawungSize = 1.0;
	double xx, yy, a, b;
	curvePoints temp;
	vec3 bezierResults;
	vec3 centre(0.0, 0.0, 0.0);
	vector<vec3> kawungPts;
	int nPoints = 50;
	
	endPoint.resize(4);
	endPoint[0] = vec3(kawungSize, kawungSize, 0.0);
	endPoint[1] = vec3(kawungSize, -kawungSize, 0.0);
	endPoint[2] = vec3(-kawungSize, -kawungSize, 0.0);
	endPoint[3] = vec3(-kawungSize, kawungSize, 0.0);

	//rectangle filled
	glColor3d(0.0, 0.3, 0.1);
	glRectd(kawungSize, kawungSize, -kawungSize, -kawungSize);

	//rectangle border
	/*glColor3d(1.0, 1.0, 1.0);
	glLineWidth(2.0);
	glBegin(GL_LINE_STRIP);
	glVertex2d(kawungSize, kawungSize);
	glVertex2d(kawungSize, -kawungSize);
	glVertex2d(-kawungSize, -kawungSize);
	glVertex2d(-kawungSize, kawungSize);
	glVertex2d(kawungSize, kawungSize);
	glEnd();*/

	for (int i = 0; i < endPoint.size(); i++) {
		temp.start = centre;
		temp.end = endPoint[i];
		for (int j = 0; j < 2; j++) {
			if (j == 0) {
				temp.mid = vec3(0.0, temp.end.y, 0.0);
			}
			else {
				temp.mid = vec3(temp.end.x, 0.0, 0.0);
				temp.start = temp.end;
				temp.end = centre;
			}

			/*cout << temp.start.x << "," << temp.start.y << " ";
			cout << temp.mid.x << "," << temp.mid.y << " ";
			cout << temp.end.x << "," << temp.end.y << " ";
			cout << endl;*/

			a = 1.0;
			b = 1.0 - a;

			for (int k = 0; k < nPoints; k++) {
				bezierResults = getBezier(temp, a, b);
				//glVertex3d(bezierResults.x, bezierResults.y, bezierResults.z);
				kawungPts.push_back(bezierResults);
				a -= 1.0 / nPoints;
				b = 1.0 - a;
			}
		}
	}

	//cout << kawungPts.size();

	glColor3d(0.3, 0.0, 0.1);
	glBegin(GL_POLYGON);
	for (int i = 0; i < kawungPts.size(); i++) {
		glVertex2d(kawungPts[i].x, kawungPts[i].y);
	}
	glEnd();

	glColor3d(0.5, 0.0, 0.3);
	glLineWidth(2.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < kawungPts.size(); i++) {
		glVertex2d(kawungPts[i].x, kawungPts[i].y);
	}
	glEnd();

	drawFilledCircle(0.0, 0.0, 0.2);

	glFlush();
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

vec3 findMidXPoints(vec3 start, vec3 end, float offset) {
	float x = ((start.x + end.x) / 2) + offset;
	float y = (start.y + end.y) / 2;
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

void drawLittleSpiral(double a, double b) {
	//http://techtutorials95.blogspot.co.uk/2015/05/opengl-making-spirals.html

	double x, y, xx, yy;
	double theta = 0.0;
	
	x = a * exp(b * theta) * cos(theta);
	y = a * exp(b * theta) * sin(theta);

	for (int i = 0; i < 150; i++) {
		glBegin(GL_LINES);
		theta = 0.025 * i;
		xx = a * exp(b * theta) * cos(theta);
		yy = a * exp(b * theta) * sin(theta);

		glVertex2f((GLfloat)x, (GLfloat)y);
		glVertex2f(xx, yy);

		x = a * exp(b * theta) * cos(theta);
		y = a * exp(b * theta) * sin(theta);
		glEnd();
	}
}

void thueMorseAlgo(int r, int c) {
	//2-dimensional thue-morse
	//int tmSize = 4;
	thueMorse.resize(c);
	for (int i = 0; i < thueMorse.size(); i++) {
		thueMorse[i].resize(r);
	}

	for (int i = 0; i < thueMorse.size(); i++) {
		if (i == 0) {
			thueMorse[i][0] = 0;
		}
		else if (i == 1) {
			thueMorse[i][0] = 1;
		}
		else if (i > 1) {
			if (i % 2 == 0) {
				thueMorse[i][0] = thueMorse[i / 2][0];
			}
			else if (i % 2 > 0) {
				thueMorse[i][0] = 1 - thueMorse[i / 2][0];
			}
		}

		for (int j = 0; j < thueMorse[i].size(); j++) {
			if (j == 1) {
				thueMorse[i][j] = 1 - thueMorse[i][0];
			}
			else if (j > 1) {
				if (j % 2 == 0) {
					thueMorse[i][j] = thueMorse[i][j / 2];
				}
				else if (j % 2 > 0) {
					thueMorse[i][j] = 1 - thueMorse[i][j / 2];
				}
			}

			//cout << thueMorse[i][j] << " ";
		}
		//cout << endl;
	}
	//cout << endl;
}

//https://gist.github.com/strife25/803118
void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
	int triangleAmount = 20;		//number of triangles used

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);	//centre
	for (int i = 0; i <= triangleAmount; i++) {
		glVertex2f(
			x + (radius*cos(i*2.0f*PI / triangleAmount)), 
			y + (radius*sin(i*2.0f*PI / triangleAmount)));
	}
	glEnd();
}