#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <cstdlib>
#include <iostream>
using namespace std;

//Mostafa Mohamed Abdelzaher Khalifa 7000338
// 
//Below is all the details of all the added furniture and items and number of primitives used in each
//Also the controllers and the aim of the game
//Game ends by closing and exiting when you be in middle of golden star in middle of room

//DRAWN ASSETS
/*
Person 7 primitives

Goal Element 7 primitives (1 sphere , 6 cones) (made from joining 6 spinners)

walls with items on it for 3 primitives such items are:
'Frames' (but static colours),
a carpet,
a door

1) 4 Wall Frames 2 primitives each
2) 3 Carpets 1 primitive each
3) Bed 7 primitives
4) Nightstand 7 primitive
5) Wardrobe 9 primitive
6) Desk 4 primitives
7) Chair 6 primitives
8) Heater 8 primitives
9) Door 2 primitives
10) Mirror 4 primitives
11) Clock 3 primitives (Animation 1)
12) Table Spinner 2 primitives (Animation 2)
13) Table 5 primitives
14) Fan (I Liked it so I used it as extra to requirements)
*/

/*
As nothing says camera can not go out of room borders, 
I left it to be able to roam outside even after selecting top / side / front view

Press r to point camera inside and on the player
*/

/* HOW TO PLAY
MOVE PLAYER: i j k l
MOVE CAMERA: w a s d q e
ROTATE CAMERA: ARROW KEYS
CHANGE WALLS COLOURS: 1
RESET CAMERA: r
TOP VIEW: 2
SIDE VIEW: 3
FRONT VIEW: 4

AIM:
Touch the goal item (is obvious and in middle of room)
*/


/*
㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣�
㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣�
㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣㎣�

*/


#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)


//SETUP VARIABLES
float rotationAngle;
double wall;
double wall2;
double wallColour=1;
double wallColour2=1;

double playerX = 1.5;
double playerZ = 1.5;
int direction = 0;
double gameMinutes = 0;
double gameHour = 0;
double spinnerRotation = 0;
double cielingMove = 1;
double sideViewWallMove = 0;
double frontViewWallMove = 2;
double goalX = 1;
double goalZ = 1;

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 1.0f, float eyeY = 1.0f, float eyeZ = 1.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

Camera camera;

