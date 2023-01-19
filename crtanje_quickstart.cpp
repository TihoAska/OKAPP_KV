#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "GL/glut.h"
#include "../../sndfile.hh"


#include <math.h>
#include <iostream>

#include <windows.h>
#include <mmsystem.h>
#include <io.h>


#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <cstdlib>
#include<minmax.h>


using namespace std;
void display(void);
void reshape(int, int);
void idle();
void readSensors(unsigned char, int, int);
void display_sensors_new(unsigned char key, int x, int y);
void play_left();
void play_right();


int lsf;
int rsf;
int rsr;
int lsr;

int gear = 1;

int polje[4] = { 0, 0, 0, 0 };

unsigned char* loadPPM(const char* filename, int& width, int& height) {
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}
	retval_fgets = fgets(buf[0], BUFSIZE, fp);
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);

	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}
	return rawData;
}

void initGL()
{
	glEnable(GL_TEXTURE_2D); // enable texture mapping
	glShadeModel(GL_SMOOTH); // enable smooth shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // get clear background (black color)
	glClearDepth(1.0f); // color depth buffer
	glDepthFunc(GL_LEQUAL); // configuration of depth testing
							//enable additional options regarding: perspective correction, anti-aliasing, etc
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

void loadTexture()
{
	GLuint texture[1]; // declaring space for one texture
	int twidth, theight; // declaring variable for width and height of an image
	unsigned char* tdata; // declaring pixel data
						  // loading image data from specific file:
	tdata = loadPPM("..\\auto3.ppm", twidth, theight);
	if (tdata == NULL) return; // check if image data is loaded
							   //generating a texture to show the image
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE,
		tdata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

int main(int argc, char** argv) {
	/* 1) INITIALIZATION */
	// initialize GLUT
	glutInit(&argc, argv);
	// set window position and size
	glutInitWindowPosition(545, 180);
	glutInitWindowSize(720, 720);
	// set the combination of predefined values for display mode
	// set color space (Red, Green, Blue - RGB)
	// alocate depth buffer
	// set the size of the buffer (double)
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	// create window
	glutCreateWindow("Screen");
	/* 2) REGISTRATION OF CALLBACK FUNCTION */
	// function called when new window need to be drawn
	glutDisplayFunc(display);

	glutKeyboardFunc(display_sensors_new);
	// function called when window changes the size
	glutReshapeFunc(reshape);
	// function called when nothing else is executing and CPU is free
	glutIdleFunc(idle);
	loadTexture();   //enable this to load image
	initGL();
	/* 3) START GLUT PROCESSING CYCLE */
	glutMainLoop();

	return 0;
}

void LF1() {
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.5, 0.5);
	glVertex3f(-1.15f, -0.70f, 0.0f);
	glVertex3f(-1.1f, -0.65f, 0.0f);
	glVertex3f(-1.4f, -0.2f, 0.0f);
	glVertex3f(-1.45f, -0.25f, 0.0f);
	glEnd();
}
void LF2() {
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.5, 0.5);
	glVertex3f(-1.15f, -0.70f, 0.0f);
	glVertex3f(-1.1f, -0.65f, 0.0f);
	glVertex3f(-1.4f, -0.2f, 0.0f);
	glVertex3f(-1.45f, -0.25f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1, 0.2, 0.2);
	glVertex3f(-1.10f, -0.55f, 0.0f);
	glVertex3f(-1.05f, -0.5f, 0.0f);
	glVertex3f(-1.25f, -0.2f, 0.0f);
	glVertex3f(-1.3f, -0.25f, 0.0f);
	glEnd();
}
void LF3() {
	glBegin(GL_QUADS); //prva linija prednja lijeva strana
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1.05f, -0.40f, 0.0f);
	glVertex3f(-1.00f, -0.35f, 0.0f);
	glVertex3f(-1.10f, -0.2f, 0.0f);
	glVertex3f(-1.15f, -0.25f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1, 0.2, 0.2);
	glVertex3f(-1.10f, -0.55f, 0.0f);
	glVertex3f(-1.05f, -0.5f, 0.0f);
	glVertex3f(-1.25f, -0.2f, 0.0f);
	glVertex3f(-1.3f, -0.25f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1.0, 0.5, 0.5);
	glVertex3f(-1.15f, -0.70f, 0.0f);
	glVertex3f(-1.1f, -0.65f, 0.0f);
	glVertex3f(-1.4f, -0.2f, 0.0f);
	glVertex3f(-1.45f, -0.25f, 0.0f);
	glEnd();
}

