#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdlib>

GLuint hexagonVBO;

void initializeHexagon() {
    GLfloat vertices[] = {
         0.0f,  0.7f, 0.0f,
        -0.6f,  0.35f, 0.0f,
        -0.6f, -0.35f, 0.0f,
         0.0f, -0.7f, 0.0f,
         0.6f, -0.35f, 0.0f,
         0.6f,  0.35f, 0.0f
    };

    glGenBuffers(1, &hexagonVBO);
    glBindBuffer(GL_ARRAY_BUFFER, hexagonVBO);

    // GL_STATIC_DRAW is suitable because the vertices do not change.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.5f, 0.0f);

    glBindBuffer(GL_ARRAY_BUFFER, hexagonVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);

    glDrawArrays(GL_POLYGON, 0, 6);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        glDeleteBuffers(1, &hexagonVBO);
        std::exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q03 - Static Hexagon");

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