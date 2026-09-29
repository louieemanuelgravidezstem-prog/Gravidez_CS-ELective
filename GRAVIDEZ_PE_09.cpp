#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
#include <cstdio>
using namespace std;

float glX = 0.0f;
float glY = 0.0f;

void drawBitmapString(void* font, const char* str) {
    for (const char* c = str; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.5f, 0.0f);

    char buffer[64];
    sprintf(buffer, "OpenGL Pos: (%.2f, %.2f)", glX, glY);
    drawBitmapString(GLUT_BITMAP_HELVETICA_18, buffer);

    glFlush();
}

void motion(int x, int y) {
    int winWidth = glutGet(GLUT_WINDOW_WIDTH);
    int winHeight = glutGet(GLUT_WINDOW_HEIGHT);

    glX = (2.0f * x / winWidth) - 1.0f;
    glY = 1.0f - (2.0f * y / winHeight);

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Q09 - Active Motion Coordinate Display");
    glutDisplayFunc(display);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
}