//-------------------------------------------------------------------------------------------------------------------------------------
void print(int x, int y, double R, double G, double B, char* string)
{//a method to draw any text on the screen
	glColor3f(R, G, B);
	glRasterPos2f(x, y);
	while (*string)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *string++);
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void drawWall(double thickness) {
	glPushMatrix();
	glTranslated(0.5, 0.5 * thickness, 0.5);
	glScaled(1.0, thickness, 1.0);
	glutSolidCube(1);
	glPopMatrix();
}
void drawTableLeg(double thick, double len) {
	glPushMatrix();
	glTranslated(0, len / 2, 0);
	glScaled(thick, len, thick);
	glutSolidCube(1.0);
	glPopMatrix();
}
void drawJackPart() {
	glPushMatrix();
	glScaled(0.2, 0.2, 1.0);
	glutSolidSphere(1, 15, 15);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, 1.2);
	glutSolidSphere(0.2, 15, 15);
	glTranslated(0, 0, -2.4);
	glutSolidSphere(0.2, 15, 15);
	glPopMatrix();
}
void drawJack() {
	glPushMatrix();
	drawJackPart();
	glRotated(90.0, 0, 1, 0);
	drawJackPart();
	glRotated(90.0, 1, 0, 0);
	drawJackPart();
	glPopMatrix();
}
void drawTable(double topWid, double topThick, double legThick, double legLen) {
	glPushMatrix();
	glTranslated(0, legLen, 0);
	glScaled(topWid, topThick, topWid);
	glutSolidCube(1.0);
	glPopMatrix();

	double dist = 0.95 * topWid / 2.0 - legThick / 2.0;
	glPushMatrix();
	glTranslated(dist, 0, dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(-2 * dist, 0, 2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glPopMatrix();
}


void person() {
	//Head
	glPushMatrix();
	glColor3d(0.9, 0.8, 0.6);
	glutSolidSphere(0.05, 15, 15);
	glRotated(180, 0, 1, 0);
	glutSolidCone(0.01, 0.1, 15, 15);
	glPopMatrix();

	//Body
	glPushMatrix();
	glColor3d(0.1, 0.2, 0.3);
	glTranslated(0, -0.11, 0);
	glScaled(1, 1.3, 1);
	glutSolidCube(0.12);
	glPopMatrix();

	//legR
	glPushMatrix();
	glColor3d(0, 0, 0);
	glTranslated(0.035, -0.28, 0);
	glScaled(0.4, 1.7, 0.4);
	glutSolidCube(0.12);
	glPopMatrix();

	//LegL
	glPushMatrix();
	glColor3d(0, 0, 0);
	glTranslated(-0.035, -0.28, 0);
	glScaled(0.4, 1.7, 0.4);
	glutSolidCube(0.12);
	glPopMatrix();

	//ArmR
	glPushMatrix();
	glColor3d(0.3, 0.2, 0.3);
	glTranslated(0.04, -0.1, 0);
	glRotated(90, 0, 1, 0);
	glutSolidCone(0.05, 0.12, 15, 15);
	glPopMatrix();

	//ArmL
	glPushMatrix();
	glColor3d(0.3, 0.2, 0.3);
	glTranslated(-0.04, -0.1, 0);
	glRotated(-90, 0, 1, 0);
	glutSolidCone(0.05, 0.12, 15, 15);
	glPopMatrix();
}

void Fan(float rotationAngle)
{
	glTranslatef(0, 5, -5.0);
	glScalef(5, 5, 5);
	//glRotatef(90, 1, 0, 0);
	//Cube
	glPushMatrix();
	glColor3f(1, 1, 1);
	glScalef(1, 1.2, 1);
	glScalef(.5, .4, .5);
	glTranslatef(0, -2.0, 0);
	glScalef(1.0f, 0.7f, 1.0f);
	glTranslatef(0, -0.5, 0);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);

	glPushMatrix();
	glTranslatef(0, -1.05, 0);
	glScalef(0.15, .15, .15);
	glTranslatef(0.0f, -2.2f, 0.0f);
	glBegin(GL_QUAD_STRIP);
	for (int j = 0; j <= 360; j += 5) {
		glColor3f(0.302, 0.149, 0.0);
		glVertex3f(cos(j), +1, sin(j));
		glColor3f(0.6, 0.302, 0.0);
		glVertex3f(cos(j), -1, sin(j));
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(.3, .3, .3);
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, 1.55);
	glScalef(.25, .25, .25);
	glutSolidTorus(.1, .72, 25, 25);
	glPopMatrix();

	glPushMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.4);
	glTranslatef(-1, -1.55, 0);
	glScalef(2.5, .1, .75);
	glutSolidCube(.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -1.55, 0);
	glScalef(0.05, .11, .05);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 0, -5);
	glScalef(1, 1, 2.4);
	glBegin(GL_QUAD_STRIP);
	for (int j = 0; j <= 360; j += 5) {
		glColor3f(.3, .3, .3);
		glVertex3f(cos(j), +1, sin(j));
		glColor3f(.7, .7, .7);
		glVertex3f(cos(j), -1, sin(j));
	}
	glEnd();
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.4);
	glTranslatef(-1, -1.55, 0);
	glScalef(2.5, .1, .75);
	glutSolidCube(.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -1.55, 0);
	glScalef(0.05, .11, .05);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 0, -5);
	glScalef(1, 1, 2.4);
	glBegin(GL_QUAD_STRIP);
	for (int j = 0; j <= 360; j += 5) {
		glColor3f(.3, .3, .3);
		glVertex3f(cos(j), +1, sin(j));
		glColor3f(.7, .7, .7);
		glVertex3f(cos(j), -1, sin(j));
	}
	glEnd();
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 0, 1, 0);

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.4);
	glTranslatef(-1, -1.55, 0);
	glScalef(2.5, .1, .75);
	glutSolidCube(.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -1.55, 0);
	glScalef(0.05, .11, .05);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 0, -5);
	glScalef(1, 1, 2.4);
	glBegin(GL_QUAD_STRIP);
	for (int j = 0; j <= 360; j += 5) {
		glColor3f(.3, .3, .3);
		glVertex3f(cos(j), +1, sin(j));
		glColor3f(.7, .7, .7);
		glVertex3f(cos(j), -1, sin(j));
	}
	glEnd();
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glRotatef(270, 0, 1, 0);
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.4);
	glTranslatef(-1, -1.55, 0);
	glScalef(2.5, .1, .75);
	glutSolidCube(.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -1.55, 0);
	glScalef(0.05, .11, .05);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 0, -5);
	glScalef(1, 1, 2.4);
	glBegin(GL_QUAD_STRIP);
	for (int j = 0; j <= 360; j += 5) {
		glColor3f(.3, .3, .3);
		glVertex3f(cos(j), +1, sin(j));
		glColor3f(.7, .7, .7);
		glVertex3f(cos(j), -1, sin(j));
	}
	glEnd();
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

