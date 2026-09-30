#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream>

void drawGround() {
    GLfloat vertices[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f, -0.3f,
        -1.0f, -0.3f
    };

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glColor3f(0.2f, 0.6f, 0.2f);
    glDrawArrays(GL_QUADS, 0, 4);
}

void drawMountain() {
    GLfloat vertices[] = {
        -0.8f, -0.3f,
         0.2f, -0.3f,
        -0.3f,  0.4f
    };

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glColor3f(0.4f, 0.4f, 0.4f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void drawSun() {
    GLfloat vertices[] = {
        0.6f,  0.6f,
        0.6f,  0.85f,
        0.8f,  0.72f,
        0.8f,  0.48f,
        0.6f,  0.35f,
        0.4f,  0.48f,
        0.4f,  0.72f
    };

    GLubyte indices[] = { 0, 1, 2, 3, 4, 5, 6, 1 };

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glColor3f(1.0f, 0.9f, 0.0f);
    glDrawElements(GL_TRIANGLE_FAN, 8, GL_UNSIGNED_BYTE, indices);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);

    drawGround();
    drawMountain();
    drawSun();

    glDisableClientState(GL_VERTEX_ARRAY);

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Exercise Q15 - Fully Array-Based Scene");

    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}