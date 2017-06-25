#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "Point3D.h"
#include "Object.h"

using namespace std;

Point3D a(0.0, 0.0, 0.0),
b(0.0, 1.0, 0.0),
c(1.0, 1.0, 0.0),
d(1.0, 0.0, 0.0),
e(0.0, 0.0, 1.0),
f(0.0, 1.0, 1.0),
g(1.0, 1.0, 1.0),
h(1.0, 0.0, 1.0);

Point3D obj_coordinates;

int winWidth = 800;
int winHeight = 600;

int oldX = 0, oldY = 0;
double dx, dy;
bool pressed = false;
bool input_coordinate = false, transform_object = false;
float rotation_x = 0;
float rotation_y = 0;
float rotation_z = 0;

float new_scale;
ObjectType new_type;
int selected_obj = 0;

string menu = "C - Add cube\nL - Add light source\nT - Add teapot\nM - Move object\nS - Scale object\nR - Rotate object";
string label, x_label, y_label, z_label;
int xyz_label = 0;

vector<Object> objects;

void showAxes() {
	// Red X
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
  	glVertex3f(-6.0, 0.0f, 0.0f);
  	glVertex3f(6.0, 0.0f, 0.0f);

	  // Draw arrow
	  glVertex3f(6.0, 0.0f, 0.0f);
	  glVertex3f(3.0, 1.0f, 0.0f);

	  glVertex3f(6.0, 0.0f, 0.0f);
	  glVertex3f(3.0, -1.0f, 0.0f);
  glEnd();
  glFlush();

  // Green Y
  glColor3f(0.0,1.0,0.0);
  glBegin(GL_LINES);
	  glVertex3f(0.0, -6.0f, 0.0f);
	  glVertex3f(0.0, 6.0f, 0.0f);

	  // Draw arrow
	  glVertex3f(0.0, 6.0f, 0.0f);
	  glVertex3f(1.0, 3.0f, 0.0f);

	  glVertex3f(0.0, 6.0f, 0.0f);
	  glVertex3f(-1.0, 3.0f, 0.0f);
  glEnd();
  glFlush();

  // Blue Z
  glColor3f(0.0,0.0,1.0);
  glBegin(GL_LINES);
	  glVertex3f(0.0, 0.0f ,-6.0f );
	  glVertex3f(0.0, 0.0f ,6.0f );

	  // Draw arrow
	  glVertex3f(0.0, 0.0f ,6.0f );
	  glVertex3f(0.0, 1.0f ,3.0f );

	  glVertex3f(0.0, 0.0f, 6.0f );
	  glVertex3f(0.0, -1.0f, 3.0f );
  glEnd();
  glFlush();
}

void drawBitmapText(string string, Point3D pos) {
	glRasterPos3f(pos.x, pos.y, pos.z);

	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char *)string.c_str());

	/*
	int lineHeight   = glutBitmapHeight(GLUT_BITMAP_TIMES_ROMAN_10);  // number of pixels between "lines"
	int stringLength = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char *)string.c_str());  // returns length of string 1 in pixels

	glRasterPos2i(-stringLength, lineHeight);  // move raster position
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char *)string.c_str());
	*/
}

