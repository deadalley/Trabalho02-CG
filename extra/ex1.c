#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glew.h>
#include <GL/glut.h>

void init(){
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}


void display(){
    glClear(GL_COLOR_BUFFER_BIT);



    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0.25, 0.25, 1.0);
        glVertex3f(0.75, 0.25, 1.0);
        glVertex3f(0.75, 0.75, 1.0);

        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(-0.25, -0.25, 1.0);
        glVertex3f(-0.75, -0.25, 1.0);
        glVertex3f(-0.75, -0.75, 1.0);

        glColor3f(0.0, 1.0, 1.0);
        glVertex3f(-0.25, 0.25, 1.0);
        glColor3f(1.0, 0.0, 1.0);
        glVertex3f(-0.75, 0.25, 1.0);
        glColor3f(1.0, 1.0, 0.0);
        glVertex3f(-0.75, 0.75, 1.0);
    glEnd();

    glFlush();
}


int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(250, 250);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello World!");
    init();
    gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -2.0f);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
