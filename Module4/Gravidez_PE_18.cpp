#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream>

int currentShape = 1;

GLfloat triangleVertices[] = {
     0.0f,  0.5f,
    -0.5f, -0.5f,
     0.5f, -0.5f
};

GLfloat quadVertices[] = {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f,
    -0.5f,  0.5f
};

GLfloat pentagonVertices[] = {
     0.0f,   0.5f,
     0.48f,  0.15f,
     0.29f, -0.4f,
    -0.29f, -0.4f,
    -0.48f,  0.15f
};

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);

    if (currentShape == 1) {
        glVertexPointer(2, GL_FLOAT, 0, triangleVertices);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    } else if (currentShape == 2) {
        glVertexPointer(2, GL_FLOAT, 0, quadVertices);
        glDrawArrays(GL_QUADS, 0, 4);
    } else if (currentShape == 3) {
        glVertexPointer(2, GL_FLOAT, 0, pentagonVertices);
        glDrawArrays(GL_POLYGON, 0, 5);
    }

    glDisableClientState(GL_VERTEX_ARRAY);

    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == '1') {
        currentShape = 1;
        glutPostRedisplay();
    } else if (key == '2') {
        currentShape = 2;
        glutPostRedisplay();
    } else if (key == '3') {
        currentShape = 3;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Exercise Q18 - Keyboard-Switched Vertex Arrays");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}