void drawCube(Point3D pos, float scale, bool colored, bool rotate) {
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	if (rotate) {
		// Handling mouse rotation
		if (rotation_x > 359)
			rotation_x -= 360;
		else if(rotation_x < 0)
			rotation_x += 360;

		if (rotation_y > 359)
			rotation_y -= 360;
		else if(rotation_y < 0)
			rotation_y += 360;

		if (rotation_z > 359)
			rotation_z -= 360;
		else if(rotation_z < 0)
			rotation_z += 360;

		glRotatef(rotation_x, 1.0, 0.0, 0.0);
		glRotatef(rotation_y, 0.0, 1.0, 0.0);
		glRotatef(rotation_z, 0.0, 0.0, 1.0);
		glRotatef(90, 1.0, 0.0, 0.0);
	}

	glScalef(scale, scale, scale);
	glTranslatef(-0.5, -0.5, -0.5);
	glBegin(GL_QUADS);
		// Top face (y = 1.0f)
		// Define vertices in counter-clockwise (CCW) order with normal pointing out
		if (colored)
			glColor3f(1.0, 0.0, 0.0);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(b.x, b.y, b.z);
		glVertex3f(c.x, c.y, c.z);
		glVertex3f(d.x, d.y, d.z);

		if (colored)
			glColor3f(0.0, 1.0, 0.0);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(d.x, d.y, d.z);
		glVertex3f(h.x, h.y, h.z);
		glVertex3f(e.x, e.y, e.z);

		if (colored)
			glColor3f(0.0, 0.0, 1.0);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(e.x, e.y, e.z);
		glVertex3f(f.x, f.y, f.z);
		glVertex3f(b.x, b.y, b.z);

		if (colored)
			glColor3f(1.0, 1.0, 0.0);
		glVertex3f(c.x, c.y, c.z);
		glVertex3f(g.x, g.y, g.z);
		glVertex3f(h.x, h.y, h.z);
		glVertex3f(d.x, d.y, d.z);

		if (colored)
			glColor3f(1.0, 0.0, 1.0);
		glVertex3f(c.x, c.y, c.z);
		glVertex3f(b.x, b.y, b.z);
		glVertex3f(f.x, f.y, f.z);
		glVertex3f(g.x, g.y, g.z);

		if (colored)
			glColor3f(0.0, 1.0, 1.0);
		glVertex3f(e.x, e.y, e.z);
		glVertex3f(h.x, h.y, h.z);
		glVertex3f(g.x, g.y, g.z);
		glVertex3f(f.x, f.y, f.z);
	glEnd();
	glPopMatrix();
}

void drawTeapot(Point3D pos, float scale) {
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(pos.x, pos.y, pos.z);
	glutSolidTeapot(scale);
}

void labelToObject() {
	string *local_label;
	Point3D pos;

	pos.x = stof(x_label.substr(3, x_label.length()));
	pos.y = stof(y_label.substr(3, y_label.length()));
	pos.z = stof(z_label.substr(3, z_label.length()));

	Object obj(pos, new_scale, new_type, "Object " + to_string(objects.size()));
	//if (objects.size() == 0)
		//obj.select(true);

	objects.push_back(obj);
	//cout << "Size: " << objects.size() << endl;
}

// Initialize OpenGL Graphics
void initGL() {
	glClearColor(0.0, 0.0, 0.2, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45.0, (GLfloat)winWidth/(GLfloat)winHeight, 1.0, 100.0);
	gluPerspective(45.0, (GLfloat)winWidth/(GLfloat)winHeight, 0.1, 500.0);

	glEnable(GL_DEPTH_TEST);   						// Enable depth testing for z-culling

	glPolygonMode(GL_FRONT, GL_FILL);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();
	// Especifica a projeção perspectiva
	gluPerspective(45.0, (GLfloat)winWidth/(GLfloat)winHeight, 0.1, 500);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	// Especifica posição do observador e do alvo
	gluLookAt(0, 80, 200, 0, 0, 0, 0, 1, 0);

	Point3D coord(-100.0, 0.0, 0.0);
	glColor3f(1.0f, 1.0f, 1.0f);
	drawBitmapText(label, coord);

	if (input_coordinate) {
		coord.y = -5.0;
		glColor3f(1.0f, 1.0f, 1.0f);
		drawBitmapText(x_label, coord);

		coord.y = -10.0;
		glColor3f(1.0f, 1.0f, 1.0f);
		drawBitmapText(y_label, coord);

		coord.y = -15.0;
		glColor3f(1.0f, 1.0f, 1.0f);
		drawBitmapText(z_label, coord);
	}


	vector<Object>::iterator it;
	for (it = objects.begin(); it < objects.end(); it++) {
		glColor3f(it->label->color.x, it->label->color.y, it->label->color.z);
		drawBitmapText(it->label->str, it->label->pos);

		if (it->type == Cube)
			drawCube(it->position, it->scale, true, transform_object && it->isSelected());
		if (it->type == LightSource)
			drawCube(it->position, it->scale, false, transform_object && it->isSelected());
		if (it->type == Teapot)
			drawTeapot(it->position, it->scale);

	}

	showAxes();
/*
	Point3D random_cube(1.0, 1.0, 1.0);
	drawCube(random_cube, 10.0, true);

	Point3D random_cube2(1.0, 2.0, 1.0);
	drawCube(random_cube2, 1.0, true);

	Point3D light_source(5.0, 5.0, 5.0);
	drawCube(light_source, 0.25, false);

	vector<Object>::iterator it;
	for (it = objects.begin(); it < objects.end(); it++) {
		glColor3f(it->label->color.x, it->label->color.y, it->label->color.z);
		drawBitmapText(it->label->str, it->label->pos);

		if (it->type == Cube)
			drawCube(it->position, it->scale, true);
		if (it->type == LightSource)
			drawCube(it->position, it->scale, false);
		if (it->type == Teapot)
			drawTeapot(it->position, it->scale);
	}
*/
	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
	// Compute aspect ratio of the new window
	if (height == 0)
		height = 1;                // To prevent divide by 0

	winWidth = width;
	winHeight = height;

	// Set the viewport to cover the new window
	glViewport(0, 0, winWidth, winHeight);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
	// Enable perspective projection with fovy, aspect, zNear and zFar
	//gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 0.1, 500.0);
}

