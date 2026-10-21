#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdlib>

GLuint hexagonVBO;
GLuint indexVBO;

void initializeHexagon() {
    GLfloat vertices[] = {
         0.0f,  0.7f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.6f,  0.35f, 0.0f, 1.0f, 0.5f, 0.0f,
         0.6f, -0.35f, 0.0f, 1.0f, 1.0f, 0.0f,
         0.0f, -0.7f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.6f, -0.35f, 0.0f, 0.0f, 0.5f, 1.0f,
        -0.6f,  0.35f, 0.0f, 0.5f, 0.0f, 1.0f
    };

    GLubyte indices[] = {0, 1, 2, 3, 4, 5};

    glGenBuffers(1, &hexagonVBO);
    glBindBuffer(GL_ARRAY_BUFFER, hexagonVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, hexagonVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    GLsizei stride = 6 * sizeof(GLfloat);

    glVertexPointer(3, GL_FLOAT, stride, (void*)0);
    glColorPointer(3, GL_FLOAT, stride, (void*)(3 * sizeof(GLfloat)));

    glDrawElements(GL_POLYGON, 6, GL_UNSIGNED_BYTE, (void*)0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        GLuint buffers[] = {hexagonVBO, indexVBO};
        glDeleteBuffers(2, buffers);

        std::exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q18 - Interleaved Indexed Hexagon");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

#ifndef __APPLE__
    if (glewInit() != GLEW_OK) {
        return 1;
    }
#endif

    initializeHexagon();
    glutMainLoop();

    return 0;
}