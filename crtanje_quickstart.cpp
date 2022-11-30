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
int zvuk = 0;
void e_button(unsigned char key, int x, int y);
void delay(int milliseconds);

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
							   // generating a texture to show the image
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

	glutKeyboardFunc(e_button);
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


	// drawing a rectangle
	/*glColor3f(1.0, 0.0, 0.0); //choosing red color
	glBegin(GL_QUADS);
	glVertex3f(-1.20f, -0.20f, 0.0f);
	glVertex3f(-1.00f, -0.20f, 0.0f);
	glVertex3f(-1.00f, 0.30f, 0.0f);
	glVertex3f(-1.20f, 0.30f, 0.0f);
	glEnd();

	glColor3f(0.0, 1.0, 0.0); //choosing green color
	glBegin(GL_TRIANGLES);
	glVertex3f(-0.30f, -0.50f, 0.0f);
	glVertex3f(-0.70f, -0.50f, 0.0f);
	glVertex3f(-0.60f, -0.20f, 0.0f);
	glEnd();

	glColor3f(1.0, 0.5, 0.0); //choosing orange color
	glBegin(GL_TRIANGLES);
	glVertex3f(1.20f, 0.20f, 0.0f);
	glVertex3f(1.00f, 0.20f, 0.0f);
	glVertex3f(1.10f, 0.30f, 0.0f);
	glEnd();

	glColor3f(1.0, 0.5, 0.0); //choosing orange color
	glBegin(GL_QUADS);
	glVertex3f(1.20f, 0.20f, 0.0f);
	glVertex3f(1.00f, 0.20f, 0.0f);
	glVertex3f(1.05f, 0.08f, 0.0f);
	glVertex3f(1.15f, 0.08f, 0.0f);


	// end of drawing
	glEnd();*/

	glBegin(GL_QUADS);
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

	// swap buffers to show new graphics
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
	// here comes the code which will be executed when program state is idle

}

void e_button(unsigned char key, int x, int y) {
	switch (key) {
	case 'q': //sljedeci dio kola izvrsava se kada je pritisnuta tipka 'q'
		x = 1;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBegin(GL_QUADS);
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 1);
		glVertex3f(-2, -1, 0);
		glTexCoord2f(1, 1);
		glVertex3f(2, -1, 0);
		glTexCoord2f(1, 0);
		glVertex3f(2, 1, 0);
		glTexCoord2f(0, 0);
		glVertex3f(-2, 1, 0);
		glEnd();
		
		glBegin(GL_QUADS);
		glColor3f(1.0, 0.5, 0.5);
		glVertex3f(-1.15f, -0.70f, 0.0f);
		glVertex3f(-1.1f, -0.65f, 0.0f);
		glVertex3f(-1.4f, -0.2f, 0.0f);
		glVertex3f(-1.45f, -0.25f, 0.0f);
		glEnd();

		PlaySound("C://Users/Tihomir/Desktop/ParkingSensorsWindows/AppleChargingSound.mp3", NULL, SND_ASYNC);

		glutSwapBuffers();
		break;

	case 'w': //sljedeci dio kola izvrsava se kada je pritisnuta tipka 'w'
		x = 1;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBegin(GL_QUADS);
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 1);
		glVertex3f(-2, -1, 0);
		glTexCoord2f(1, 1);
		glVertex3f(2, -1, 0);
		glTexCoord2f(1, 0);
		glVertex3f(2, 1, 0);
		glTexCoord2f(0, 0);
		glVertex3f(-2, 1, 0);
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

		PlaySound("C://Users/Tihomir/Desktop/ParkingSensorsWindows/AppleChargingSound.mp3", NULL, SND_ASYNC);

		glutSwapBuffers();
		break;
	case 'e': //sljedeci dio kola izvrsava se kada je pritisnuta tipka 'e'
		x = 1;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBegin(GL_QUADS);
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 1);
		glVertex3f(-2, -1, 0);
		glTexCoord2f(1, 1);
		glVertex3f(2, -1, 0);
		glTexCoord2f(1, 0);
		glVertex3f(2, 1, 0);
		glTexCoord2f(0, 0);
		glVertex3f(-2, 1, 0);
		glEnd();

		glBegin(GL_QUADS);
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

		PlaySound("C://Users/Tihomir/Desktop/ParkingSensorsWindows/AppleChargingSound.mp3", NULL, SND_ASYNC);

		glutSwapBuffers();
		break;

	case 'r': //sljedeci dio kola izvrsava se kada je pritisnuta tipka 'r'
		x = 1;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBegin(GL_QUADS);
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 1);
		glVertex3f(-2, -1, 0);
		glTexCoord2f(1, 1);
		glVertex3f(2, -1, 0);
		glTexCoord2f(1, 0);
		glVertex3f(2, 1, 0);
		glTexCoord2f(0, 0);
		glVertex3f(-2, 1, 0);
		glEnd();

		glBegin(GL_QUADS);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(-1.05f, -0.40f, 0.0f);
		glVertex3f(-1.00f, -0.35f, 0.0f);
		glVertex3f(-1.10f, -0.2f, 0.0f);
		glVertex3f(-1.15f, -0.25f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(-1.10f, -0.55f, 0.0f);
		glVertex3f(-1.05f, -0.5f, 0.0f);
		glVertex3f(-1.25f, -0.2f, 0.0f);
		glVertex3f(-1.3f, -0.25f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(-1.15f, -0.70f, 0.0f);
		glVertex3f(-1.1f, -0.65f, 0.0f);
		glVertex3f(-1.4f, -0.2f, 0.0f);
		glVertex3f(-1.45f, -0.25f, 0.0f);
		glEnd();

		glutSwapBuffers();
		break;
	}
}

void delay(int milliseconds)
{
	long pause;
	clock_t now, then;

	pause = milliseconds * (CLOCKS_PER_SEC / 1000);
	now = then = clock();
	while ((now - then) < pause)
		now = clock();
}




