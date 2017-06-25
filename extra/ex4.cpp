#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define PI acos(-1.0)

class Point3d{
public:
	float x;
	float y;
	float z;

	Point3d();
	Point3d(float x, float y, float z);
};

Point3d::Point3d(){
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
}

Point3d::Point3d(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

Point3d calculateNormal(Point3d a, Point3d b, Point3d c){
	Point3d u,v,n;
	u.x = b.x - a.x;
	u.y = b.y - a.y;
	u.z = b.z - a.z;
	v.x = c.x - a.x;
	v.y = c.y - a.y;
	v.z = c.z - a.z;

	n.x = u.y*v.z - u.z*v.y;
	n.y = u.z*v.x - u.x*v.z;
	n.z = u.x*v.y - u.y*v.x;
	return n;
}

Point3d a(0.0, 0.0, 0.0),
		b(0.0, 1.0, 0.0),
		c(1.0, 1.0, 0.0),
		d(1.0, 0.0, 0.0),
		e(0.0, 0.0, 1.0),
		f(0.0, 1.0, 1.0),
		g(1.0, 1.0, 1.0),
		h(1.0, 0.0, 1.0);

int winWidth = 800;
int winHeight = 600;
int filling = 0;
int projection = 0;
int shade = 0;
int light = 0;
int colorMaterial = 0;

int oldX = 0, oldY = 0;
double dx, dy;
int pressed = 0;
float rotation_x = 0;
float rotation_y = 0;
float rotation_z = 0;

float scale = 1.0;

float rotation_x_increment = 0;
float rotation_y_increment = 0;
float rotation_z_increment = 0;


void setProjection(int projection){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(projection){
		gluPerspective(45.0f,(GLfloat)winWidth/(GLfloat)winHeight,1.0f,100.0f);
	}else{
		glOrtho(-5,5,-5,5,-5,100);
	}
}

void setColorMaterial(int colorMaterial){
	if(colorMaterial){
		glEnable(GL_COLOR_MATERIAL);
	}else{
		glDisable(GL_COLOR_MATERIAL);
	}
}

void setShade(int shade){
	if(shade){
		glShadeModel(GL_SMOOTH);
	} else {
		glShadeModel(GL_FLAT);
	}
}

void initLights(){
	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 10.0, 10.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHT0);
}

void init(){
	glClearColor(0.0, 0.0, 0.2, 0.0);

	glViewport(0,0,winWidth, winHeight);
	setProjection(projection);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45.0f,(GLfloat)winWidth/(GLfloat)winHeight,1.0f,100.0f);
	setShade(shade);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode (GL_FRONT, GL_FILL);
	setColorMaterial(colorMaterial);
	initLights();
}