void RF1() {
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.5, 0.5);
	glVertex3f(-1.15f, 0.70f, 0.0f);
	glVertex3f(-1.1f, 0.65f, 0.0f);
	glVertex3f(-1.4f, 0.2f, 0.0f);
	glVertex3f(-1.45f, 0.25f, 0.0f);
	glEnd();
}
void RF2() {
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.5, 0.5);
	glVertex3f(-1.15f, 0.70f, 0.0f); //1
	glVertex3f(-1.1f, 0.65f, 0.0f); //2 
	glVertex3f(-1.4f, 0.2f, 0.0f); //3
	glVertex3f(-1.45f, 0.25f, 0.0f); //4
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1, 0.2, 0.2);
	glVertex3f(-1.10f, 0.55f, 0.0f);
	glVertex3f(-1.05f, 0.5f, 0.0f);
	glVertex3f(-1.25f, 0.2f, 0.0f);
	glVertex3f(-1.3f, 0.25f, 0.0f);
	glEnd();
}
void RF3() {
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.5, 0.5);
	glVertex3f(-1.15f, 0.70f, 0.0f); //1
	glVertex3f(-1.1f, 0.65f, 0.0f); //2 
	glVertex3f(-1.4f, 0.2f, 0.0f); //3
	glVertex3f(-1.45f, 0.25f, 0.0f); //4
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1, 0.2, 0.2);
	glVertex3f(-1.10f, 0.55f, 0.0f);
	glVertex3f(-1.05f, 0.5f, 0.0f);
	glVertex3f(-1.25f, 0.2f, 0.0f);
	glVertex3f(-1.3f, 0.25f, 0.0f);
	glEnd();

	glBegin(GL_QUADS); //prva linija prednja lijeva strana 
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1.05f, 0.40f, 0.0f);
	glVertex3f(-1.00f, 0.35f, 0.0f);
	glVertex3f(-1.10f, 0.2f, 0.0f);
	glVertex3f(-1.15f, 0.25f, 0.0f);
	glEnd();
}

void RB1() {
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.5, 0.5);
	glVertex3f(1.15f, 0.70f, 0.0f); //1
	glVertex3f(1.1f, 0.65f, 0.0f); //2 
	glVertex3f(1.4f, 0.2f, 0.0f); //3
	glVertex3f(1.45f, 0.25f, 0.0f); //4
	glEnd();
}
void RB2() {
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.5, 0.5);
	glVertex3f(1.15f, 0.70f, 0.0f); //1
	glVertex3f(1.1f, 0.65f, 0.0f); //2 
	glVertex3f(1.4f, 0.2f, 0.0f); //3
	glVertex3f(1.45f, 0.25f, 0.0f); //4
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1, 0.2, 0.2);
	glVertex3f(1.10f, 0.55f, 0.0f);
	glVertex3f(1.05f, 0.5f, 0.0f);
	glVertex3f(1.25f, 0.2f, 0.0f);
	glVertex3f(1.3f, 0.25f, 0.0f);
	glEnd();
}
void RB3() {
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.5, 0.5);
	glVertex3f(1.15f, 0.70f, 0.0f); //1
	glVertex3f(1.1f, 0.65f, 0.0f); //2 
	glVertex3f(1.4f, 0.2f, 0.0f); //3
	glVertex3f(1.45f, 0.25f, 0.0f); //4
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1, 0.2, 0.2);
	glVertex3f(1.10f, 0.55f, 0.0f);
	glVertex3f(1.05f, 0.5f, 0.0f);
	glVertex3f(1.25f, 0.2f, 0.0f);
	glVertex3f(1.3f, 0.25f, 0.0f);
	glEnd();

	glBegin(GL_QUADS); //prva linija prednja lijeva strana 
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(1.05f, 0.40f, 0.0f);
	glVertex3f(1.00f, 0.35f, 0.0f);
	glVertex3f(1.10f, 0.2f, 0.0f);
	glVertex3f(1.15f, 0.25f, 0.0f);
	glEnd();
}