void carpet() {

	glPushMatrix();
	glColor3f(0.3, 0.3, 0.3);
	glTranslatef(0.5, 0.02, 0.5);
	glScalef(1, 0.01, 1);
	glutSolidCube(1);
	glPopMatrix();
}

void frames(float r, float g, float b) {
	glPushMatrix();
	glColor3f(r, g, b);
	glScalef(0.5, 0.5, 0.01);
	glutSolidCube(1);
	glPopMatrix();
}

void bed()
{
	glTranslated(1.95, 0.19, 0.24);
	//bed headboard
	glPushMatrix();
	glColor3f(0.6, 0.6, 0.6);
	glScalef(0.3, 0.7, 0.96);
	glTranslatef(0, 0, 0);
	glutSolidCube(0.5);
	glPopMatrix();

	//Deco on headBoard
	glPushMatrix();
	glColor3f(0.2, 0.3, 0.2);
	glScalef(0.3, 0.7, 0.96);
	glTranslatef(0, 0.25, 0);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.1, 0.2, 15, 15);
	glPopMatrix();

	//bed body
	glPushMatrix();
	glColor3f(0.4, 0.4, 0.4);
	glScalef(1.7, 0.2, 0.9);
	glTranslatef(-0.2, -0.6, 0);
	glutSolidCube(0.5);
	glPopMatrix();

	//Bed body 2
	glPushMatrix();
	glColor3f(0.2, 0.2, 0.2);
	glScalef(1.4, 0.2, 0.9);
	glTranslatef(-0.21, -0.6, 0.01);
	glutSolidCube(0.5);
	glPopMatrix();

	//Matress
	glPushMatrix();
	glColor3f(0.9, 0.9, 0.9);
	glScalef(1.6, 0.2, 0.87);
	glTranslatef(-0.2, -0.2, 0);
	glutSolidCube(0.5);
	glPopMatrix();

	//Pillow
	glPushMatrix();
	glColor3f(0.1, 0.1, 0.5);
	glScalef(0.2, 0.2, 0.85);
	glTranslatef(-0.4, 0.2, 0);
	glRotatef(20, 0, 0, 1);
	glutSolidCube(0.5);
	glPopMatrix();

	//blanket
	glPushMatrix();
	glColor3f(0.1, 0.1, 0.5);
	glScalef(1, 0.03, 0.87);
	glTranslatef(-0.5, 0.5, 0);
	glutSolidCube(0.5);
	glPopMatrix();
}

void nightstand(){
	glTranslated(1.95, 0.07, 0.75);
	//Lower Drawer
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.2);
	glScalef(0.3, 0.25, 0.8);
	glTranslatef(0, 0.05, 0);
	glutSolidCube(0.5);
	glPopMatrix();

	//Upper Drawer
	glPushMatrix();
	glColor3d(0.4, 0.4, 0.4);
	glScalef(0.3, 0.25, 0.8);
	glTranslatef(0, 0.56, 0);
	glutSolidCube(0.5);
	glPopMatrix();

	//Handle top
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.2);
	glScalef(0.1, 0.1, 0.1);
	glTranslatef(-0.7, 1.3, 0.15);
	glutSolidSphere(0.4,15,15);
	glPopMatrix();

	//Handle bottom
	glPushMatrix();
	glColor3d(0.4, 0.4, 0.4);
	glScalef(0.1, 0.1, 0.1);
	glTranslatef(-0.7, 0.2, 0.15);
	glutSolidSphere(0.4, 15, 15);
	glPopMatrix();

	//Bars
	//Nearer to bed
	glPushMatrix();
	glColor3d(0.1, 0.1, 0.1);
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(0, 0.4, -0.2);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.1, 0.2, 15, 15);
	glPopMatrix();

	//Further from bed
	glPushMatrix();
	glColor3d(0.1, 0.1, 0.1);
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(0, 0.4, 0.25);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.1, 0.2, 15, 15);
	glPopMatrix();

	//Shelf
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.2);
	glScalef(0.3, 0.03, 0.8);
	glTranslatef(0, 9.68, 0.01);
	glutSolidCube(0.5);
	glPopMatrix();




}

