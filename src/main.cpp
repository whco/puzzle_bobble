/*
* made in windows10 visual studio 19 environment
*
* zip-off "opengl.zip"
* Copy “include” and “lib” folder included in "opengl.zip" to your project folder
* Copy “glut32.dll”included in "opengl.zip" > "dll" to your project folder or exe folder
*
* Project > Properties > C/C++ > General > Additional include directories > Type “./include"
* Project > Properties > Linker > General > Additional library directories > Type“./lib"
* Project > Properties > Linker > Input > Additional dependencies > Add“glut32.lib"
*
*/

#include <iostream>
#include <vector>
#include <time.h>
#include "SolidSphere.h"
#include "Light.h"
#include <cmath>


using namespace std;

clock_t start = clock();
clock_t end_;
const float fps = 1000 / 60.0;

int score = 0;

#define WIDTH 500
#define HEIGHT 750
#define UPPER HEIGHT/4
#define LOWER HEIGHT/4

#define boundaryX WIDTH/2
#define boundaryY HEIGHT/2
#define PI 3.141592
#define ANGLE PI/36

vector<SolidSphere> spheres;

Light* light;
int bn = 20;
float rad = WIDTH / bn;
int phase = 1;
bool restart = false;
SolidSphere moving;
SolidSphere waiting;
SolidSphere nextBall;

//initinalize light, wating sphere, nextball
void init() {
	light = new Light(boundaryX, boundaryY, boundaryX / 2, GL_LIGHT0);
	light->setAmbient(0.5, 0.5, 0.5, 1.0);
	light->setDiffuse(0.7, 0.7, 0.7, 1.0);
	light->setSpecular(1.0, 1.0, 1.0, 1.0);


	SolidSphere sphere1(rad, 30, 30);
	sphere1.setCenter(0, -boundaryY + rad, 0);
	sphere1.setVelocity(0, 0, 0);
	waiting = sphere1;

	SolidSphere sphere2(rad, 30, 30);
	sphere2.setCenter(-200, -300, 0);
	sphere2.setVelocity(0, 0, 0);
	nextBall = sphere2;
}


//check whether a sphere touched the ceiling 
bool ceilingcheck(SolidSphere sp) {
	return((sp.getCenter()[1] + sp.getProperties()[0] >= boundaryY - UPPER)
		&& (sp.getVelocity()[1] > 0));
}

//fix coordinates where spheres could fit in
int m0 = 5 + (HEIGHT - UPPER - rad) / (sqrt(3) * rad), m1 = WIDTH / rad;
Vector3** Locs = new Vector3 * [m0];
void setLocs() {

	for (int i = 0; i < m0; ++i)
		Locs[i] = new Vector3[m1];
	for (int i = 0; i < m0; ++i)
		for (int j = 0; j < m1; ++j) {
			if (i % 2 == 1)
				Locs[i][j] = Vector3(-boundaryX + (2 * j + 1) * rad, boundaryY - UPPER - (sqrt(3) * i - sqrt(3) + 1) * rad, 0);
			if (i % 2 == 0)
				Locs[i][j] = Vector3(-boundaryX + (2 * j + 2) * rad, boundaryY - UPPER - (sqrt(3) * i - sqrt(3) + 1) * rad, 0);
		}
}

//search the nearest coordinate a sphere can fit in
Vector3 findnearest(SolidSphere& sph) {
	setLocs();

	float x = sph.getCenter()[0];
	float y = sph.getCenter()[1];
	int ynear = 0;
	int xnear = 0;
	for (int i = 1; i <= m0; i++) {
		if (y > Locs[0][0][1]) { ynear = 0; }
		else if (Locs[i - 1][0][1] >= y && y >= Locs[i][0][1]) {
			if (-y + Locs[i - 1][0][1] >= -Locs[i][0][1] + y) ynear = i;
			if (-y + Locs[i - 1][0][1] <= -Locs[i][0][1] + y) ynear = i - 1;
			//ynear = i;
		}
		for (int j = 1; j <= m1; j++) {
			if (x < Locs[ynear][0][0]) { xnear = 0; }
			if (x > Locs[ynear][m1 - 1][0]) { xnear = m1 - 1; }
			if (Locs[ynear][j - 1][0] <= x && x <= Locs[ynear][j][0]) {
				if (x - Locs[ynear][j - 1][0] >= Locs[ynear][j][0] - x) xnear = j;
				if (x - Locs[ynear][j - 1][0] <= Locs[ynear][j][0] - x) xnear = j - 1;
			}
		}
	}
	sph.setLocsIndex(Vector3(ynear, xnear, 0));
	return Locs[ynear][xnear];
}


vector<SolidSphere> SameColors;
vector<int> same;

