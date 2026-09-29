#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
using namespace std;

float sqX = 0.0f;
float sqY = 0.0f;
bool isDragging = false;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.9f, 0.3f, 0.2f);
    glBegin(GL_POLYGON);
        glVertex2f(sqX - 0.1f, sqY - 0.1f);
        glVertex2f(sqX + 0.1f, sqY - 0.1f);
        glVertex2f(sqX + 0.1f, sqY + 0.1f);
        glVertex2f(sqX - 0.1f, sqY + 0.1f);
    glEnd();

    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
        } else if (state == GLUT_UP) {
            isDragging = false;
        }
    }
}

void motion(int x, int y) {
    if (isDragging) {
        int winWidth = glutGet(GLUT_WINDOW_WIDTH);
        int winHeight = glutGet(GLUT_WINDOW_HEIGHT);

        sqX = (2.0f * x / winWidth) - 1.0f;
        sqY = 1.0f - (2.0f * y / winHeight);

        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Q16 - Click-and-Drag Square");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
}