void wardrobe() {
	glTranslated(1.95, 0.31, 1.2);
	//Left Wardrobe
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.2);
	glScalef(0.3, 1.45, 0.4);
	glTranslatef(0, 0.05, 0);
	glutSolidCube(0.5);
	glPopMatrix();

	//Handle Left
	glPushMatrix();
	glColor3d(0.8, 0.8, 0.8);
	glScalef(0.02, 0.2, 0.02);
	glTranslatef(-4, 0.6, 4);
	glutSolidCube(0.5);
	glPopMatrix();

	//Right Wardrobe
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.2);
	glScalef(0.3, 1.45, 0.4);
	glTranslatef(0, 0.05, 1);
	glutSolidCube(0.5);
	glPopMatrix();

	//Handle Right
	glPushMatrix();
	glColor3d(0.8, 0.8, 0.8);
	glScalef(0.02, 0.2, 0.02);
	glTranslatef(-4, 0.6, 16);
	glutSolidCube(0.5);
	glPopMatrix();

	//Top Shelf
	glPushMatrix();
	glColor3d(0.8, 0.8, 0.8);
	glScalef(0.3, 0.03, 0.4);
	glTranslatef(0, 10, 0.5);
	glutSolidCube(0.5);
	glPopMatrix();

	//Middle Shelf
	glPushMatrix();
	glColor3d(0.8, 0.8, 0.8);
	glScalef(0.3, 0.03, 0.4);
	glTranslatef(0, 6, 0.5);
	glutSolidCube(0.5);
	glPopMatrix();

	// Lower Shelf
	glPushMatrix();
	glColor3d(0.8, 0.8, 0.8);
	glScalef(0.3, 0.03, 0.4);
	glTranslatef(0, 2, 0.5);
	glutSolidCube(0.5);
	glPopMatrix();

	//Drawer
	glPushMatrix();
	glColor3d(0.8, 0.8, 0.8);
	glScalef(0.3, 0.35, 0.4);
	glTranslatef(0, -0.58, 0.5);
	glutSolidCube(0.5);
	glPopMatrix();

	//Handle Drawer
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.2);
	glScalef(0.02, 0.02, 0.2);
	glTranslatef(-4, -7, 1.02);
	glutSolidCube(0.5);
	glPopMatrix();

}

void desk() {
	//Left Storage
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.2);
	glScalef(0.3, 0.5, 0.4);
	glTranslatef(0.4, 0.25, 4.6);
	glutSolidCube(0.5);
	glPopMatrix();

	//Handle
	glPushMatrix();
	glColor3d(0.3, 0.3, 0.3);
	glTranslatef(0.19, 0.15, 1.78);
	glScalef(0.05, 0.2, 0.05);
	glutSolidCube(0.5);
	glPopMatrix();


	//Right Leg
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.2);
	glScalef(0.3, 0.5, 0.1);
	glTranslatef(0.4, 0.25, 14.65);
	glutSolidCube(0.5);
	glPopMatrix();

	//Desk
	glPushMatrix();
	glColor3d(0.3, 0.3, 0.3);
	glScalef(0.3, 0.1, 1);
	glTranslatef(0.4, 2.75, 1.69);
	glutSolidCube(0.5);
	glPopMatrix();




}

