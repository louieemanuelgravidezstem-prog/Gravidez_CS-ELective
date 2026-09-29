#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
using namespace std;

float px = 0.0f;
float halfSize = 0.1f;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.2f, 0.6f, 1.0f);
    glBegin(GL_POLYGON);
        glVertex2f(px - halfSize, -0.1f);
        glVertex2f(px + halfSize, -0.1f);
        glVertex2f(px + halfSize,  0.1f);
        glVertex2f(px - halfSize,  0.1f);
    glEnd();

    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'a' || key == 'A') {
        px -= 0.05f;
    } else if (key == 'd' || key == 'D') {
        px += 0.05f;
    }

    if (px - halfSize < -0.9f) px = -0.9f + halfSize;
    if (px + halfSize > 0.9f)  px = 0.9f - halfSize;

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Q08 - Clamped Keyboard Movement");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}