void LB1() {
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.5, 0.5);
	glVertex3f(1.15f, -0.70f, 0.0f);
	glVertex3f(1.1f, -0.65f, 0.0f);
	glVertex3f(1.4f, -0.2f, 0.0f);
	glVertex3f(1.45f, -0.25f, 0.0f);
	glEnd();
}
void LB2() {
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.5, 0.5);
	glVertex3f(1.15f, -0.70f, 0.0f); //1
	glVertex3f(1.1f, -0.65f, 0.0f); //2 
	glVertex3f(1.4f, -0.2f, 0.0f); //3
	glVertex3f(1.45f, -0.25f, 0.0f); //4
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1, 0.2, 0.2);
	glVertex3f(1.10f, -0.55f, 0.0f);
	glVertex3f(1.05f, -0.5f, 0.0f);
	glVertex3f(1.25f, -0.2f, 0.0f);
	glVertex3f(1.3f, -0.25f, 0.0f);
	glEnd();
}
void LB3() {

	glBegin(GL_QUADS);
	glColor3f(1.0, 0.5, 0.5);
	glVertex3f(1.15f, -0.70f, 0.0f); //1
	glVertex3f(1.1f, -0.65f, 0.0f); //2 
	glVertex3f(1.4f, -0.2f, 0.0f); //3
	glVertex3f(1.45f, -0.25f, 0.0f); //4
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1, 0.2, 0.2);
	glVertex3f(1.10f, -0.55f, 0.0f);
	glVertex3f(1.05f, -0.5f, 0.0f);
	glVertex3f(1.25f, -0.2f, 0.0f);
	glVertex3f(1.3f, -0.25f, 0.0f);
	glEnd();

	glBegin(GL_QUADS); //prva linija prednja lijeva strana 
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(1.05f, -0.40f, 0.0f);
	glVertex3f(1.00f, -0.35f, 0.0f);
	glVertex3f(1.10f, -0.2f, 0.0f);
	glVertex3f(1.15f, -0.25f, 0.0f);
	glEnd();
}