//search neighborhood spheres with same color
void searchnear(SolidSphere& sph) {
	int existcheck = 0;
	int i = 0;
	int ii = 0;

	while (ii < spheres.size()) {

		if (spheres[ii].getColorIndex() == sph.getColorIndex() &&
			(dotProduct(sph.getCenter() - spheres[ii].getCenter(),
				sph.getCenter() - spheres[ii].getCenter()) <= 4 * rad * rad + 100)
			)
		{
			existcheck += 1;
		}
		ii += 1;
	}

	while (i < spheres.size()) {

		if (existcheck == 0) {
			return;
		}

		if (spheres[i].getColorIndex() == sph.getColorIndex() &&
			(dotProduct(sph.getCenter() - spheres[i].getCenter(),
				sph.getCenter() - spheres[i].getCenter()) <= 4 * rad * rad + 100)
			)
		{
			int count = 0;
			for (int k = 0; k < SameColors.size(); k++)
			{
				if (i == same[k]) {
					count += 1;
				}
			}
			if (count == 0) {
				SameColors.push_back(spheres[i]);
				same.push_back(i);
			}
		}
		i += 1;



	}
}


void remove(SolidSphere sph) {
	searchnear(sph);
	for (int k = 0; k < SameColors.size(); k++) { searchnear(SameColors[k]); }
	if (SameColors.size() >= 3) {
		int temp = 0;

		sort(same.begin(), same.end(), greater<int>());

		int k = 0;
		int fixed_size = same.size();
		while (k < fixed_size) {
			spheres.erase(spheres.begin() + same[k]);
			k += 1;
			score += 100;
		}
	}
	moving.setCenter(-1000, -1000, 0);
	glutPostRedisplay();
	SameColors.clear();
	same.clear();

}

void gameover() {
	for (int i = 0; i < (signed int)spheres.size(); i++) {
		if (spheres[i].getCenter()[1] - rad < -boundaryY + LOWER + 10)
		{
			phase = 5;
		}
	}
}

vector<SolidSphere> tocheck;
void dropsearch(SolidSphere& sph) {

	if (sph.getdropcheck() == 1)
		return;
	else sph.setdrop1(sph);

	for (int i = 0; i < spheres.size(); i++) {
		if (dotProduct(sph.getCenter() - spheres[i].getCenter(), sph.getCenter() - spheres[i].getCenter()) <= 4 * rad * rad + 100
			&& spheres[i].getdropcheck() == 0)
		{
			dropsearch(spheres[i]);
		}
	}

}

vector<SolidSphere> dropping;
void drop() {
	for (int i = 0; i < spheres.size(); i++) {
		if (spheres[i].getLocsIndex()[0] == 1) {
			dropsearch(spheres[i]);
		}
	}


	for (int i = 0; i < spheres.size(); i++) {
		if (spheres[i].getdropcheck() == 0) {
			dropping.push_back(spheres[i]);
			spheres[i].setCenter(1000, 2000, 0);
			spheres.erase(spheres.begin() + i);
			glutPostRedisplay();
		}
	}

	for (int i = 0; i < spheres.size(); i++) {
		int count = 0;
		for (int j = 0; j < spheres.size(); j++)
		{
			if (dotProduct(spheres[j].getCenter() - spheres[i].getCenter(), spheres[j].getCenter() - spheres[i].getCenter())
				<= 4 * rad * rad + 100)
				count += 1;
		}
		if (count == 1 && spheres[i].getLocsIndex()[0] != 1) {
			dropping.push_back(spheres[i]);
			spheres.erase(spheres.begin() + i);
		}

	}


	for (int i = 0; i < spheres.size(); i++) { spheres[i].setdrop0(spheres[i]); }

}

double LinebarLength = 150;
double LinebarLife = 400;
double LinebarVel = LinebarLength / LinebarLife;
int left_clicks;
int right_clicks;
Vector3 lastvel;

void click_up() {
	phase = 3;
	double theta = ANGLE * (right_clicks - left_clicks);
	waiting.setVelocity(10 * sin(theta), 10 * cos(theta), 0);
	waiting.move();
	moving = waiting;
	waiting = nextBall;
	waiting.setCenter(0, -boundaryY + rad, 0);
	SolidSphere sph(rad, 30, 30);
	sph.setCenter(-200, -300, 0);
	sph.setVelocity(0, 0, 0);
	nextBall = sph;
	phase = 2;
}

void idle() {

	end_ = clock();

	if (end_ - start > fps) {
		if (
			(
				(
					(moving.getCenter()[0] + moving.getProperties()[0] >= boundaryX)
					&& (moving.getVelocity()[0] > 0)
					)
				|| (
					(moving.getCenter()[0] - moving.getProperties()[0] <= -boundaryX)
					&& (moving.getVelocity()[0] < 0)
					)
				)
			)
			moving.setVelocity(-moving.getVelocity()[0], moving.getVelocity()[1], moving.getVelocity()[2]);
		bool flag = ceilingcheck(moving);

		for (int j = 0; j < (signed int)spheres.size(); j++)
			flag = flag || moving.collisionDetection(spheres[j]);

		if (flag == true)
		{
			phase = 3;
			LinebarLength = 150;
			Vector3 near = findnearest(moving);
			lastvel = moving.getVelocity();
			moving.setVelocity(0, 0, 0);
			moving.setCenter(near);
			spheres.push_back(moving);
			remove(moving);


			gameover();
			if (phase == 5)return;
			phase = 4;

		}
		for (int i = 0; i < dropping.size(); i++) {
			dropping[i].setVelocity(0, -20, 0);
			dropping[i].move();
			if (dropping[i].getCenter()[1] < -boundaryY + LOWER) {
				dropping[i].setCenter(1000, 2000, 0);
				dropping.erase(dropping.begin() + i);
			}
		}
		drop();
		moving.move();

		if (phase != 5) {
			if (LinebarLength > 0) {
				LinebarLength -= LinebarVel;
			}
			if (LinebarLength == 0) {
				click_up();
				LinebarLength = 150;
			}
		}
		start = end_;
	}

	glutPostRedisplay();
}

