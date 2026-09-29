#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
#include <cstdio>
#include <cmath>
using namespace std;

float targetX = 0.0f;
float targetY = 0.0f;
float hoverX = 0.0f;
float hoverY = 0.0f;
float pulseAngle = 0.0f;

void toGL(int x, int y, float &ox, float &oy) {
    int winWidth = glutGet(GLUT_WINDOW_WIDTH);
    int winHeight = glutGet(GLUT_WINDOW_HEIGHT);
    ox = (2.0f * x / winWidth) - 1.0f;
    oy = 1.0f - (2.0f * y / winHeight);
}

void drawBitmapString(void* font, const char* str) {
    for (const char* c = str; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    float scale = 0.08f + 0.02f * sin(pulseAngle);

    glColor3f(0.3f, 0.8f, 1.0f);
    glBegin(GL_POLYGON);
        glVertex2f(targetX - scale, targetY - scale);
        glVertex2f(targetX + scale, targetY - scale);
        glVertex2f(targetX + scale, targetY + scale);
        glVertex2f(targetX - scale, targetY + scale);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(targetX - 0.15f, targetY + scale + 0.05f);
    char labelBuffer[64];
    snprintf(labelBuffer, sizeof(labelBuffer), "Target: (%.2f, %.2f)", targetX, targetY);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, labelBuffer);

    glColor3f(0.7f, 0.7f, 0.7f);
    glRasterPos2f(-0.95f, -0.9f);
    char hoverBuffer[64];
    snprintf(hoverBuffer, sizeof(hoverBuffer), "Hover GL Pos: (%.2f, %.2f)", hoverX, hoverY);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, hoverBuffer);

    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        toGL(x, y, targetX, targetY);
        glutPostRedisplay();
    }
}

void passiveMotion(int x, int y) {
    toGL(x, y, hoverX, hoverY);
    glutPostRedisplay();
}

void idle() {
    pulseAngle += 0.03f;
    if (pulseAngle > 2.0f * 3.14159f) {
        pulseAngle -= 2.0f * 3.14159f;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Q20 - Interactive Text Placer");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(passiveMotion);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}