void display() {
	cerr << "display callback" << endl;
	// clean color buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// start drawing quads
	glBegin(GL_QUADS);
	// choose color (white)
	glColor3f(1, 1, 1);
	// coordinates of initial white rectangle for the background
	glTexCoord2f(0, 1); glVertex3f(-2, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(2, -1, 0);
	glTexCoord2f(1, 0); glVertex3f(2, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(-2, 1, 0);
	glEnd();

	// swap buffers to show new graphics
	//glutSwapBuffers();

	if (polje[0] == 1) LF1();
	if (polje[0] == 2) LF2();
	if (polje[0] == 3) LF3();

	if (polje[1] == 1) RF1();
	if (polje[1] == 2) RF2();
	if (polje[1] == 3) RF3();

	if (polje[2] == 1) RB1();
	if (polje[2] == 2) RB2();
	if (polje[2] == 3) RB3();

	if (polje[3] == 1) LB1();
	if (polje[3] == 2) LB2();
	if (polje[3] == 3) LB3();

	glutSwapBuffers();
}


void reshape(int width, int height)
{
	cerr << "reshape callback" << endl;
	// specify the desired rectangle
	glViewport(0, 0, width, height);
	// switch to matrix projection
	glMatrixMode(GL_PROJECTION);
	// clean projection matrix
	glLoadIdentity();
	// set camera view (orthographic projection with 4x4 unit square canvas)
	glOrtho(-2, 2, -2, 2, 2, -2);
	// swith back to matrix
	glMatrixMode(GL_MODELVIEW);
}

void idle()
{
	if (gear == 1) {
		if (lsf == rsf) {
			if (lsf == 1) {
				PlaySound("both.wav", NULL, SND_ASYNC);
				Sleep(1000);
			}
			if (lsf == 2) {
				PlaySound("both.wav", NULL, SND_ASYNC);
				Sleep(700);
			}
			if (lsf == 3) {
				PlaySound("both.wav", NULL, SND_ASYNC);
				Sleep(300);
			}
		}

		else if ((lsf > rsf)) {
			if (lsf == 1) {
				PlaySound("left.wav", NULL, SND_ASYNC);
				Sleep(1000);
			}
			if (lsf == 2) {
				PlaySound("left.wav", NULL, SND_ASYNC);
				Sleep(700);
			}
			if (lsf == 3) {
				PlaySound("left.wav", NULL, SND_ASYNC);
				Sleep(300);
			}
		}

		else if ((lsf < rsf)) {
			if (rsf == 1) {
				PlaySound("right.wav", NULL, SND_ASYNC);
				Sleep(1000);
			}
			if (rsf == 2) {
				PlaySound("right.wav", NULL, SND_ASYNC);
				Sleep(700);
			}
			if (rsf == 3) {
				PlaySound("right.wav", NULL, SND_ASYNC);
				Sleep(300);
			}
		}
	}
	else {
		if (lsr == rsr) {
			if (lsr == 1) {
				PlaySound("both_reverse.wav", NULL, SND_ASYNC);
				Sleep(1000);
			}
			if (lsr == 2) {
				PlaySound("both_reverse.wav", NULL, SND_ASYNC);
				Sleep(700);
			}
			if (lsr == 3) {
				PlaySound("both_reverse.wav", NULL, SND_ASYNC);
				Sleep(300);
			}
		}
		else if ((lsr > rsr)) {
			if (lsr == 1) {
				PlaySound("left_reverse.wav", NULL, SND_ASYNC);
				Sleep(1000);
			}
			if (lsr == 2) {
				PlaySound("left_reverse.wav", NULL, SND_ASYNC);
				Sleep(700);
			}
			if (lsr == 3) {
				PlaySound("left_reverse.wav", NULL, SND_ASYNC);
				Sleep(300);
			}
		}
		else if ((lsr < rsr)) {
			if (rsr == 1) {
				PlaySound("right_reverse.wav", NULL, SND_ASYNC);
				Sleep(1000);
			}
			if (rsr == 2) {
				PlaySound("right_reverse.wav", NULL, SND_ASYNC);
				Sleep(700);
			}
			if (rsr == 3) {
				PlaySound("right_reverse.wav", NULL, SND_ASYNC);
				Sleep(300);
			}
		}
	}
}


void display_sensors_new(unsigned char key, int x, int y) {

	switch (key) {
	case '8':
		gear = 1;
		break;
	case '2':
		gear = 0;
		break;
	}

	switch (key) {
	case 'y':
		lsf = 1;
		polje[0] = 1;
		display();
		break;
	case 'x':
		lsf = 2;
		polje[0] = 2;
		display();
		break;
	case 'c':
		lsf = 3;
		polje[0] = 3;
		display();
		break;

	case 'q':
		rsf = 1;
		polje[1] = 1;
		display();
		break;
	case 'w':
		rsf = 2;
		polje[1] = 2;
		display();
		break;
	case 'e':
		rsf = 3;
		polje[1] = 3;
		display();
		break;

	case 't':
		rsr = 1;
		polje[2] = 1;
		display();
		break;
	case 'z':
		rsr = 2;
		polje[2] = 2;
		display();
		break;
	case 'u':
		rsr = 3;
		polje[2] = 3;
		display();
		break;

	case 'v':
		lsr = 1;
		polje[3] = 1;
		display();
		break;
	case 'b':
		lsr = 2;
		polje[3] = 2;
		display();
		break;
	case 'n':
		lsr = 3;
		polje[3] = 3;
		display();
		break;

	case 'r':
		for (int i = 0; i < 4; i++)
		{
			polje[i] = 0;
		}
		lsf = rsf = lsr = rsr = 0;
		display();
		break;

	default:
		break;
	}
}