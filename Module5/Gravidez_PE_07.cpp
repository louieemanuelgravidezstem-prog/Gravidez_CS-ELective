#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdlib>

GLuint zigzagVBO;

void initializeZigzag() {
    GLfloat vertices[] = {
        -0.8f, -0.4f, 0.0f,
        -0.4f,  0.4f, 0.0f,
         0.0f, -0.4f, 0.0f,
         0.4f,  0.4f, 0.0f,
         0.8f, -0.4f, 0.0f
    };

    glGenBuffers(1, &zigzagVBO);
    glBindBuffer(GL_ARRAY_BUFFER, zigzagVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 0.0f);
    glLineWidth(4.0f);

    glBindBuffer(GL_ARRAY_BUFFER, zigzagVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glDrawArrays(GL_LINE_STRIP, 0, 5);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        glDeleteBuffers(1, &zigzagVBO);
        std::exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q07 - VBO Line-Strip Zigzag");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

#ifndef __APPLE__
    if (glewInit() != GLEW_OK) {
        return 1;
    }
#endif

    initializeZigzag();
    glutMainLoop();

    return 0;
}