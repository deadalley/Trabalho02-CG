#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <time.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glsl.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Point3D.h"
#include "Object.h"

using namespace std;
using namespace cwc;

glShaderManager SM;
glShader *shader;
GLuint ProgramObject;

int winWidth = 800;
int winHeight = 600;

int oldX = 0, oldY = 0;
double dx, dy;
bool pressed = false;
bool input_coordinate = false, rotate_object = false, remove_object = false, move_object = false, light_source = false, object_label = false, show_axes = true;

GLfloat light_pos[] = {40.0f, 40.0f, 40.0f, 1.0f};

float new_scale;
ObjectType new_type;
int selected_obj = 0;

string menu = "C - Add cube\nS - Add sphere\nT - Add teapot\nM - Move object\nR - Rotate object\nX - Remove object\nL - Move light source\nH - Show/Hide axes\n\nF (Flat)\nG (Gouraud)\nP (Phong)";
string label, x_label, y_label, z_label;
int xyz_label = 0;

vector<Object> objects;

void rotateObject(Point3D rotation) {
	// Handling mouse rotation
	if (rotation.x > 359)
		rotation.x -= 360;
	else if(rotation.x < 0)
		rotation.x += 360;

	if (rotation.y > 359)
		rotation.y -= 360;
	else if(rotation.y < 0)
		rotation.y += 360;

	if (rotation.z > 359)
		rotation.z -= 360;
	else if(rotation.z < 0)
		rotation.z += 360;

	glRotatef(rotation.x, 1.0, 0.0, 0.0);
	glRotatef(rotation.y, 0.0, 1.0, 0.0);
	glRotatef(rotation.z, 0.0, 0.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
}

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

void labelToObject() {
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

void configLight() {
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_NORMALIZE);

   // Light model parameters:
   GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);

   glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
   glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

   // Spotlight Attenuation
   GLfloat spot_direction[] = {1.0, -1.0, -1.0 };
   GLint spot_exponent = 30;
   GLint spot_cutoff = 180;

   glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
   glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
   glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);

   GLfloat Kc = 1.0;
   GLfloat Kl = 0.0;
   GLfloat Kq = 0.0;

   glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
   glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
   glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);

   // Lighting parameters:
   GLfloat light_Ka[]  = {1.0f, 0.5f, 0.5f, 1.0f};
   GLfloat light_Kd[]  = {1.0f, 0.1f, 0.1f, 1.0f};
   GLfloat light_Ks[]  = {1.0f, 1.0f, 1.0f, 1.0f};

   glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

   // Material parameters:
   GLfloat material_Ka[] = {0.5f, 0.0f, 0.0f, 1.0f};
   GLfloat material_Kd[] = {0.4f, 0.4f, 0.5f, 1.0f};
   GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
   GLfloat material_Ke[] = {0.1f, 0.0f, 0.0f, 0.0f};
   GLfloat material_Se = 20.0f;

   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
 }