void mouseMovementEvent(int x, int y){
	if (pressed) {
		dx = oldX - x;
		dy = oldY - y;

		oldX = x;
		oldY = y;

		rotation_x += dy;
		rotation_y += dx;

		glutPostRedisplay();
	}
}

void mousePressEvent(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			oldX = x;
			oldY = y;
			pressed = true;
		}
		else {
			pressed = false;
		}
	}
}

void keyPressEvent (unsigned char key, int x, int y) {
	if (input_coordinate) {
		string *local_label;
		if (xyz_label == 0) {
			local_label = &x_label;
		}
		if (xyz_label == 1)
			local_label = &y_label;

		if (xyz_label == 2)
			local_label = &z_label;

		switch (key) {
			case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '0':
				local_label->append(to_string(key-48));
				break;
			case '.':
				local_label->append(".");
				break;
			case '-':
				local_label->append("-");
				break;
			case 'q': case 'Q':
				label = "C - Add cube\nL - Add light source";
				input_coordinate = false;
				break;
			case 13: // Enter
				xyz_label++;

				if (xyz_label > 2) {
					xyz_label = 0;
					input_coordinate = false;
					label = menu;
					labelToObject();
					x_label = "X: ";
					y_label = "Y: ";
					z_label = "Z: ";
				}
				break;
			case 8: // Backspace
				//if (coordinate_label)
				local_label->pop_back();
				break;
			case 27: // Escape
				exit(0);
		}
	}

	else if (transform_object) {
		switch (key) {
			case 'q': case 'Q':
				label = menu;
				transform_object = false;
				objects.at(selected_obj).select(false);
				break;
			case 32: // Space
				objects.at(selected_obj).select(false);
				selected_obj++;
				if (selected_obj >= objects.size())
					selected_obj = 0;
				objects.at(selected_obj).select(true);
				break;
			case 27: // Escape
				exit(0);
		}
	}

	else {
		switch (key) {
			case 'c': case 'C':
				label = "Adding a cube (Q to quit)";
				new_type = Cube;
				new_scale = 10.0;
				input_coordinate = true;
				break;
			case 'l': case 'L':
				label = "Adding a light source (Q to quit)";
				new_type = LightSource;
				new_scale = 1.0;
				input_coordinate = true;
				break;
			case 't': case 'T':
				label = "Adding a teapot (Q to quit)";
				new_type = Teapot;
				new_scale = 8.0;
				input_coordinate = true;
				break;
			case 'r': case 'R':
				label = "Rotating object (Q to quit)\nSPACE - Choose object";
				transform_object = true;
				objects.at(0).select(true);
				break;
			case 27:
				exit(0);
		}
	}

	glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	label = menu;

	x_label = "X: ";
	y_label = "Y: ";
	z_label = "Z: ";

	glutInit(&argc, argv);            	// Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH); 	// Enable double buffered mode
	glutInitWindowSize(800, 600);   		// Set the window's initial width & height
	glutInitWindowPosition(100, 100); 	// Position the window's initial top-left corner
	glutCreateWindow("Oh no");         	// Create window with the given title

	glutDisplayFunc(display);       		// Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       		// Register callback handler for window re-size event

	glutMouseFunc(mousePressEvent);			// Mouse press events handler
	glutMotionFunc(mouseMovementEvent);	// Mouse move events handler

	glutKeyboardFunc(keyPressEvent);		// Keyboard press events handler


	initGL();                       		// Our own OpenGL initialization
	glutMainLoop();                 		// Enter the infinite event-processing loop

	return 0;
}
