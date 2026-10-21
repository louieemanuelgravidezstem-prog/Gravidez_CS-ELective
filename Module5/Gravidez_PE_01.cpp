#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdlib>

GLuint pointVBO;

void initializePoints() {
    // Five points: top-left, center, bottom-right, top-right, bottom-left
    GLfloat points[] = {
        -0.6f,  0.6f, 0.0f,
         0.0f,  0.0f, 0.0f,
         0.6f, -0.6f, 0.0f,
         0.6f,  0.6f, 0.0f,
        -0.6f, -0.6f, 0.0f
    };

    // Generate a buffer ID.
    glGenBuffers(1, &pointVBO);

    // Bind the buffer so OpenGL knows which buffer to use.
    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);

    // Upload the point positions to the buffer.
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(points),
        points,
        GL_STATIC_DRAW
    );

    // Unbind the buffer after uploading.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(12.0f);

    // Bind the VBO before using its data.
    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);

    glEnableClientState(GL_VERTEX_ARRAY);

    // Offset 0 means the vertex data starts at the beginning of the VBO.
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);

    // Draw 5 points from the VBO.
    glDrawArrays(GL_POINTS, 0, 5);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // Esc
        glDeleteBuffers(1, &pointVBO);
        std::exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q01 - VBO X-Pattern Points");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

#ifndef __APPLE__
    if (glewInit() != GLEW_OK) {
        return 1;
    }
#endif

    initializePoints();
    glutMainLoop();

    return 0;
}