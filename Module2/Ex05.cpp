#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
using namespace std;

  
/* 
 * Example 05 - glLineWidth 
 * ------------------------------------ 
 * Concept: void glLineWidth(GLfloat width) sets the fixed width of 
 * lines. The default is 1.0; width must be greater than 0.0. 
 * This example draws three lines with three different widths so 
 * students can compare them directly. 
 */ 
  
void display() { 
    glClear(GL_COLOR_BUFFER_BIT); 
    glColor3f(1.0f, 1.0f, 1.0f); 
  
    glLineWidth(1.0f); 
    glBegin(GL_LINES); 
        glVertex2f(-0.7f, 0.5f); 
        glVertex2f(0.7f, 0.5f); 
    glEnd(); 
  
    glLineWidth(4.0f); 
    glBegin(GL_LINES); 
        glVertex2f(-0.7f, 0.0f); 
        glVertex2f(0.7f, 0.0f); 
    glEnd(); 
  
    glLineWidth(8.0f); 
    glBegin(GL_LINES); 
        glVertex2f(-0.7f, -0.5f); 
        glVertex2f(0.7f, -0.5f); 
    glEnd(); 
  
    glFlush(); 
} 
  
int main(int argc, char** argv) { 
    glutInit(&argc, argv); 
    glutInitWindowSize(600, 600); 
    glutCreateWindow("Ex05 - glLineWidth Comparison"); 
    glutDisplayFunc(display); 
    glutMainLoop(); 
    return 0; 
} 