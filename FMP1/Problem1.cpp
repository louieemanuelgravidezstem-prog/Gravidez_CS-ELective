#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>
using namespace std;


// Use small triangles to make each circular light.
const int SEGMENTS = 40;
const int RECTANGLE_VERTICES = 6;
const int CIRCLE_VERTICES = SEGMENTS * 3;
const float PI = 3.14159265f;

GLuint vertexBuffer = 0;
// Start with the red light on.
int activeSignal = 1;
string message = "Ready. Select a number from the menu.";

// Set up the shape data once when the program starts.
void createBuffer() {
    // Store the rectangle first, then the triangles that form the circle.
    GLfloat vertices[(RECTANGLE_VERTICES + CIRCLE_VERTICES) * 2] = {
        0, 0,  1, 0,  1, 1,
        0, 0,  1, 1,  0, 1
    };

    int position = RECTANGLE_VERTICES * 2;
    // Build the circle one triangle at a time.
    for (int i = 0; i < SEGMENTS; i++) {
        float angle1 = 2 * PI * i / SEGMENTS;
        float angle2 = 2 * PI * (i + 1) / SEGMENTS;

        vertices[position++] = 0;
        vertices[position++] = 0;
        vertices[position++] = cos(angle1);
        vertices[position++] = sin(angle1);
        vertices[position++] = cos(angle2);
        vertices[position++] = sin(angle2);
    }

    // Save both shapes in one buffer for drawing.
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Free the buffer before closing the program.
void deleteBuffer() {
    if (vertexBuffer != 0) {
        glDeleteBuffers(1, &vertexBuffer);
        vertexBuffer = 0;
    }
}

// Reuse the rectangle to draw the ground, pole, body, and menu panel.
void drawRectangle(float x, float y, float width, float height,
                   float red, float green, float blue) {
    glColor3f(red, green, blue);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(width, height, 1);
    glDrawArrays(GL_TRIANGLES, 0, RECTANGLE_VERTICES);
    glPopMatrix();
}

// Draw a circular light at the given position and color.
void drawLight(float x, float y, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(42, 42, 1);
    glDrawArrays(GL_TRIANGLES, RECTANGLE_VERTICES, CIRCLE_VERTICES);
    glPopMatrix();
}

// Show text at the given position in the window.
void drawText(float x, float y, const string& text,
              void* font = GLUT_BITMAP_HELVETICA_18) {
    glColor3f(0.12f, 0.16f, 0.20f);
    glRasterPos2f(x, y);
    for (char letter : text) {
        glutBitmapCharacter(font, letter);
    }
}

// Draw the traffic light, menu, and current signal state.
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, nullptr);

    drawRectangle(0, 0, 800, 70, 0.55f, 0.72f, 0.45f);
    drawRectangle(180, 40, 30, 75, 0.45f, 0.47f, 0.49f);
    drawRectangle(80, 100, 230, 400, 0.35f, 0.38f, 0.41f);
    drawRectangle(90, 110, 210, 380, 0.10f, 0.11f, 0.13f);
    drawRectangle(350, 100, 410, 400, 0.98f, 0.98f, 0.95f);

    // Light up the selected signal and keep the others dark.
    if (activeSignal == 1) drawLight(195, 420, 1.0f, 0.10f, 0.10f);
    else drawLight(195, 420, 0.22f, 0.04f, 0.04f);

    if (activeSignal == 2) drawLight(195, 300, 1.0f, 0.85f, 0.05f);
    else drawLight(195, 300, 0.22f, 0.18f, 0.03f);

    if (activeSignal == 3) drawLight(195, 180, 0.10f, 0.90f, 0.25f);
    else drawLight(195, 180, 0.03f, 0.18f, 0.05f);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    drawText(80, 550, "CAMPUS TRAFFIC SIGNAL VISUALIZER");
    drawText(380, 460, "MENU - PRESS A NUMBER");
    drawText(380, 415, "1  Red / Stop");
    drawText(380, 380, "2  Yellow / Caution");
    drawText(380, 345, "3  Green / Go");
    drawText(380, 310, "4  All Off");
    drawText(380, 275, "5  Reset");
    drawText(380, 240, "0  Exit");

    string state = "ALL OFF";
    if (activeSignal == 1) state = "RED - STOP";
    else if (activeSignal == 2) state = "YELLOW - CAUTION";
    else if (activeSignal == 3) state = "GREEN - GO";

    drawText(380, 180, "State: " + state);
    drawText(380, 140, message, GLUT_BITMAP_HELVETICA_12);
    drawText(80, 20, "Click the graphics window, then press 0-5. No Enter is needed.",
             GLUT_BITMAP_HELVETICA_12);
    glutSwapBuffers();
}

// Adjust the drawing area when the window size changes.
void reshape(int width, int height) {
    if (width < 1) width = 1;
    if (height < 1) height = 1;

    // Keep the lights round when the window is resized.
    int viewWidth = width;
    int viewHeight = static_cast<int>(width * 600.0 / 800.0);
    if (viewHeight > height) {
        viewHeight = height;
        viewWidth = static_cast<int>(height * 800.0 / 600.0);
    }
    glViewport((width - viewWidth) / 2, (height - viewHeight) / 2,
               viewWidth, viewHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

// Read the menu number and change the active signal.
void keyboard(unsigned char key, int, int) {
    switch (key) {
        case '1':
            activeSignal = 1;
            message = "Red selected. Stop.";
            break;
        case '2':
            activeSignal = 2;
            message = "Yellow selected. Proceed with caution.";
            break;
        case '3':
            activeSignal = 3;
            message = "Green selected. Go.";
            break;
        case '4':
            // Turn off all three lights.
            activeSignal = 0;
            message = "All signals are off.";
            break;
        case '5':
            // Reset the traffic light to red.
            activeSignal = 1;
            message = "Reset complete. Red is the starting signal.";
            break;
        case '0':
            deleteBuffer();
            glutDestroyWindow(glutGetWindow());
            std::exit(EXIT_SUCCESS);
        default:
            message = "Invalid input. Choose a number from 0 to 5.";
    }
    cout << message << endl;
    glutPostRedisplay();
}

// Treat arrow and function keys as invalid menu choices.
void specialKeyboard(int, int, int) {
    keyboard('?', 0, 0);
}

// Create the window and start the program.
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    int window = glutCreateWindow("Problem 1 - Campus Traffic Signal Visualizer");

#ifndef __APPLE__
    // Set up GLEW after creating the window.
    GLenum result = glewInit();
    if (result != GLEW_OK) {
        cerr << "GLEW initialization failed: " << glewGetErrorString(result) << endl;
        glutDestroyWindow(window);
        return EXIT_FAILURE;
    }
    if (!GLEW_VERSION_1_5) {
        cerr << "OpenGL 1.5 or newer is required for VBO support." << endl;
        glutDestroyWindow(window);
        return EXIT_FAILURE;
    }
#endif

    glClearColor(0.85f, 0.92f, 0.96f, 1.0f);
    createBuffer();
    if (glGetError() != GL_NO_ERROR) {
        cerr << "Unable to create the graphics buffer." << endl;
        deleteBuffer();
        glutDestroyWindow(window);
        return EXIT_FAILURE;
    }

    cout << "Click inside the graphics window and press a menu number:\n"
         << "1 Red / Stop\n2 Yellow / Caution\n3 Green / Go\n"
         << "4 All Off\n5 Reset\n0 Exit\n";

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutMainLoop();
    return EXIT_SUCCESS;
}


