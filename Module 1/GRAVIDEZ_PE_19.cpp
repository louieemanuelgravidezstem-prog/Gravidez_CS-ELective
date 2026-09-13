#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

float colors[6][3] = {
    { 1.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f },
    { 1.0f, 1.0f, 0.0f },
    { 1.0f, 0.0f, 1.0f },
    { 0.0f, 1.0f, 1.0f }
};

string colorNames[6] = {
    "Red",
    "Green",
    "Blue",
    "Yellow",
    "Magenta",
    "Cyan"
};

int selectedColor = 0;

void display() {
    glClearColor(
        colors[selectedColor][0],
        colors[selectedColor][1],
        colors[selectedColor][2],
        1.0f
    );

    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key >= '1' && key <= '6') {
        selectedColor = key - '1';
        cout << "Selected color: "
            << colorNames[selectedColor] << endl;
        glutPostRedisplay();
    }

    if (key == 27) {
        exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q19 - Keyboard Color Picker");

    cout << "Press keys 1 through 6 to change color." << endl;
    cout << "Press ESC to quit." << endl;

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}