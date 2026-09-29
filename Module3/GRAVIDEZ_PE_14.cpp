#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
using namespace std;

float posY = 0.0f;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.2f, 0.8f, 0.4f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.1f, posY - 0.1f);
        glVertex2f( 0.1f, posY - 0.1f);
        glVertex2f( 0.1f, posY + 0.1f);
        glVertex2f(-0.1f, posY + 0.1f);
    glEnd();

    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') {
        posY += 0.05f;
    } else if (key == 's' || key == 'S') {
        posY -= 0.05f;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        posY = 0.0f;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Q14 - Keyboard + Mouse Combo");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}