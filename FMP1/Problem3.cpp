#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

// Module 5 - Machine Problem 03: Warehouse Robot Route Simulator
// Click inside the window, then press a number from the menu.

GLuint robotBuffer = 0;
GLuint rectangleBuffer = 0;
// The robot has three corners, each with an X and Y coordinate.
GLfloat robot[] = {120, 190, 105, 160, 135, 160};
const float LEFT = 50, RIGHT = 630, BOTTOM = 130, TOP = 550;
float stepSize = 20;
bool enteringStep = false;
bool autoMove = false;
int targetZone = 0;
string stepInput;
string message = "Ready. Choose a menu number.";
// These are the centers of zones A, B, and C.
const float targets[3][2] = {{550, 200}, {550, 480}, {150, 480}};

// Turn a number into text so we can show it on screen.
string number(float value) {
    ostringstream text;
    text << value;
    return text.str();
}

// Set up the shape data when the program starts.
void createBuffers() {
    GLfloat rectangle[] = {0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1};

    // Save the robot coordinates once so we can update them when it moves.
    glGenBuffers(1, &robotBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, robotBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(robot), robot, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &rectangleBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, rectangleBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Free the buffers before closing the program.
void deleteBuffers() {
    if (robotBuffer != 0) glDeleteBuffers(1, &robotBuffer);
    if (rectangleBuffer != 0) glDeleteBuffers(1, &rectangleBuffer);
    robotBuffer = rectangleBuffer = 0;
}

// Reuse one rectangle to draw the walls, zones, and menu panel.
void drawRectangle(float x, float y, float width, float height,
                   float red, float green, float blue) {
    glBindBuffer(GL_ARRAY_BUFFER, rectangleBuffer);
    glVertexPointer(2, GL_FLOAT, 0, nullptr);
    glColor3f(red, green, blue);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(width, height, 1);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glPopMatrix();
}

// Show text at the given position in the window.
void drawText(float x, float y, const string& text,
              void* font = GLUT_BITMAP_HELVETICA_18) {
    glColor3f(0.12f, 0.16f, 0.20f);
    glRasterPos2f(x, y);
    for (char letter : text) glutBitmapCharacter(font, letter);
}

// Move all three corners by the same amount to keep the robot's shape.
bool moveRobot(float dx, float dy) {
    float minX = min(robot[0], min(robot[2], robot[4]));
    float maxX = max(robot[0], max(robot[2], robot[4]));
    float minY = min(robot[1], min(robot[3], robot[5]));
    float maxY = max(robot[1], max(robot[3], robot[5]));

    // Keep all three corners of the robot inside the warehouse.
    dx = max(LEFT - minX, min(dx, RIGHT - maxX));
    dy = max(BOTTOM - minY, min(dy, TOP - maxY));
    if (dx == 0 && dy == 0) {
        message = "Boundary reached. Choose another direction.";
        return false;
    }

    // Work out the new coordinates before changing the buffer.
    GLfloat next[6];
    bool changed = false;
    for (int i = 0; i < 3; i++) {
        next[i * 2] = robot[i * 2] + dx;
        next[i * 2 + 1] = robot[i * 2 + 1] + dy;
        if (next[i * 2] != robot[i * 2] ||
            next[i * 2 + 1] != robot[i * 2 + 1]) changed = true;
    }
    if (!changed) {
        message = "Step too small. Choose a larger value.";
        return false;
    }

    glBindBuffer(GL_ARRAY_BUFFER, robotBuffer);
    // Open the robot buffer so we can change its coordinates.
    GLfloat* data = static_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE));
    bool updated = false;
    if (data != nullptr) {
        // Change the X and Y coordinates of each corner.
        for (int i = 0; i < 3; i++) {
            data[i * 2] += dx;
            data[i * 2 + 1] += dy;
        }
        updated = (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_TRUE);
        // We cannot use this pointer after closing the mapped buffer.
    }

    // If the mapped update fails, send all six saved coordinates instead.
    if (!updated) glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(next), next);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    for (int i = 0; i < 6; i++) robot[i] = next[i];
    // Show which direction the robot actually moved.
    message = "Robot moved";
    if (dx < 0) message += " left";
    else if (dx > 0) message += " right";
    if (dy != 0) {
        if (dx != 0) message += " and";
        message += (dy > 0) ? " up" : " down";
    }
    message += ".";
    if (!updated) message += " Buffer-update fallback used.";
    return true;
}

