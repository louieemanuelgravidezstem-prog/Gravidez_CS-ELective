#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream>
#include <cmath>

const int PETALS = 8;
const int DISC_SEGMENTS = 20;
const float M_PI_F = 3.14159265358979323846f;

void drawGround() {
    GLfloat vertices[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f, -0.6f,
        -1.0f, -0.6f
    };

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glColor3f(0.2f, 0.6f, 0.2f);
    glDrawArrays(GL_QUADS, 0, 4);
}

void drawPetals() {
    const int vertexCount = PETALS * 3;
    GLfloat vertices[vertexCount * 2];
    GLfloat colors[vertexCount * 3];

    for (int i = 0; i < PETALS; ++i) {
        float angle = 2.0f * M_PI_F * i / PETALS;
        float angleNext = 2.0f * M_PI_F * (i + 0.5f) / PETALS;

        int vIdx = i * 6;
        int cIdx = i * 9;

        vertices[vIdx]     = 0.0f;
        vertices[vIdx + 1] = 0.0f;
        vertices[vIdx + 2] = 0.6f * cosf(angle);
        vertices[vIdx + 3] = 0.6f * sinf(angle);
        vertices[vIdx + 4] = 0.6f * cosf(angleNext);
        vertices[vIdx + 5] = 0.6f * sinf(angleNext);

        for (int k = 0; k < 3; ++k) {
            colors[cIdx + k * 3]     = 0.9f;
            colors[cIdx + k * 3 + 1] = 0.3f;
            colors[cIdx + k * 3 + 2] = 0.6f;
        }
    }

    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    glDisableClientState(GL_COLOR_ARRAY);
}

void drawDisc() {
    const int vertexCount = DISC_SEGMENTS + 2;
    GLfloat vertices[vertexCount * 2];
    GLubyte indices[vertexCount];

    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    indices[0] = 0;

    for (int i = 0; i <= DISC_SEGMENTS; ++i) {
        float angle = 2.0f * M_PI_F * i / DISC_SEGMENTS;
        int vIdx = (i + 1) * 2;
        vertices[vIdx]     = 0.2f * cosf(angle);
        vertices[vIdx + 1] = 0.2f * sinf(angle);
        indices[i + 1] = static_cast<GLubyte>(i + 1);
    }

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glColor3f(1.0f, 0.8f, 0.0f);

    glDrawElements(GL_TRIANGLE_FAN, vertexCount, GL_UNSIGNED_BYTE, indices);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);

    drawGround();
    drawPetals();
    drawDisc();

    glDisableClientState(GL_VERTEX_ARRAY);

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Exercise Q20 - Capstone: Procedural Flower Scene");

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}