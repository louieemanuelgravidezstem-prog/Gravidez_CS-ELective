#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream>
#include <cmath>

const int TEETH = 12;
const float M_PI_F = 3.14159265358979323846f;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    const int totalPoints = TEETH * 2;
    GLfloat vertices[(totalPoints + 1) * 2];

    vertices[0] = 0.0f;
    vertices[1] = 0.0f;

    for (int i = 0; i < totalPoints; ++i) {
        float angle = 2.0f * M_PI_F * i / totalPoints;
        float radius = (i % 2 == 0) ? 0.7f : 0.4f;
        int idx = (i + 1) * 2;
        vertices[idx]     = radius * cosf(angle);
        vertices[idx + 1] = radius * sinf(angle);
    }

    GLubyte evenIndices[TEETH * 3];
    GLubyte oddIndices[TEETH * 3];

    for (int i = 0; i < totalPoints; ++i) {
        GLubyte p1 = static_cast<GLubyte>(i + 1);
        GLubyte p2 = static_cast<GLubyte>((i + 1) % totalPoints + 1);

        if (i % 2 == 0) {
            int idx = (i / 2) * 3;
            evenIndices[idx]     = 0;
            evenIndices[idx + 1] = p1;
            evenIndices[idx + 2] = p2;
        } else {
            int idx = (i / 2) * 3;
            oddIndices[idx]     = 0;
            oddIndices[idx + 1] = p1;
            oddIndices[idx + 2] = p2;
        }
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices);

    glColor3f(0.9f, 0.3f, 0.2f);
    glDrawElements(GL_TRIANGLES, TEETH * 3, GL_UNSIGNED_BYTE, evenIndices);

    glColor3f(0.2f, 0.6f, 0.9f);
    glDrawElements(GL_TRIANGLES, TEETH * 3, GL_UNSIGNED_BYTE, oddIndices);

    glDisableClientState(GL_VERTEX_ARRAY);

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Exercise Q17 - Procedural Gear Shape");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}