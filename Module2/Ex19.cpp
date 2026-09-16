#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/*
 * Example 19 - Stipple Pattern Gallery
 *
 * Combines line width, line stippling, and color.
 * Expected output: four stacked lines with different
 * colors and dash/dot patterns.
 */

void drawStippledLine(
    float y,
    GLint factor,
    GLushort pattern,
    float red,
    float green,
    float blue
) {
    glColor3f(red, green, blue);
    glLineStipple(factor, pattern);

    glBegin(GL_LINES);
    glVertex2f(-0.8f, y);
    glVertex2f(0.8f, y);
    glEnd();
}

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(3.0f);
    glEnable(GL_LINE_STIPPLE);

    // White dashed line
    drawStippledLine(
        0.6f, 1, 0x00FF,
        1.0f, 1.0f, 1.0f
    );

    // Orange dash-dot line
    drawStippledLine(
        0.2f, 1, 0x0C0F,
        1.0f, 0.6f, 0.0f
    );

    // Cyan dotted line
    drawStippledLine(
        -0.2f, 1, 0xAAAA,
        0.2f, 0.8f, 1.0f
    );

    // Pink stretched dotted line
    drawStippledLine(
        -0.6f, 2, 0xAAAA,
        1.0f, 0.2f, 0.6f
    );

    glDisable(GL_LINE_STIPPLE);
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 500);
    glutCreateWindow("Ex19 - Stipple Pattern Gallery");

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}