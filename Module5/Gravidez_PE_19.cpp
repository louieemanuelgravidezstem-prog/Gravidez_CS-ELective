#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdlib>

GLuint positionVBO;
GLuint colorVBO;
int currentColorScheme = 0;

GLfloat colorSchemes[3][9] = {
    {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    },
    {
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f
    },
    {
        1.0f, 0.5f, 0.0f,
        0.5f, 0.0f, 1.0f,
        0.0f, 0.7f, 0.3f
    }
};

void initializeTriangle() {
    GLfloat positions[] = {
         0.0f,  0.7f, 0.0f,
        -0.6f, -0.5f, 0.0f,
         0.6f, -0.5f, 0.0f
    };

    glGenBuffers(1, &positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorSchemes[0]), colorSchemes[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glColorPointer(3, GL_FLOAT, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'c' || key == 'C') {
        currentColorScheme = (currentColorScheme + 1) % 3;

        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);

        GLfloat* colors = static_cast<GLfloat*>(
            glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE)
        );

        if (colors != nullptr) {
            for (int i = 0; i < 9; i++) {
                colors[i] = colorSchemes[currentColorScheme][i];
            }

            glUnmapBuffer(GL_ARRAY_BUFFER);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glutPostRedisplay();
    }

    if (key == 27) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        GLuint buffers[] = {positionVBO, colorVBO};
        glDeleteBuffers(2, buffers);

        std::exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q19 - Keyboard Color Change");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

#ifndef __APPLE__
    if (glewInit() != GLEW_OK) {
        return 1;
    }
#endif

    initializeTriangle();
    glutMainLoop();

    return 0;
}