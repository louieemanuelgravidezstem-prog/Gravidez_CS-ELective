#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdlib>

GLuint leftTriangleVBO;
GLuint rightTriangleVBO;

void initializeTriangles() {
    GLfloat leftVertices[] = {
        -0.8f, -0.5f, 0.0f,
        -0.2f, -0.5f, 0.0f,
        -0.5f,  0.3f, 0.0f
    };

    GLfloat rightVertices[] = {
         0.2f, -0.5f, 0.0f,
         0.8f, -0.5f, 0.0f,
         0.5f,  0.3f, 0.0f
    };

    glGenBuffers(1, &leftTriangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, leftTriangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(leftVertices), leftVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &rightTriangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, rightTriangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rightVertices), rightVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, leftTriangleVBO);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glColor3f(1.0f, 0.2f, 0.2f);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindBuffer(GL_ARRAY_BUFFER, rightTriangleVBO);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glColor3f(0.2f, 0.6f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        GLuint buffers[] = {leftTriangleVBO, rightTriangleVBO};
        glDeleteBuffers(2, buffers);
        std::exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q05 - Two Separate VBOs");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

#ifndef __APPLE__
    if (glewInit() != GLEW_OK) {
        return 1;
    }
#endif

    initializeTriangles();
    glutMainLoop();

    return 0;
}