void chair() {
	//Leg1
	glPushMatrix();
	glColor3d(0.4, 0.4, 0.4);
	glTranslatef(0.25, 0.08, 1.7);
	glScalef(0.05, 0.25, 0.05);
	glutSolidCube(0.5);
	glPopMatrix();

	//Leg2
	glPushMatrix();
	glColor3d(0.4, 0.4, 0.4);
	glTranslatef(0.25, 0.08, 1.55);
	glScalef(0.05, 0.25, 0.05);
	glutSolidCube(0.5);
	glPopMatrix();

	//Leg3
	glPushMatrix();
	glColor3d(0.4, 0.4, 0.4);
	glTranslatef(0.35, 0.08, 1.7);
	glScalef(0.05, 0.25, 0.05);
	glutSolidCube(0.5);
	glPopMatrix();

	//Leg4
	glPushMatrix();
	glColor3d(0.4, 0.4, 0.4);
	glTranslatef(0.35, 0.08, 1.55);
	glScalef(0.05, 0.25, 0.05);
	glutSolidCube(0.5);
	glPopMatrix();

	//Sitting place
	glPushMatrix();
	glColor3d(0.4, 0.4, 0.4);
	glTranslatef(0.3, 0.15, 1.625);
	glScalef(0.45, 0.05, 0.45);
	glutSolidCube(0.5);
	glPopMatrix();

	//Back Rest
	glPushMatrix();
	glColor3d(0.4, 0.4, 0.4);
	glTranslatef(0.38, 0.23, 1.625);
	glScalef(0.05, 0.3, 0.42);
	glutSolidCube(0.5);
	glPopMatrix();

}

void heater() {
	//Vertical Bars
	//1
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.8);
	glTranslatef(0.9, 0.2, 0.015);
	glScalef(0.05, 0.5, 0.05);
	glutSolidCube(0.5);
	glPopMatrix();
	//2
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.8);
	glTranslatef(0.85, 0.2, 0.015);
	glScalef(0.05, 0.5, 0.05);
	glutSolidCube(0.5);
	glPopMatrix();
	//3
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.8);
	glTranslatef(0.8, 0.2, 0.015);
	glScalef(0.05, 0.5, 0.05);
	glutSolidCube(0.5);
	glPopMatrix();
	//4
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.8);
	glTranslatef(0.75, 0.2, 0.015);
	glScalef(0.05, 0.5, 0.05);
	glutSolidCube(0.5);
	glPopMatrix();
	//5
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.8);
	glTranslatef(0.7, 0.2, 0.015);
	glScalef(0.05, 0.5, 0.05);
	glutSolidCube(0.5);
	glPopMatrix();
	//Horizontal Bars
	//1
	glPushMatrix();
	glColor3d(0.2, 0.5, 0.8);
	glTranslatef(0.8, 0.25, 0.015);
	glScalef(0.45, 0.05, 0.05);
	glutSolidCube(0.5);
	glPopMatrix();
	//2
	glPushMatrix();
	glColor3d(0.2, 0.5, 0.8);
	glTranslatef(0.8, 0.15, 0.015);
	glScalef(0.45, 0.05, 0.05);
	glutSolidCube(0.5);
	glPopMatrix();

	//Knob
	glPushMatrix();
	glColor3d(0.8, 0.2, 0.2);
	glTranslatef(0.925, 0.31, 0.015);
	glScalef(0.03, 0.03, 0.03);
	glutSolidSphere(0.5,15,15);
	glPopMatrix();


}

void door() {

	//Door
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.4);
	glTranslatef(0.7, 0.41, 1.98);
	glScalef(0.9, 1.6, 0.05);
	glutSolidCube(0.5);
	glPopMatrix();

	//Knob
	glPushMatrix();
	glColor3d(0, 0, 0.6);
	glTranslatef(0.5, 0.41, 1.97);
	glScalef(0.04, 0.04, 0.04);
	glutSolidSphere(0.5, 15, 15);
	glPopMatrix();

}

void mirror() {
	//Stand
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.4);
	glTranslatef(0.09, 0.04, 1.2);
	glScalef(0.1, 0.1, 0.5);
	glutSolidCube(0.5);
	glPopMatrix();

	//Left Bar
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.4);
	glTranslatef(0.1, 0.28, 1.3);
	glScalef(0.05, 1, 0.05);
	glutSolidCube(0.5);
	glPopMatrix();

	//Right Bar
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.4);
	glTranslatef(0.1, 0.28, 1.1);
	glScalef(0.05, 1, 0.05);
	glutSolidCube(0.5);
	glPopMatrix();

	//Reflective area
	glPushMatrix();
	glColor3d(1, 1, 1);
	glTranslatef(0.1, 0.3, 1.2);
	glScalef(0.04, 0.8, 0.35);
	glutSolidCube(0.5);
	glPopMatrix();
}

