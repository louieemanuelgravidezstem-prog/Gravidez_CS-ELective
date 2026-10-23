
#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

GLuint vertexBuffer = 0;
GLuint indexBuffer = 0;

// Keep track of the visible features and the selected layout.
bool showPlayground = true;
bool showGarden = true;
bool showSeating = true;
bool layoutA = true;
string statusMessage = "Ready. Press a number from the menu.";

// These four corners form one rectangle.
GLfloat vertices[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
};

// Use two triangles that share the same corners.
GLuint indices[] = {
    0, 1, 2,
    0, 2, 3
};

// Save the rectangle data once when the program starts.
void createBuffers() {
    // Store the positions of the four corners.
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Store the order used to join the corners into triangles.
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Free both buffers before closing the program.
void cleanup() {
    if (vertexBuffer != 0) {
        glDeleteBuffers(1, &vertexBuffer);
        vertexBuffer = 0;
    }
    if (indexBuffer != 0) {
        glDeleteBuffers(1, &indexBuffer);
        indexBuffer = 0;
    }
}

// Show text at the given position in the window.
void drawText(float x, float y, const string& text,
              void* font = GLUT_BITMAP_HELVETICA_18) {
    glColor3f(0.12f, 0.17f, 0.14f);
    glRasterPos2f(x, y);
    for (char letter : text) {
        glutBitmapCharacter(font, letter);
    }
}


// Move and resize the rectangle using its saved corners.
void drawRectangle(float x, float y, float width, float height,
                   float red, float green, float blue) {
    glColor3f(red, green, blue);
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(width, height, 1.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glPopMatrix();
}

// Draw the playground in orange and add its label.
void drawPlayground(float x, float y, float width, float height) {
    drawRectangle(x, y, width, height, 1.0f, 0.65f, 0.25f);
    drawText(x + 20, y + height / 2, "Playground");
}

// Draw the garden in green and add its label.
void drawGarden(float x, float y, float width, float height) {
    drawRectangle(x, y, width, height, 0.35f, 0.72f, 0.42f);
    drawText(x + 20, y + height / 2, "Garden");
}

// Draw the seating area in blue and add its label.
void drawSeating(float x, float y, float width, float height) {
    drawRectangle(x, y, width, height, 0.43f, 0.70f, 0.95f);
    drawText(x + 20, y + height / 2, "Seating");
}

// Draw the park and place the features in the selected layout.
void drawPark() {
    
    // Draw the park border and the grass inside it.
    drawRectangle(35, 100, 620, 510, 0.20f, 0.36f, 0.25f);
    drawRectangle(45, 110, 600, 490, 0.78f, 0.88f, 0.67f);

    // Add the walking paths and the central activity area.
    drawRectangle(325, 110, 40, 490, 0.88f, 0.80f, 0.65f);
    drawRectangle(45, 335, 600, 40, 0.88f, 0.80f, 0.65f);
    drawRectangle(300, 310, 90, 90, 0.76f, 0.67f, 0.88f);
    drawText(312, 356, "Activity", GLUT_BITMAP_HELVETICA_12);
    drawText(324, 338, "area", GLUT_BITMAP_HELVETICA_12);
    drawText(60, 348, "Walking path", GLUT_BITMAP_HELVETICA_12);

    // Draw only the features that are currently visible.
    if (layoutA) {
        if (showPlayground) drawPlayground(90, 425, 185, 120);
        if (showGarden) drawGarden(425, 425, 175, 120);
        if (showSeating) drawSeating(110, 180, 170, 95);
    } else {
        if (showPlayground) drawPlayground(425, 170, 175, 120);
        if (showGarden) drawGarden(90, 425, 185, 120);
        if (showSeating) drawSeating(440, 465, 160, 80);
    }
}

// Show the menu and explain the feature colors.
void drawMenu() {
    drawRectangle(685, 100, 290, 510, 0.96f, 0.97f, 0.94f);
    drawText(710, 575, "MENU - PRESS A NUMBER");
    drawText(710, 540, "1  Show All");
    drawText(710, 510, "2  Toggle Playground");
    drawText(710, 480, "3  Toggle Garden");
    drawText(710, 450, "4  Toggle Seating");
    drawText(710, 420, "5  Layout A");
    drawText(710, 390, "6  Layout B");
    drawText(710, 360, "0  Exit");
    drawText(710, 315, "COLOR LEGEND");

    drawRectangle(710, 280, 18, 18, 1.0f, 0.65f, 0.25f);
    drawText(740, 282, "Playground");
    drawRectangle(710, 248, 18, 18, 0.35f, 0.72f, 0.42f);
    drawText(740, 250, "Garden");
    drawRectangle(710, 216, 18, 18, 0.43f, 0.70f, 0.95f);
    drawText(740, 218, "Seating");
    drawRectangle(710, 184, 18, 18, 0.88f, 0.80f, 0.65f);
    drawText(740, 186, "Walking path");
    drawRectangle(710, 152, 18, 18, 0.76f, 0.67f, 0.88f);
    drawText(740, 154, "Activity area");
}

// Draw the park, menu, and latest status message.
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexPointer(2, GL_FLOAT, 0, nullptr);
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    drawText(35, 645, "COMMUNITY PARK LAYOUT PLANNER");
    drawText(490, 645, layoutA ? "Layout A" : "Layout B");
    drawPark();
    drawMenu();
    drawText(35, 65, statusMessage);
    drawText(35, 35, "Click inside this window, then press 0-6. No Enter is needed.",
             GLUT_BITMAP_HELVETICA_12);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glutSwapBuffers();
}

// Adjust the drawing area when the window size changes.
void reshape(int width, int height) {
    if (width < 1) width = 1;
    if (height < 1) height = 1;

  
    // Keep the park from stretching when the window is resized.
    int viewWidth = width;
    int viewHeight = static_cast<int>(width * 680.0 / 1000.0);
    if (viewHeight > height) {
        viewHeight = height;
        viewWidth = static_cast<int>(height * 1000.0 / 680.0);
    }
    glViewport((width - viewWidth) / 2, (height - viewHeight) / 2,
               viewWidth, viewHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1000, 0, 680, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

// Read the menu number to show, hide, or rearrange the features.
void keyboard(unsigned char key, int, int) {
    switch (key) {
        case '1':
            // Show all three park features.
            showPlayground = showGarden = showSeating = true;
            statusMessage = "All features are visible.";
            break;
        case '2':
            showPlayground = !showPlayground;
            statusMessage = showPlayground ? "Playground shown." : "Playground hidden.";
            break;
        case '3':
            showGarden = !showGarden;
            statusMessage = showGarden ? "Garden shown." : "Garden hidden.";
            break;
        case '4':
            showSeating = !showSeating;
            statusMessage = showSeating ? "Seating shown." : "Seating hidden.";
            break;
        case '5':
            layoutA = true;
            statusMessage = "Layout A selected.";
            break;
        case '6':
            layoutA = false;
            statusMessage = "Layout B selected.";
            break;
        case '0':
            cleanup();
            glutDestroyWindow(glutGetWindow());
            std::exit(EXIT_SUCCESS);
        default:
            statusMessage = "Invalid input. Press a number from 0 to 6.";
    }
    cout << statusMessage << endl;
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
    glutInitWindowSize(1000, 680);
    int window = glutCreateWindow("Machine Problem 02 - Community Park Layout Planner");

#ifndef __APPLE__
   
    // Set up GLEW after creating the window.
    GLenum result = glewInit();
    if (result != GLEW_OK) {
        cerr << "GLEW initialization failed: " << glewGetErrorString(result) << endl;
        glutDestroyWindow(window);
        return 1;
    }
    if (!GLEW_VERSION_1_5) {
        cerr << "OpenGL 1.5 or newer is required for this program." << endl;
        glutDestroyWindow(window);
        return 1;
    }
#else

    (void)window;
#endif

    glClearColor(0.92f, 0.95f, 0.89f, 1.0f);
    createBuffers();
    cout << "COMMUNITY PARK LAYOUT PLANNER\n"
         << "Click inside the graphics window, then press:\n"
         << "1 Show All\n2 Toggle Playground\n3 Toggle Garden\n"
         << "4 Toggle Seating\n5 Layout A\n6 Layout B\n0 Exit\n";

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutMainLoop();
    return 0;
}