void resize(int width, int height){
	winWidth = width;
	winHeight = height;
	glViewport(0.0,0.0,winWidth,winHeight);

	setProjection(projection);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45.0f,(GLfloat)winWidth/(GLfloat)winHeight,1.0f,100.0f);

}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0,0.0,-10);
	rotation_x += rotation_x_increment;
	rotation_y += rotation_y_increment;
	rotation_z += rotation_z_increment;
	if (rotation_x > 359) rotation_x -= 360;
	else if(rotation_x < 0) rotation_x += 360;
	if (rotation_y > 359) rotation_y -= 360;
	else if(rotation_y < 0) rotation_y += 360;
	if (rotation_z > 359) rotation_z -= 360;
	else if(rotation_z < 0) rotation_z += 360;



	glRotatef(rotation_x,1.0,0.0,0.0);
	glRotatef(rotation_y,0.0,1.0,0.0);
	glRotatef(rotation_z,0.0,0.0,1.0);
	glColor3f(1.0,1.0,0.0);
	//printf("%d\n",fracDegree);
	glRotatef(90,1.0,0.0,0.0);

	glScalef(scale, scale, scale);
	//sierpinski(fracDegree, 40.0, -20.0, -20.0, 0.0);
	//sierpinski3D(fracDegree, 10.0, 0.0, 0.0, 0.0);
	//drawEqTetrahedron(10.0, 0.0, 0.0, 0.0);
	glTranslatef(-0.5, -0.5, -0.5);
	Point3d n;
	glBegin(GL_QUADS);
		glColor3f(1.0, 0.0, 0.0);
		n = calculateNormal(a,b,c);
		glNormal3f(n.x, n.y, n.z);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(b.x, b.y, b.z);
		glVertex3f(c.x, c.y, c.z);
		glVertex3f(d.x, d.y, d.z);

		glColor3f(0.0, 1.0, 0.0);
		n = calculateNormal(a,d,h);
		glNormal3f(n.x, n.y, n.z);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(d.x, d.y, d.z);
		glVertex3f(h.x, h.y, h.z);
		glVertex3f(e.x, e.y, e.z);

		glColor3f(0.0, 0.0, 1.0);
		n = calculateNormal(a,e,f);
		glNormal3f(n.x, n.y, n.z);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(e.x, e.y, e.z);
		glVertex3f(f.x, f.y, f.z);
		glVertex3f(b.x, b.y, b.z);

		glColor3f(1.0, 1.0, 0.0);
		n = calculateNormal(c,g,h);
		glNormal3f(n.x, n.y, n.z);
		glVertex3f(c.x, c.y, c.z);
		glVertex3f(g.x, g.y, g.z);
		glVertex3f(h.x, h.y, h.z);
		glVertex3f(d.x, d.y, d.z);

		glColor3f(1.0, 0.0, 1.0);
		n = calculateNormal(c,b,f);
		glNormal3f(n.x, n.y, n.z);
		glVertex3f(c.x, c.y, c.z);
		glVertex3f(b.x, b.y, b.z);
		glVertex3f(f.x, f.y, f.z);
		glVertex3f(g.x, g.y, g.z);

		glColor3f(0.0, 1.0, 1.0);
		n = calculateNormal(e,h,g);
		glNormal3f(n.x, n.y, n.z);
		glVertex3f(e.x, e.y, e.z);
		glVertex3f(h.x, h.y, h.z);
		glVertex3f(g.x, g.y, g.z);
		glVertex3f(f.x, f.y, f.z);
	glEnd();

	glutSwapBuffers();
}


void keyPressEvent (unsigned char key, int x, int y)
{
	switch (key){
		case 'd': case 'D':
			rotation_x=0;
			rotation_y=0;
			rotation_z=0;
			scale = 1.0;
			break;

		case 'r': case 'R':
			if (filling==0)
			{
				glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
				filling=1;
			}
			else
			{
				glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
				filling=0;
			}
			break;

		case 'p': case 'P':
			projection++;
			projection%=2;
			setProjection(projection);
			break;

		case 's': case 'S':
			shade++;
			shade%=2;
			setShade(shade);
			break;

		case 'l': case 'L':
			light++;
			light%=2;
			if(light){
				glEnable(GL_LIGHTING);
			}else{
				glDisable(GL_LIGHTING);
			}
			break;

		case 'c': case 'C':
			colorMaterial++;
			colorMaterial%=2;
			setColorMaterial(colorMaterial);
			break;


		case 27:
			exit(0);
	}
	glutPostRedisplay();
}

void specialKeyPressEvent (int key, int x, int y){
	switch (key){
		case GLUT_KEY_UP:
			rotation_x += 10;
			break;
		case GLUT_KEY_DOWN:
			rotation_x += -10;
			break;
		case GLUT_KEY_LEFT:
			rotation_y += 10;
			break;
		case GLUT_KEY_RIGHT:
			rotation_y += -10;
			break;
		case GLUT_KEY_PAGE_DOWN:
			scale *= 0.9;
			break;
		case GLUT_KEY_PAGE_UP:
			scale *=1.1;
			break;
	}
	glutPostRedisplay();
}


void mouseMovementEvent(int x, int y){
	if(pressed){
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
	if(button == GLUT_LEFT_BUTTON){
		if(state == GLUT_DOWN){
			oldX = x;
			oldY = y;
			pressed = 1;
		}else{
			pressed = 0;
		}
	}
}

int main(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Exemplo 3");

	glutDisplayFunc(display);
	glutReshapeFunc (resize);
	glutKeyboardFunc (keyPressEvent);
	glutSpecialFunc (specialKeyPressEvent);
	glutMouseFunc(mousePressEvent);
	glutMotionFunc(mouseMovementEvent);

	init();
	glutMainLoop();

	return 0;
}
