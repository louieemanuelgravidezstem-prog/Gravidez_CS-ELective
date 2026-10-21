#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdlib>

GLuint squareVBO;
GLuint triangleVBO;

void initializeShapes() {
    GLfloat squareVertices[] = {
        -0.9f, -0.5f, 0.0f,
        -0.1f, -0.5f, 0.0f,
        -0.1f,  0.5f, 0.0f,
        -0.9f,  0.5f, 0.0f
    };

    GLfloat triangleVertices[] = {
         0.1f, -0.5f, 0.0f,
         0.9f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f
    };

    glGenBuffers(1, &squareVBO);
    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glColor3f(0.2f, 0.6f, 1.0f);
    glDrawArrays(GL_QUADS, 0, 4);

    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glColor3f(1.0f, 0.4f, 0.2f);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        GLuint buffers[] = {squareVBO, triangleVBO};
        glDeleteBuffers(2, buffers);

        std::exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q13 - ESC Cleanup for Two VBOs");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

#ifndef __APPLE__
    if (glewInit() != GLEW_OK) {
        return 1;
    }
#endif

    initializeShapes();
    glutMainLoop();

    return 0;
}