// Draw the warehouse, robot, and menu.
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnableClientState(GL_VERTEX_ARRAY);

    drawRectangle(40, 120, 600, 440, 0.30f, 0.34f, 0.38f);
    drawRectangle(50, 130, 580, 420, 0.91f, 0.92f, 0.88f);
    drawRectangle(500, 160, 100, 80, 0.97f, 0.65f, 0.35f);
    drawRectangle(500, 440, 100, 80, 0.45f, 0.72f, 0.95f);
    drawRectangle(100, 440, 100, 80, 0.48f, 0.78f, 0.48f);
    drawRectangle(675, 120, 295, 440, 0.98f, 0.98f, 0.96f);

    glBindBuffer(GL_ARRAY_BUFFER, robotBuffer);
    glVertexPointer(2, GL_FLOAT, 0, nullptr);
    glColor3f(0.70f, 0.18f, 0.65f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    drawText(40, 640, "WAREHOUSE ROBOT ROUTE SIMULATOR");
    drawText(520, 195, "Zone A");
    drawText(520, 475, "Zone B");
    drawText(120, 475, "Zone C");
    drawText(700, 525, "MENU - PRESS A NUMBER");
    drawText(700, 485, "1  Move Left");
    drawText(700, 450, "2  Move Right");
    drawText(700, 415, "3  Move Up");
    drawText(700, 380, "4  Move Down");
    drawText(700, 345, "5  Set Step Size");
    drawText(700, 310, "6  Return to Start");
    drawText(700, 275, "7  Auto-Move Demo");
    drawText(700, 240, "0  Exit");
    drawText(700, 195, "Step size: " + number(stepSize));
    drawText(700, 160, autoMove ? "Auto demo: running" : "Auto demo: stopped",
             GLUT_BITMAP_HELVETICA_12);
    drawText(40, 85, message);
    drawText(40, 50, enteringStep ? "New step: " + stepInput + "_"
                                : "Click the graphics window, then press 0-7.");
    if (enteringStep)
        drawText(40, 20, "Type a positive number, then press Enter. Backspace edits; Esc cancels.",
                 GLUT_BITMAP_HELVETICA_12);
    glutSwapBuffers();
}

// Adjust the drawing area when the window size changes.
void reshape(int width, int height) {
    if (width < 1) width = 1;
    if (height < 1) height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1000, 0, 700, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

// Read and check the step size typed in the window.
void readStep(unsigned char key) {
    if (key == 13) { // Enter confirms the new step size.
        istringstream input(stepInput);
        float value;
        char extra;
        if ((input >> value) && !(input >> extra) && isfinite(value) && value > 0) {
            stepSize = value;
            enteringStep = false;
            message = "Step size set to " + number(stepSize) + ".";
        } else {
            message = "Invalid step. Enter a positive number.";
            stepInput.clear();
        }
    } else if (key == 8 || key == 127) { // Backspace or Delete removes the last character.
        if (!stepInput.empty()) stepInput.pop_back();
    } else if (key == 27) { // Esc cancels step entry.
        enteringStep = false;
        message = "Step entry cancelled.";
    } else if (key >= 32 && key <= 126 && stepInput.size() < 24) {
        stepInput += key;
    }
}

// Handle the numbers pressed for the menu.
void keyboard(unsigned char key, int, int) {
    if (enteringStep) {
        readStep(key);
        glutPostRedisplay();
        return;
    }
    switch (key) {
        case '1': autoMove = false; moveRobot(-stepSize, 0); break;
        case '2': autoMove = false; moveRobot(stepSize, 0); break;
        case '3': autoMove = false; moveRobot(0, stepSize); break;
        case '4': autoMove = false; moveRobot(0, -stepSize); break;
        case '5':
            autoMove = false;
            enteringStep = true;
            stepInput.clear();
            message = "Type the new step size below, then press Enter.";
            break;
        case '6':
            autoMove = false;
            moveRobot(120 - robot[0], 190 - robot[1]);
            message = "Robot returned to its starting position.";
            break;
        case '7':
            targetZone = 0;
            autoMove = true;
            message = "Auto demo started: Zone A, then B, then C.";
            break;
        case '0':
            deleteBuffers();
            glutDestroyWindow(glutGetWindow());
            std::exit(EXIT_SUCCESS);
        default:
            message = "Invalid input. Choose a number from 0 to 7.";
    }
    cout << message << endl;
    glutPostRedisplay();
}

// Show a reminder if the user presses an arrow or function key.
void specialKeyboard(int, int, int) {
    message = "Use the numbered menu. Arrow keys are not menu choices.";
    glutPostRedisplay();
}

// Move the robot toward zones A, B, and C during the auto demo.
void timer(int) {
    if (autoMove) {
        // Find the center of the robot.
        float centerX = (robot[0] + robot[2] + robot[4]) / 3;
        float centerY = (robot[1] + robot[3] + robot[5]) / 3;
        float dx = targets[targetZone][0] - centerX;
        float dy = targets[targetZone][1] - centerY;
        bool arriving = (fabs(dx) <= stepSize && fabs(dy) <= stepSize);
        dx = max(-stepSize, min(dx, stepSize));
        dy = max(-stepSize, min(dy, stepSize));

        if ((dx != 0 || dy != 0) && !moveRobot(dx, dy)) autoMove = false;
        if (autoMove && arriving) {
            targetZone++;
            if (targetZone == 3) {
                autoMove = false;
                message = "Auto demo finished. All three zones visited.";
            }
        }
        glutPostRedisplay();
    }
    // Run this timer again after 100 milliseconds.
    glutTimerFunc(100, timer, 0);
}

// Create the window and start the program.
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 700);
    int window = glutCreateWindow("Problem 3 - Warehouse Robot Route Simulator");

#ifndef __APPLE__
    // Set up GLEW after creating the window.
    GLenum result = glewInit();
    if (result != GLEW_OK || !GLEW_VERSION_1_5) {
        cerr << "GLEW initialization or OpenGL 1.5 support is unavailable." << endl;
        glutDestroyWindow(window);
        return EXIT_FAILURE;
    }
#endif

    glClearColor(0.86f, 0.91f, 0.95f, 1);
    createBuffers();
    if (glGetError() != GL_NO_ERROR) {
        cerr << "Unable to create the graphics buffers." << endl;
        deleteBuffers();
        glutDestroyWindow(window);
        return EXIT_FAILURE;
    }
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    // Start one timer. The timer function handles the next call.
    glutTimerFunc(100, timer, 0);
    glutMainLoop();
    return EXIT_SUCCESS;
}
