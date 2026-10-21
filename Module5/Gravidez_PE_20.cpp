#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdlib>

GLuint shipPositionVBO;
GLuint shipColorVBO;
GLuint groundPositionVBO;
GLuint groundIndexVBO;

GLfloat shipOffset = 0.0f;

void initializeShip() {
    GLfloat positions[] = {
        -0.9f, 0.2f, 0.0f,
        -0.7f, 0.2f, 0.0f,
        -0.8f, 0.5f, 0.0f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 1.0f
    };

    glGenBuffers(1, &shipPositionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, shipPositionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &shipColorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, shipColorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void initializeGround() {
    GLfloat positions[] = {
        -1.0f, -0.6f, 0.0f,
         1.0f, -0.6f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f
    };

    GLubyte indices[] = {0, 1, 2, 0, 2, 3};

    glGenBuffers(1, &groundPositionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, groundPositionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glGenBuffers(1, &groundIndexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundIndexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);

    glColor3f(0.2f, 0.7f, 0.2f);
    glBindBuffer(GL_ARRAY_BUFFER, groundPositionVBO);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundIndexVBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glEnableClientState(GL_COLOR_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, shipPositionVBO);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, shipColorVBO);
    glColorPointer(3, GL_FLOAT, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glFlush();
}

void moveShip(int value) {
    shipOffset += 0.1f;

    if (shipOffset > 1.8f) {
        shipOffset = 0.0f;
    }

    glBindBuffer(GL_ARRAY_BUFFER, shipPositionVBO);

    GLfloat* positions = static_cast<GLfloat*>(
        glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE)
    );

    if (positions != nullptr) {
        positions[0] = -0.9f + shipOffset;
        positions[3] = -0.7f + shipOffset;
        positions[6] = -0.8f + shipOffset;

        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glutPostRedisplay();
    glutTimerFunc(1000, moveShip, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        GLuint buffers[] = {
            shipPositionVBO,
            shipColorVBO,
            groundPositionVBO,
            groundIndexVBO
        };

        glDeleteBuffers(4, buffers);
        std::exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 600);
    glutCreateWindow("Q20 - Animated Ship Over Indexed Ground");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

#ifndef __APPLE__
    if (glewInit() != GLEW_OK) {
        return 1;
    }
#endif

    initializeShip();
    initializeGround();

    glutTimerFunc(1000, moveShip, 0);
    glutMainLoop();

    return 0;
}