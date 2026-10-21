#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdlib>

GLuint trianglePositionVBO;
GLuint triangleColorVBO;
GLuint groundVBO;
GLuint groundIndexVBO;

void buildScene() {
    GLfloat trianglePositions[] = {
         0.0f,  0.75f, 0.0f,
        -0.3f,  0.35f, 0.0f,
         0.3f,  0.35f, 0.0f
    };

    GLfloat triangleColors[] = {
        1.0f, 0.2f, 0.2f,
        0.2f, 1.0f, 0.2f,
        0.2f, 0.2f, 1.0f
    };

    GLfloat groundVertices[] = {
        -0.9f, -0.9f, 0.0f,
         0.9f, -0.9f, 0.0f,
         0.9f, -0.6f, 0.0f,
        -0.9f, -0.6f, 0.0f
    };

    GLubyte groundIndices[] = {0, 1, 2, 0, 2, 3};

    glGenBuffers(1, &trianglePositionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, trianglePositionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePositions), trianglePositions, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &triangleColorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleColorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColors), triangleColors, GL_STATIC_DRAW);

    glGenBuffers(1, &groundVBO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &groundIndexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundIndexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundIndices), groundIndices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void drawGround() {
    glDisableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundIndexVBO);
    glColor3f(0.3f, 0.6f, 0.3f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void drawTriangle() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, trianglePositionVBO);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, triangleColorVBO);
    glColorPointer(3, GL_FLOAT, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawGround();
    drawTriangle();
    glFlush();
}

void animateStep(int value) {
    static GLfloat dy = -0.01f;

    glBindBuffer(GL_ARRAY_BUFFER, trianglePositionVBO);

    GLfloat* positions = static_cast<GLfloat*>(
        glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE)
    );

    if (positions != nullptr) {
        if (positions[1] > 0.75f || positions[1] < 0.4f) {
            dy = -dy;
        }

        positions[1] += dy;
        positions[4] += dy;
        positions[7] += dy;

        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glutPostRedisplay();
    glutTimerFunc(100, animateStep, 0);
}

void monitorKeyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        GLuint buffers[] = {
            trianglePositionVBO,
            triangleColorVBO,
            groundVBO,
            groundIndexVBO
        };

        glDeleteBuffers(4, buffers);
        std::exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("EX20 - Animated VBO Scene");

    glutDisplayFunc(display);
    glutKeyboardFunc(monitorKeyboard);

#ifndef __APPLE__
    if (glewInit() != GLEW_OK) {
        return 1;
    }
#endif

    buildScene();
    glutTimerFunc(100, animateStep, 0);
    glutMainLoop();

    return 0;
}