#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
#include <cstdio>
using namespace std;

int pixelX = 0;
int pixelY = 0;

void drawBitmapString(void* font, const char* str) {
    for (const char* c = str; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.4f, 0.0f);

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Mouse at (%d, %d)", pixelX, pixelY);
    drawBitmapString(GLUT_BITMAP_HELVETICA_18, buffer);

    glFlush();
}

void passiveMotion(int x, int y) {
    pixelX = x;
    pixelY = y;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Q10 - Passive Motion Pixel Readout");
    glutDisplayFunc(display);
    glutPassiveMotionFunc(passiveMotion);
    glutMainLoop();
    return 0;
}