// Initialize OpenGL Graphics
void initGL() {
	glClearColor(0.0, 0.0, 0.0, 0.0);

  glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);

	shader = SM.loadfromFile("shaders/phong_vertexshader.txt","shaders/phong_fragmentshader.txt"); // load (and compile, link) from file
	if(shader == 0) {
		cout << "Error loading, compiling or linking INITIAL shader" << endl;
	}
  else {
		ProgramObject = shader->GetProgramObject();
  }
  shader->begin();
  configLight();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45.0, (GLfloat)winWidth/(GLfloat)winHeight, 1.0, 100.0);
	gluPerspective(45.0, (GLfloat)winWidth/(GLfloat)winHeight, 0.1, 500.0);

	glEnable(GL_DEPTH_TEST);

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

	glClear(GL_COLOR_BUFFER_BIT);
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

	if (show_axes)
		showAxes();

	configLight();

  if (shader)
		shader->begin();

	vector<Object>::iterator it;
	for (it = objects.begin(); it < objects.end(); it++) {
		// glColor3f(it->label->color.x, it->label->color.y, it->label->color.z);
		// drawBitmapText(it->label->str, it->label->pos);

		if (it->isSelected())
			glColor3f(0.0, 1.0, 0.0);
		else
			glColor3f(1.0, 1.0, 0.0);

		glPushMatrix();
		glTranslatef(it->position.x, it->position.y, it->position.z);
		rotateObject(it->rotation);

		if (it->type == Cube)
			glutSolidCube(it->scale);
		if (it->type == Teapot)
			glutSolidTeapot(it->scale);
		if (it->type == Sphere)
			glutSolidSphere(it->scale, 50.0, 50.0);

		glPopMatrix();
	}

	if (shader)
		shader->end();

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
		if (rotate_object) {
			objects.at(selected_obj).rotation.x += dy;
			objects.at(selected_obj).rotation.y += dx;
		}

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
				label = menu;
				input_coordinate = false;
				object_label = false;
				light_source = false;
				move_object = false;
				break;
			case 13: // Enter
				xyz_label++;

				if (xyz_label > 2) {
					xyz_label = 0;
					input_coordinate = false;
					label = menu;
					if (object_label) {
						labelToObject();
						object_label = false;
					}

					if (light_source) {
						light_pos[0] = stof(x_label.substr(3, x_label.length()));
						light_pos[1] = stof(y_label.substr(3, y_label.length()));
						light_pos[2] = stof(z_label.substr(3, z_label.length()));
						light_source = false;
					}

					if (move_object) {
						Point3D pos;

						pos.x = stof(x_label.substr(3, x_label.length()));
						pos.y = stof(y_label.substr(3, y_label.length()));
						pos.z = stof(z_label.substr(3, z_label.length()));

						objects.at(selected_obj).setPosition(pos);
						move_object = false;
					}

					x_label = "X: ";
					y_label = "Y: ";
					z_label = "Z: ";
				}
				break;
			case 8: // Backspace
				local_label->pop_back();
				break;
			case 32: // Space
				if (move_object) {
					objects.at(selected_obj).select(false);
					selected_obj++;
					if (selected_obj >= objects.size())
						selected_obj = 0;
					objects.at(selected_obj).select(true);
				}
				break;
			case 27: // Escape
				exit(0);
		}
	}

	else if (rotate_object) {
		switch (key) {
			case 'q': case 'Q':
				label = menu;
				rotate_object = false;
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

	else if (remove_object) {
		switch (key) {
			case 'q': case 'Q':
				label = menu;
				remove_object = false;
				if (objects.size() > 0)
					objects.at(selected_obj).select(false);
				break;
			case 32: // Space
				objects.at(selected_obj).select(false);
				selected_obj++;
				if (selected_obj >= objects.size())
					selected_obj = 0;
				objects.at(selected_obj).select(true);
				break;
			case 13: // Enter
				objects.erase(objects.begin() + selected_obj);
				if (selected_obj >= objects.size())
					selected_obj = 0;
				if (objects.size() > 0)
					objects.at(selected_obj).select(true);
				break;
		}
	}

	else if (move_object) {
		switch (key) {
			case 'q': case 'Q':
				label = menu;
				move_object = false;
				objects.at(selected_obj).select(false);
				break;
			case 32: // Space
				objects.at(selected_obj).select(false);
				selected_obj++;
				if (selected_obj >= objects.size())
					selected_obj = 0;
				objects.at(selected_obj).select(true);
				break;
			case 13: // Enter
				label = "Moving object (Q to quit)";
				input_coordinate = true;
				break;
		}
	}

	else {
		switch (key) {
			case 'c': case 'C':
				label = "Adding a cube (Q to quit)";
				new_type = Cube;
				new_scale = 20.0;
				input_coordinate = true;
				object_label = true;
				break;
			case 's': case 'S':
				label = "Adding a sphere (Q to quit)";
				new_type = Sphere;
				new_scale = 14.0;
				input_coordinate = true;
				object_label = true;
				break;
			case 't': case 'T':
				label = "Adding a teapot (Q to quit)";
				new_type = Teapot;
				new_scale = 24.0;
				input_coordinate = true;
				object_label = true;
				break;
			case 'r': case 'R':
				if (objects.size() == 0)
					break;
				label = "Rotating object (Q to quit)\nSPACE - Choose object";
				rotate_object = true;
				objects.at(selected_obj).select(true);
				break;
			case 'm': case 'M':
				if (objects.size() == 0)
					break;
				label = "Moving object (Q to quit)\nSPACE - Choose object";
				move_object = true;
				objects.at(selected_obj).select(true);
				break;
			case 'x': case 'X':
				if (objects.size() == 0)
					break;
				label = "Removing object (Q to quit)\nSPACE - Choose object\nENTER - Remove object";
				remove_object = true;
				objects.at(selected_obj).select(true);
				break;
			case 'l': case 'L':
				label = "Moving light source (Q to quit)";
				input_coordinate = true;
				light_source = true;
				break;
			case 'f': case 'F':
	  		shader->disable();
	  		glShadeModel(GL_FLAT);
	  		glEnable(GL_DEPTH_TEST);
	  		cout << "Shader changed to FLAT" << endl;
	  		break;
      case 'g': case 'G':
        shader->disable();
        glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
        cout << "Shader changed to GOURAUD" << endl;
        break;
      case 'p': case 'P':
        shader = SM.loadfromFile("shaders/phong_vertexshader.txt","shaders/phong_fragmentshader.txt"); // load (and compile, link) from file
        if (shader == 0){
           cout << "Error Loading, compiling or linking PHONG shader\n";
           shader->disable();
        }
        else {
           ProgramObject = shader->GetProgramObject();
        }
        shader->enable();
        cout << "Shader changed to PHONG" << endl;
        break;
			case 'h': case 'H':
				show_axes = !show_axes;
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
	glutCreateWindow("Trabalho 2 CG"); 	// Create window with the given title

	glutDisplayFunc(display);       		// Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       		// Register callback handler for window re-size event

	glutMouseFunc(mousePressEvent);			// Mouse press events handler
	glutMotionFunc(mouseMovementEvent);	// Mouse move events handler

	glutKeyboardFunc(keyPressEvent);		// Keyboard press events handler


	initGL();                       		// Our own OpenGL initialization
	glutMainLoop();                 		// Enter the infinite event-processing loop

	return 0;
}
