#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
#include <cstdio>
using namespace std;

int timeLeft = 30;

void drawBitmapString(void* font, const char* str) {
    for (const char* c = str; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);
    
    char buffer[64];
    if (timeLeft > 0) {
        snprintf(buffer, sizeof(buffer), "Time Remaining: %d s", timeLeft);
        glRasterPos2f(-0.4f, 0.0f);
    } else {
        snprintf(buffer, sizeof(buffer), "Time's up!");
        glRasterPos2f(-0.2f, 0.0f);
    }

    drawBitmapString(GLUT_BITMAP_HELVETICA_18, buffer);

    glFlush();
}

void timer(int value) {
    if (timeLeft > 0) {
        timeLeft--;
        glutPostRedisplay();
        if (timeLeft > 0) {
            glutTimerFunc(1000, timer, 0);
        }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Q15 - 30-Second Countdown Timer");
    glutDisplayFunc(display);
    glutTimerFunc(1000, timer, 0);
    glutMainLoop();
    return 0;
}