Vector3 rotateleft(Vector3 vec) {
	double pi = 3.141592;
	Vector3 vec2(vec[0] * cos(ANGLE) - vec[1] * sin(ANGLE),
		vec[0] * sin(ANGLE) + vec[1] * cos(ANGLE), vec[2]);
	return vec2;

}
Vector3 rotateright(Vector3 vec) {
	double pi = 3.141592;
	Vector3 vec2(vec[0] * cos(ANGLE) + vec[1] * sin(ANGLE),
		vec[0] * -sin(ANGLE) + vec[1] * cos(ANGLE), vec[2]);
	return vec2;
}


void processSpecialKeys(int key, int x, int y) {
	printf("You pressed %c at (%d, %d)\n", key, x, y);
	switch (key) {
	case GLUT_KEY_UP: {
		if (phase != 2 && phase != 3 && phase != 5) {
			click_up();
			LinebarLength = 150;
		}
	}
					break;
	case GLUT_KEY_LEFT:
		if (phase != 5) {
			if (right_clicks - left_clicks > -16) {
				waiting.setVelocity(rotateleft(lastvel));
				lastvel = waiting.getVelocity();
				++left_clicks;
			}
		}
		break;
	case GLUT_KEY_RIGHT:
		if (phase != 5) {
			if (right_clicks - left_clicks < 16) {
				waiting.setVelocity(rotateright(lastvel));
				lastvel = waiting.getVelocity();
				++right_clicks;
			}
		}
		break;
	}
}

void processNormalKey(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
}


void drawLine1() {
	glColor3f(1, 1, 1);
	glLineWidth(10);
	glEnable(GL_LINE);
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glVertex2f(-boundaryX, -boundaryY + LOWER);// A
	glColor3f(1, 1, 1);
	glVertex2f(boundaryX, -boundaryY + LOWER);// B
	glEnd();
}
void drawLine2() {
	glColor3f(1, 1, 1);
	glLineWidth(10);
	glEnable(GL_LINE);
	glBegin(GL_LINES);
	glVertex2f(-boundaryX, boundaryY - LOWER);// A
	glVertex2f(boundaryX, boundaryY - LOWER);// B
	glEnd();
}

void drawLinebar() {
	glColor3f(1, 1, 1);
	glLineWidth(30);
	glEnable(GL_LINE);
	glBegin(GL_LINES);
	glVertex2f(boundaryX - 200, boundaryY - 100);// A
	glVertex2f(boundaryX - 200 + LinebarLength, boundaryY - 100);// B
	glEnd();
}


void drawArrow() {
	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);
	glTranslatef(0, -boundaryY + rad, 0);
	glRotatef(-90, 1, 0, 0);
	glRotatef(5 * (right_clicks - left_clicks), 0, 1, 0);

	GLUquadricObj* quadratic;
	quadratic = gluNewQuadric();
	gluCylinder(quadratic, 20, 0, 50, 32, 32);
	glPopMatrix();
}
void draw_characters(void* font, const char* c, float x, float y) {
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos2f(x, y);
	for (int i = 0; i < strlen(c); i++)
		glutBitmapCharacter(font, c[i]);
}

void renderScene() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_characters(GLUT_BITMAP_TIMES_ROMAN_24, "TIME", 150, 300);
	draw_characters(GLUT_BITMAP_TIMES_ROMAN_24, "NEXT", -200, -250);
	if (phase == 5) {
		draw_characters(GLUT_BITMAP_TIMES_ROMAN_24, "GAMEOVER!!!!Start New Game", -150, 0);
		draw_characters(GLUT_BITMAP_TIMES_ROMAN_24, "Press esc to exit", -150, -100);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-boundaryX, boundaryX, -boundaryY, boundaryY, -100.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	light->draw();

	glColor3f(1, 1, 1);

	for (auto sph : spheres)
		sph.draw();
	moving.draw();
	waiting.draw();
	drawArrow();
	nextBall.draw();

	for (auto sph : dropping)
		sph.draw();


	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);


	glColor3f(0.5, 0.5, 0.5);
	drawLine1();
	drawLine2();
	drawLinebar();


	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Puzzle Bobble");
	init();

	// register callbacks
	glutDisplayFunc(renderScene);
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processNormalKey);
	glutIdleFunc(idle);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}
