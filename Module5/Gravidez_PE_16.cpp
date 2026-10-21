#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdlib>

GLuint sunVBO;
GLuint sunIndexVBO;
GLuint groundVBO;
GLuint groundIndexVBO;
GLuint trunkVBO;
GLuint leavesVBO;

void initializeSun() {
    GLfloat vertices[] = {
        -0.65f,  0.65f, 0.0f,
        -0.65f,  0.83f, 0.0f,
        -0.49f,  0.74f, 0.0f,
        -0.49f,  0.56f, 0.0f,
        -0.65f,  0.47f, 0.0f,
        -0.81f,  0.56f, 0.0f,
        -0.81f,  0.74f, 0.0f
    };

    GLubyte indices[] = {0, 1, 2, 3, 4, 5, 6, 1};

    glGenBuffers(1, &sunVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &sunIndexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sunIndexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void initializeGround() {
    GLfloat vertices[] = {
        -1.0f, -0.5f, 0.0f,
         1.0f, -0.5f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f
    };

    GLubyte indices[] = {0, 1, 2, 0, 2, 3};

    glGenBuffers(1, &groundVBO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &groundIndexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundIndexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void initializeTree() {
    GLfloat trunkVertices[] = {
         0.35f, -0.5f, 0.0f,
         0.55f, -0.5f, 0.0f,
         0.55f,  0.1f, 0.0f,
         0.35f,  0.1f, 0.0f
    };

    GLfloat leavesVertices[] = {
         0.15f,  0.05f, 0.0f,
         0.75f,  0.05f, 0.0f,
         0.45f,  0.75f, 0.0f
    };

    glGenBuffers(1, &trunkVBO);
    glBindBuffer(GL_ARRAY_BUFFER, trunkVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trunkVertices), trunkVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &leavesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, leavesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(leavesVertices), leavesVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColor3f(1.0f, 0.8f, 0.0f);
    glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sunIndexVBO);
    glDrawElements(GL_TRIANGLE_FAN, 8, GL_UNSIGNED_BYTE, (void*)0);

    glColor3f(0.2f, 0.7f, 0.2f);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundIndexVBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glColor3f(0.5f, 0.25f, 0.1f);
    glBindBuffer(GL_ARRAY_BUFFER, trunkVBO);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glDrawArrays(GL_QUADS, 0, 4);

    glColor3f(0.0f, 0.5f, 0.1f);
    glBindBuffer(GL_ARRAY_BUFFER, leavesVBO);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        GLuint buffers[] = {
            sunVBO, sunIndexVBO,
            groundVBO, groundIndexVBO,
            trunkVBO, leavesVBO
        };

        glDeleteBuffers(6, buffers);
        std::exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 600);
    glutCreateWindow("Q16 - VBO Scene");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

#ifndef __APPLE__
    if (glewInit() != GLEW_OK) {
        return 1;
    }
#endif

    initializeSun();
    initializeGround();
    initializeTree();

    glutMainLoop();

    return 0;
}