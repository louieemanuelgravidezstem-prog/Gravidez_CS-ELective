#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream>
#include <cmath>

const int SEGMENTS = 32;
const float M_PI_F = 3.14159265358979323846f;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    const int vertexCount = SEGMENTS + 2;
    GLfloat vertices[vertexCount * 2];
    GLfloat colors[vertexCount * 3];

    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    colors[0] = 1.0f;
    colors[1] = 1.0f;
    colors[2] = 1.0f;

    for (int i = 0; i <= SEGMENTS; ++i) {
        float angle = 2.0f * M_PI_F * i / SEGMENTS;
        int vIdx = (i + 1) * 2;
        int cIdx = (i + 1) * 3;

        vertices[vIdx]     = 0.5f * cosf(angle);
        vertices[vIdx + 1] = 0.5f * sinf(angle);

        colors[cIdx]     = 0.5f + 0.5f * cosf(angle);
        colors[cIdx + 1] = 0.5f + 0.5f * sinf(angle);
        colors[cIdx + 2] = (float)i / SEGMENTS;
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);

    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Exercise Q11 - Procedural Shaded Circle");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}