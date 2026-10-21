#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

GLuint roofVBO;
GLuint wallVBO;
GLuint sunVBO;

void drawRoof() {
    GLfloat vertices[] = {
         0.0f,  0.6f, 0.0f,
        -0.5f,  0.1f, 0.0f,
         0.5f,  0.1f, 0.0f
    };

    glGenBuffers(1, &roofVBO);
    glBindBuffer(GL_ARRAY_BUFFER, roofVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glColor3f(0.7f, 0.2f, 0.2f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void drawWall() {
    GLfloat vertices[] = {
        -0.5f,  0.1f, 0.0f,
         0.5f,  0.1f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    glGenBuffers(1, &wallVBO);
    glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glColor3f(0.85f, 0.7f, 0.5f);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void drawSun() {
    GLfloat vertices[] = {
        0.75f, 0.70f, 0.0f,
        0.65f, 0.60f, 0.0f,
        0.75f, 0.55f, 0.0f,
        0.85f, 0.60f, 0.0f,
        0.80f, 0.72f, 0.0f
    };

    glGenBuffers(1, &sunVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glColor3f(1.0f, 0.85f, 0.1f);
    glDrawArrays(GL_POLYGON, 0, 5);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawWall();
    drawRoof();
    drawSun();

    glDeleteBuffers(1, &roofVBO);
    glDeleteBuffers(1, &wallVBO);
    glDeleteBuffers(1, &sunVBO);

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("EX18 - House Scene with Three VBOs");

    glutDisplayFunc(display);

#ifndef __APPLE__
    if (glewInit() != GLEW_OK) {
        return 1;
    }
#endif

    glutMainLoop();
    return 0;
}