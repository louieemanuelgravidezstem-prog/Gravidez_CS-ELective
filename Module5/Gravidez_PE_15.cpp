#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdlib>

GLuint quadVBO;
GLfloat verticalOffset = 0.0f;
GLfloat direction = 1.0f;

void initializeQuad() {
    GLfloat vertices[] = {
        -0.4f, -0.3f, 0.0f,
         0.4f, -0.3f, 0.0f,
         0.4f,  0.3f, 0.0f,
        -0.4f,  0.3f, 0.0f
    };

    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.2f, 0.7f, 1.0f);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glDrawArrays(GL_QUADS, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glFlush();
}

void bounce(int value) {
    verticalOffset += direction * 0.02f;

    if (verticalOffset >= 0.5f) {
        verticalOffset = 0.5f;
        direction = -1.0f;
    } else if (verticalOffset <= -0.5f) {
        verticalOffset = -0.5f;
        direction = 1.0f;
    }

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    GLfloat* vertices = static_cast<GLfloat*>(
        glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE)
    );

    if (vertices != nullptr) {
        vertices[1] = -0.3f + verticalOffset;
        vertices[4] = -0.3f + verticalOffset;
        vertices[7] =  0.3f + verticalOffset;
        vertices[10] = 0.3f + verticalOffset;

        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glutPostRedisplay();
    glutTimerFunc(16, bounce, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        glDeleteBuffers(1, &quadVBO);
        std::exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q15 - Bouncing VBO Quad");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

#ifndef __APPLE__
    if (glewInit() != GLEW_OK) {
        return 1;
    }
#endif

    initializeQuad();
    glutTimerFunc(16, bounce, 0);
    glutMainLoop();

    return 0;
}