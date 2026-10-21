#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdlib>

GLuint triangleVBOs[3];

void initializeTriangles() {
    GLfloat vertices[3][9] = {
        {
            -0.9f, -0.4f, 0.0f,
            -0.3f, -0.4f, 0.0f,
            -0.6f,  0.4f, 0.0f
        },
        {
            -0.3f, -0.4f, 0.0f,
             0.3f, -0.4f, 0.0f,
             0.0f,  0.4f, 0.0f
        },
        {
             0.3f, -0.4f, 0.0f,
             0.9f, -0.4f, 0.0f,
             0.6f,  0.4f, 0.0f
        }
    };

    GLenum usageHints[3] = {
        GL_STATIC_DRAW,
        GL_DYNAMIC_DRAW,
        GL_STREAM_DRAW
    };

    glGenBuffers(3, triangleVBOs);

    for (int i = 0; i < 3; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, triangleVBOs[i]);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(vertices[i]),
            vertices[i],
            usageHints[i]
        );
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);

    GLfloat colors[3][3] = {
        {1.0f, 0.2f, 0.2f},
        {0.2f, 1.0f, 0.2f},
        {0.2f, 0.4f, 1.0f}
    };

    for (int i = 0; i < 3; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, triangleVBOs[i]);
        glVertexPointer(3, GL_FLOAT, 0, (void*)0);
        glColor3fv(colors[i]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(3, triangleVBOs);
        std::exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("EX07 - VBO Usage Hints");

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