void uhr() {
	//Clock body
	glPushMatrix();
	glColor3d(0.6, 0.6, 0.8);
	glTranslatef(0.005, 0.7 , 0.9);
	glScalef(0.02, 0.2, 0.2);
	glutSolidCube(0.5);
	glPopMatrix();

	//Minutes Hand
	glPushMatrix();
	glColor3d(0.1, 0.1, 0.1);
	glTranslatef(0.011, 0.7, 0.9);
	glRotated(-6*gameMinutes, 1, 0, 0);
	glTranslated(0, 0.01, 0);
	glScalef(0.01, 0.08, 0.01);
	glutSolidCube(0.5);
	glPopMatrix();

	//Hour Hand
	glPushMatrix();
	glColor3d(0.1, 0.7, 0.1);
	glTranslatef(0.012, 0.7, 0.9);
	glRotated(-30*gameHour, 1, 0, 0);
	glTranslated(0, 0.01, 0);
	glScalef(0.01, 0.05, 0.01);
	glutSolidCube(0.5);
	glPopMatrix();


}

void spinner(double r , double g , double b) {
	//Direction Cone
	glPushMatrix();
	glColor3d(r, g, b);
	glRotated(-90,1,0,0);
	glutSolidCone(0.08, 0.15, 15, 15);
	glPopMatrix();

	//Sphere
	glPushMatrix();
	glColor3d(r-0.1, g-0.1, b-0.1);
	glutSolidSphere(0.05, 15, 15);
	glPopMatrix();

}

