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
         0.0f,   0.5f,
         0.43f,  0.25f,
         0.43f, -0.25f,
         0.0f,  -0.5f,
        -0.43f, -0.25f,
        -0.43f,  0.25f
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_POLYGON, 0, 6);
    glDisableClientState(GL_VERTEX_ARRAY);

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Exercise Q02 - Filled Hexagon via Vertex Array");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}