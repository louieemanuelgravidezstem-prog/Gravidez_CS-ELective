#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cmath>

const int TRIANGLE_COUNT = 50;
GLuint VBOid;

void createTriangles() {
    GLfloat vertices[TRIANGLE_COUNT * 9];

    for (int i = 0; i < TRIANGLE_COUNT; i++) {
        GLfloat t = static_cast<GLfloat>(i) / TRIANGLE_COUNT;
        GLfloat centerX = -0.9f + t * 1.8f;
        GLfloat centerY = 0.3f * sinf(t * 6.28f * 2.0f);
        int start = i * 9;

        vertices[start]     = centerX;
        vertices[start + 1] = centerY + 0.03f;
        vertices[start + 2] = 0.0f;

        vertices[start + 3] = centerX - 0.02f;
        vertices[start + 4] = centerY - 0.02f;
        vertices[start + 5] = 0.0f;

        vertices[start + 6] = centerX + 0.02f;
        vertices[start + 7] = centerY - 0.02f;
        vertices[start + 8] = 0.0f;
    }

    glGenBuffers(1, &VBOid);
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    createTriangles();

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glColor3f(0.2f, 0.8f, 0.9f);
    glDrawArrays(GL_TRIANGLES, 0, TRIANGLE_COUNT * 3);
    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBOid);
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("EX19 - 50 Triangles in One VBO");

    glutDisplayFunc(display);

#ifndef __APPLE__
    if (glewInit() != GLEW_OK) {
        return 1;
    }
#endif

    glutMainLoop();
    return 0;
}