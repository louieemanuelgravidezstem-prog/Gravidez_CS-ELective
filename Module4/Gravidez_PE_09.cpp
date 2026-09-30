#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    GLfloat vertices[] = {
         0.0f,   0.0f,
         0.0f,   0.5f,
         0.48f,  0.15f,
         0.29f, -0.4f,
        -0.29f, -0.4f,
        -0.48f,  0.15f
    };

    GLubyte indices[] = { 0, 1, 2, 3, 4, 5, 1 };

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices);

    glDrawElements(GL_TRIANGLE_FAN, 7, GL_UNSIGNED_BYTE, indices);

    glDisableClientState(GL_VERTEX_ARRAY);

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Exercise Q09 - Indexed Triangle Fan Pentagon");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}