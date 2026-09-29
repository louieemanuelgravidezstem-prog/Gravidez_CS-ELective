#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
using namespace std;

float r = 0.0f, g = 0.0f, b = 0.0f;

void display() {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'r':
        case 'R':
            r = 1.0f; g = 0.0f; b = 0.0f;
            break;
        case 'g':
        case 'G':
            r = 0.0f; g = 1.0f; b = 0.0f;
            break;
        case 'b':
        case 'B':
            r = 0.0f; g = 0.0f; b = 1.0f;
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Q04 - Keyboard Background Color Switch");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}