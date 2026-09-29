#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
#include <cstdio>
using namespace std;

int elapsedSeconds = 0;
bool isRunning = false;

void drawBitmapString(void* font, const char* str) {
    for (const char* c = str; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.35f, 0.0f);

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Elapsed Time: %d s", elapsedSeconds);
    drawBitmapString(GLUT_BITMAP_HELVETICA_18, buffer);

    glFlush();
}

void timer(int value) {
    if (isRunning) {
        elapsedSeconds++;
        glutPostRedisplay();
    }
    glutTimerFunc(1000, timer, 0);
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            isRunning = true;
        } else if (button == GLUT_RIGHT_BUTTON) {
            isRunning = false;
        }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Q17 - Mouse-Controlled Stopwatch");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutTimerFunc(1000, timer, 0);
    glutMainLoop();
    return 0;
}