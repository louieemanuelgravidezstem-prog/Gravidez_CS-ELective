#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

float squareX = 0.0f;
float squareY = 0.0f;
const float squareSize = 0.1f;
const float moveSpeed = 0.05f;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 1.0f, 1.0f);

    glBegin(GL_POLYGON);
    glVertex2f(squareX - squareSize, squareY - squareSize);
    glVertex2f(squareX + squareSize, squareY - squareSize);
    glVertex2f(squareX + squareSize, squareY + squareSize);
    glVertex2f(squareX - squareSize, squareY + squareSize);
    glEnd();

    glFlush();
}

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) {
        squareX -= moveSpeed;
    }

    if (key == GLUT_KEY_RIGHT) {
        squareX += moveSpeed;
    }

    if (key == GLUT_KEY_UP) {
        squareY += moveSpeed;
    }

    if (key == GLUT_KEY_DOWN) {
        squareY -= moveSpeed;
    }

    if (squareX < -1.0f + squareSize) {
        squareX = -1.0f + squareSize;
    }

    if (squareX > 1.0f - squareSize) {
        squareX = 1.0f - squareSize;
    }

    if (squareY < -1.0f + squareSize) {
        squareY = -1.0f + squareSize;
    }

    if (squareY > 1.0f - squareSize) {
        squareY = 1.0f - squareSize;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q15 - Move a Square");

    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutMainLoop();

    return 0;
}