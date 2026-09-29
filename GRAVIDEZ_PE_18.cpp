#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
#include <cmath>
using namespace std;

float angle = 0.0f;
bool inside = false;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    float radius = 0.5f;
    float posX = radius * cos(angle);
    float posY = radius * sin(angle);

    glColor3f(1.0f, 0.6f, 0.2f);
    glBegin(GL_POLYGON);
        glVertex2f(posX - 0.08f, posY - 0.08f);
        glVertex2f(posX + 0.08f, posY - 0.08f);
        glVertex2f(posX + 0.08f, posY + 0.08f);
        glVertex2f(posX - 0.08f, posY + 0.08f);
    glEnd();

    glFlush();
}

void idle() {
    if (inside) {
        angle += 0.02f;
        if (angle > 2.0f * 3.14159f) {
            angle -= 2.0f * 3.14159f;
        }
        glutPostRedisplay();
    }
}

void entry(int state) {
    if (state == GLUT_ENTERED) {
        inside = true;
    } else if (state == GLUT_LEFT) {
        inside = false;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Q18 - Entry + Idle Freeze Combo");
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutEntryFunc(entry);
    glutMainLoop();
    return 0;
}