void goalSTAR(double r, double g, double b) {
	glColor3b(0.5, 0.5, 0);

	spinner(r,g,b);

	glPushMatrix();
	glRotated(90, 1, 0, 0);
	spinner(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	spinner(r, g, b);
	glPopMatrix();
	
	glPushMatrix();
	glRotated(90, 0, 0, 1);
	spinner(r, g, b);
	glPopMatrix();
	
	glPushMatrix();
	glRotated(-90, 0, 0, 1);
	spinner(r, g, b);
	glPopMatrix();
	
	glPushMatrix();
	glRotated(180, 1, 0, 0);
	spinner(r, g, b);
	glPopMatrix();
}

//void lamp() {
//
//	//Direction Cone
//	glPushMatrix();
//	glColor3d(0.4, 0.4, 0.4);
//	glRotated(-90, 1, 0, 0);
//	glutSolidCone(0.1, 0.2, 15, 15);
//	glPopMatrix();
//
//	//Sphere
//	glPushMatrix();
//	glColor3d(0.3, 0.3, 0.3);
//	glutSolidSphere(0.05, 15, 15);
//	glPopMatrix();
//
//}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 720 / 700, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

void Display() {
	setupCamera();
	setupLights();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslated(-1, 0, -1);

	//Person
	glPushMatrix();
	glTranslated(playerX, 0.4, playerZ);
	glRotated(90*direction,0,1,0);
	person();
	glPopMatrix();

	//Carpets
	//1
	glPushMatrix();
	carpet();
	glPopMatrix();

	//2
	glPushMatrix();
	glTranslated(1, 0, 1);
	glScaled(0.5, 1, 0.5);
	carpet();
	glPopMatrix();

	//3
	glPushMatrix();
	glTranslated(1.5, 0, 1.5);
	glScaled(0.53, 1, 0.5);
	carpet();
	glPopMatrix();

	//Wall Frames
	//Above bed frames
	//Outer Part
	glPushMatrix();
	glTranslated(1.5, 0.5, 0.0069);
	frames(0.5,0.5,1);
	glPopMatrix();
	//Inner Part
	glPushMatrix();
	glTranslated(1.5, 0.5, 0.0075);
	glScaled(0.5, 0.5, 1);
	frames(0.5, 1, 0.5);
	glPopMatrix();

	//Above NightStand
	//Outer Part
	glPushMatrix();
	glTranslated(2.026, 0.6, 0.75);
	glRotated(90,0,1,0);
	glScaled(0.5, 0.5, 1);
	frames(0.5, 1, 1);
	glPopMatrix();
	//Inner Part
	glPushMatrix();
	glTranslated(2.024, 0.6, 0.75);
	glRotated(90, 0, 1, 0);
	glScaled(0.25, 0.25, 1);
	frames(1, 1, 0.5);
	glPopMatrix();

	//Above Table
	//Outer Part
	glPushMatrix();
	glTranslated(0.01, 0.6, 0.4);
	glRotated(90, 0, 1, 0);
	glScaled(0.7, 0.7, 1);
	frames(1, 0.5, 0.5);
	glPopMatrix();
	//Inner Part
	glPushMatrix();
	glTranslated(0.015, 0.6, 0.4);
	glRotated(90, 0, 1, 0);
	glScaled(0.35, 0.35, 1);
	frames(0.5, 1, 0.5);
	glPopMatrix();

	//Beside Door
	//Inner Part
	glPushMatrix();
	glTranslated(1.5, 0.5, 1.974);
	glScaled(0.8, 0.8, 1);
	frames(1, 0.5, 0.5);
	glPopMatrix();
	//Outer Part
	glPushMatrix();
	glTranslated(1.5, 0.5, 1.9705);
	glScaled(0.4, 0.4, 1);
	frames(0.5, 1, 0.5);
	glPopMatrix();

	//Bed
	glPushMatrix();
	bed();
	glPopMatrix();

	//Nightstand
	glPushMatrix();
	nightstand();
	glPopMatrix();

	//Wardrobe
	glPushMatrix();
	wardrobe();
	glPopMatrix();

	//Desk
	glPushMatrix();
	desk();
	glPopMatrix();

	//Chair
	glPushMatrix();
	chair();
	glPopMatrix();

	//Heater
	glPushMatrix();
	heater();
	glPopMatrix();

	//Door
	glPushMatrix();
	door();
	glPopMatrix();

	//Mirror
	glPushMatrix();
	mirror();
	glPopMatrix();

	//Clock (1st Animated Object)
	glPushMatrix();
	uhr();
	glPopMatrix();

	//Table Spinner (2nd Animated Object)
	glPushMatrix();
	glTranslated(0.35, 0.25, 0.35);
	glRotated(10*spinnerRotation,0,1,1);
	spinner(0.4,0.4,0.4);
	glPopMatrix();
	
	//Goal
	glPushMatrix();
	glTranslated(goalX, 0.18, goalZ);
	goalSTAR(0.5,0.5,0);
	glPopMatrix();

	//TABLE
	glPushMatrix();
	glTranslated(0.4, 0.0, 0.4);
	glColor3d(0.5, 0.5, 0.7);
	glScaled(0.8, 0.7, 0.8);
	drawTable(0.6, 0.02, 0.02, 0.3);
	glPopMatrix();
	
	//FLOOR
	glPushMatrix();
	glScaled(2.047, 1, 2);
	glColor3d(0, 0.5, 0.5);
	drawWall(0.02);
	glPopMatrix();

	//Ceiling
	glPushMatrix();
	glTranslated(0, cielingMove, 0);
	glScaled(2.047, 1, 2);
	glColor3d(0, 0.5, 0.5);
	drawWall(0.02);
	glPopMatrix();

	//SIDE WALL LEFT
	glPushMatrix();
	glColor3d(wallColour, wallColour2, wallColour / wallColour2);
	glTranslated(0, 0, sideViewWallMove);
	glRotated(90, 0, 0, 1.0);
	glScaled(1, 1, 2);
	drawWall(0.02);
	glPopMatrix();

	//SIDE WALL RIGHT
	glPushMatrix();
	glColor3d(wallColour, wallColour2, wallColour/wallColour2);
	glRotated(-90, 1.0, 0.0, 0.0);
	glScaled(2.047, 1, 1);
	drawWall(0.02);
	glPopMatrix();

	//SIDE WALL BACK RIGHT
	glPushMatrix();
	glColor3d(wallColour, wallColour2, wallColour / wallColour2);
	glTranslated(2.047, 0, 0);
	glRotated(90, 0, 0, 1.0);
	glScaled(1, 1, 2);
	drawWall(0.02);
	glPopMatrix();

	//SIDE WALL BACK LEFT
	glPushMatrix();
	glColor3d(wallColour, wallColour2, wallColour / wallColour2);
	glTranslated(0, 0, frontViewWallMove);
	glRotated(-90, 1.0, 0.0, 0.0);
	glScaled(2.047, 1, 1);
	drawWall(0.02);
	glPopMatrix();



	//FAN (ANIMATED)
	glPushMatrix();
	glTranslatef(1, 1, 1);
	glScalef(0.05, 0.05, 0.05);
	Fan(rotationAngle);
	glPopMatrix();


	glFlush();
}

void Keyboard(unsigned char key, int x, int y) {
	float d = 0.01;

	switch (key) {
	
	case 'w': //Move camera look up
		camera.moveY(d);
		break;
	case 's': //Move camera look down
		camera.moveY(-d);
		break;
	case 'a': //Move camera look left
		camera.moveX(d);
		break;
	case 'd': //Move camera look right
		camera.moveX(-d);
		break;
	case 'q': //Move camera Zoom In
		camera.moveZ(d);
		break;
	case 'e': //Move camera Zoom Out
		camera.moveZ(-d);
		break;

	case'i': //Move person forward
		direction = 0;
		if (playerZ > 0.1) { playerZ -= 0.1; }
		break;

	case'k': //Move person backward
		direction = 2; 
		if (playerZ < 1.9) { playerZ += 0.1; }
		break;

	case'j': // Move person left
		direction = 1;
		if (playerX > 0.1) { playerX -= 0.1; }
		break;

	case'l': //Move person right
		direction = 3;
		if (playerX < 1.9) { playerX += 0.1; }
		break;

	case'1': //Change walls colours
		wallColour = wall;
		wallColour2 = wall2;
		break;

	case'2': //Top View
		camera.eye.x = 0.3;
		camera.eye.y = 3.5;
		camera.eye.z = 0.3;
		camera.center.x = 0;
		camera.center.y = 0;
		camera.center.z = 0;
		camera.up.x = 0;
		camera.up.y = 1;
		camera.up.z = 0;
		cielingMove = 15;
		sideViewWallMove = 0;
		frontViewWallMove = 2;
		break;

	case'3': //Side View
		camera.eye.x = -3;
		camera.eye.y = 1;
		camera.eye.z = 0.2;
		camera.center.x = 0;
		camera.center.y = 0.75;
		camera.center.z = 0.2;
		camera.up.x = 0;
		camera.up.y = 1;
		camera.up.z = 0;
		sideViewWallMove = -15;
		cielingMove = 1;
		frontViewWallMove = 2;
		break;

	case'4': //Front View
		camera.eye.x = 1;
		camera.eye.y = 1;
		camera.eye.z = 1.5;
		camera.center.x = 0;
		camera.center.y = 0;
		camera.center.z = 0;
		camera.up.x = 0;
		camera.up.y = 1;
		camera.up.z = 0;
		frontViewWallMove = -15;
		sideViewWallMove = 0;
		cielingMove = 1;
		break;

	case'r': //Reset camera
		camera.eye.x = 1;
		camera.eye.y = 1;
		camera.eye.z = 1;
		camera.center.x = 0;
		camera.center.y = 0;
		camera.center.z = 0;
		camera.up.x = 0;
		camera.up.y = 1;
		camera.up.z = 0;
		frontViewWallMove = 2;
		sideViewWallMove = 0;
		cielingMove = 1;
		camera.moveZ(0.1);
		break;



	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();
}
void Special(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}

void animations() {

	//Wall colouration
	srand((unsigned)time(NULL));
	wall = ((double)rand() / (RAND_MAX));
	wall2 = ((double)rand() / (RAND_MAX));

	//Fan rotation
	rotationAngle += 0.1f;

	//Clock movement
	gameMinutes+=0.01;
	gameHour = gameMinutes / 60;

	//Spinner movement
	spinnerRotation += 0.05;

	glutPostRedisplay();
}

void gameEnd(int v) {
	//if(playerX == goalX && playerZ == goalZ){
	if ((playerX > goalX-0.05 && playerX < goalX+0.05) && (playerZ > goalZ - 0.05 && playerZ < goalZ + 0.05)) {
		std::cout << "HERE" << endl;
		print(360, 360, 0, 0, 0, "You win. GAME OVER");
		exit(EXIT_SUCCESS);
	}
	glutTimerFunc(100, gameEnd, v);

}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(720, 720);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("7000338 Assignment2");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);
	glutTimerFunc(0, gameEnd, 0);
	glutIdleFunc(animations);

	glutMainLoop();
}
