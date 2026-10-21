#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdlib>

GLuint VBOid;
GLfloat horizontalOffset = 0.0f;
GLfloat direction = 0.02f;

void initializeTriangle() {
    GLfloat vertices[] = {
         0.0f,  0.75f, 0.0f,
        -0.75f, 0.0f,  0.0f,
         0.75f, 0.0f,  0.0f
    };

    glGenBuffers(1, &VBOid);
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor4f(0.16f, 0.72f, 0.08f, 1.0f);

    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glFlush();
}

void animateStep(int value) {
    horizontalOffset += direction;

    if (horizontalOffset >= 0.3f || horizontalOffset <= -0.3f) {
        direction = -direction;
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBOid);

    GLfloat* vertices = static_cast<GLfloat*>(
        glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE)
    );

    if (vertices != nullptr) {
        vertices[0] = horizontalOffset;
        vertices[3] = -0.75f + horizontalOffset;
        vertices[6] = 0.75f + horizontalOffset;
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glutPostRedisplay();
    glutTimerFunc(200, animateStep, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        glDeleteBuffers(1, &VBOid);
        std::exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("EX16 - Timer and glMapBuffer Animation");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

#ifndef __APPLE__
    if (glewInit() != GLEW_OK) {
        return 1;
    }
#endif

    initializeTriangle();
    glutTimerFunc(200, animateStep, 0);
    glutMainLoop();

    return 0;
}