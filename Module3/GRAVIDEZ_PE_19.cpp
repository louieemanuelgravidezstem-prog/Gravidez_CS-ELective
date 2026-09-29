#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
#include <cstdio>
using namespace std;

int score = 0;

void drawBitmapString(void* font, const char* str) {
    for (const char* c = str; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    int stage = (score / 5) % 4;
    switch (stage) {
        case 0: glColor3f(0.2f, 0.6f, 1.0f); break;
        case 1: glColor3f(0.2f, 0.8f, 0.4f); break;
        case 2: glColor3f(0.9f, 0.7f, 0.1f); break;
        case 3: glColor3f(0.9f, 0.3f, 0.2f); break;
    }

    glBegin(GL_POLYGON);
        glVertex2f(-0.3f, -0.3f);
        glVertex2f( 0.3f, -0.3f);
        glVertex2f( 0.3f,  0.3f);
        glVertex2f(-0.3f,  0.3f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.2f, 0.45f);

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Score: %d", score);
    drawBitmapString(GLUT_BITMAP_HELVETICA_18, buffer);

    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        score++;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Q19 - HUD Score with Color Milestones");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}