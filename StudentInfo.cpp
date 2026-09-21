#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

void text(float x, float y, void* font, const char* message) {
    glRasterPos2f(x, y);
    glutBitmapString(font, (const unsigned char*)message);
}

void rectangle(float left, float top, float right, float bottom) {
    glBegin(GL_QUADS);
    glVertex2f(left, top);
    glVertex2f(right, top);
    glVertex2f(right, bottom);
    glVertex2f(left, bottom);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);


    glColor3f(0.46f, 0.54f, 0.46f);
    text(355, 55, GLUT_BITMAP_TIMES_ROMAN_24, "Student's Subject Schedule");


    glColor3f(0.55f, 0.74f, 0.55f);
    rectangle(25, 100, 220, 390);


    glColor3f(0.46f, 0.54f, 0.46f);
    rectangle(25, 100, 220, 130);
    rectangle(240, 100, 980, 130);


    glColor3f(1.0f, 1.0f, 1.0f);
    text(75, 120, GLUT_BITMAP_HELVETICA_12, "STUDENT INFO");
    text(535, 120, GLUT_BITMAP_HELVETICA_12, "COURSE SCHEDULE");

    glColor3f(0.0f, 0.0f, 0.0f);
    text(45, 165, GLUT_BITMAP_HELVETICA_12, "Enrollment Status : ENROLLED");
    text(45, 185, GLUT_BITMAP_HELVETICA_12, "Student # : 202412344");
    text(45, 220, GLUT_BITMAP_HELVETICA_12, "Name : GRAVIDEZ, LOUIE EMANUEL BALICUTCHA");
    text(45, 240, GLUT_BITMAP_HELVETICA_12, "Phone : +639287515775");
    text(45, 275, GLUT_BITMAP_HELVETICA_12, "College : COMPUTER STUDIES");
    text(45, 310, GLUT_BITMAP_HELVETICA_12, "Program : BSCSSE");
    text(45, 330, GLUT_BITMAP_HELVETICA_12, "Address : 41 Zambales St Cubao Quezon City");


    glColor3f(0.55f, 0.74f, 0.55f);
    rectangle(240, 130, 980, 160);

    glColor3f(0.0f, 0.0f, 0.0f);
    text(250, 150, GLUT_BITMAP_HELVETICA_12, "Course");
    text(315, 150, GLUT_BITMAP_HELVETICA_12, "Title");
    text(605, 150, GLUT_BITMAP_HELVETICA_12, "Section");
    text(675, 150, GLUT_BITMAP_HELVETICA_12, "Units");
    text(720, 150, GLUT_BITMAP_HELVETICA_12, "Days");
    text(775, 150, GLUT_BITMAP_HELVETICA_12, "Time");
    text(925, 150, GLUT_BITMAP_HELVETICA_12, "Room");


    text(250, 185, GLUT_BITMAP_HELVETICA_12, "CS0011");
    text(315, 185, GLUT_BITMAP_HELVETICA_12, "MOBILE PROGRAMMING");
    text(610, 185, GLUT_BITMAP_HELVETICA_12, "TN35"); text(680, 185, GLUT_BITMAP_HELVETICA_12, "3");
    text(720, 185, GLUT_BITMAP_HELVETICA_12, "M / W"); text(775, 185, GLUT_BITMAP_HELVETICA_12, "11:00 - 12:50");
    text(925, 185, GLUT_BITMAP_HELVETICA_12, "F608");

    text(250, 220, GLUT_BITMAP_HELVETICA_12, "CS0016");
    text(315, 220, GLUT_BITMAP_HELVETICA_12, "NETWORK AND COMMUNICATIONS 2A");
    text(610, 220, GLUT_BITMAP_HELVETICA_12, "TN35"); text(680, 220, GLUT_BITMAP_HELVETICA_12, "3");
    text(720, 220, GLUT_BITMAP_HELVETICA_12, "T / W"); text(775, 220, GLUT_BITMAP_HELVETICA_12, "17:00 - 18:50");
    text(925, 220, GLUT_BITMAP_HELVETICA_12, "ONLINE");

    text(250, 255, GLUT_BITMAP_HELVETICA_12, "CS0019");
    text(315, 255, GLUT_BITMAP_HELVETICA_12, "MODELING AND SIMULATION");
    text(610, 255, GLUT_BITMAP_HELVETICA_12, "TN35"); text(680, 255, GLUT_BITMAP_HELVETICA_12, "3");
    text(720, 255, GLUT_BITMAP_HELVETICA_12, "F / T"); text(775, 255, GLUT_BITMAP_HELVETICA_12, "11:00 - 12:50");
    text(925, 255, GLUT_BITMAP_HELVETICA_12, "ONLINE");

    text(250, 290, GLUT_BITMAP_HELVETICA_12, "CS0025");
    text(315, 290, GLUT_BITMAP_HELVETICA_12, "SOFTWARE ENGINEERING 1");
    text(610, 290, GLUT_BITMAP_HELVETICA_12, "TN35"); text(680, 290, GLUT_BITMAP_HELVETICA_12, "3");
    text(720, 290, GLUT_BITMAP_HELVETICA_12, "F / TH"); text(775, 290, GLUT_BITMAP_HELVETICA_12, "15:00 - 16:50");
    text(925, 290, GLUT_BITMAP_HELVETICA_12, "E609");

    text(250, 325, GLUT_BITMAP_HELVETICA_12, "CS0045");
    text(315, 325, GLUT_BITMAP_HELVETICA_12, "COMPUTER GRAPHICS AND VISUAL COMPUTING");
    text(610, 325, GLUT_BITMAP_HELVETICA_12, "TN35"); text(680, 325, GLUT_BITMAP_HELVETICA_12, "3");
    text(720, 325, GLUT_BITMAP_HELVETICA_12, "M / TH"); text(775, 325, GLUT_BITMAP_HELVETICA_12, "13:00 - 14:50");
    text(925, 325, GLUT_BITMAP_HELVETICA_12, "E601");

    text(250, 360, GLUT_BITMAP_HELVETICA_12, "CS0053");
    text(315, 360, GLUT_BITMAP_HELVETICA_12, "PROGRAMMING TOOLS AND TECHNIQUES");
    text(610, 360, GLUT_BITMAP_HELVETICA_12, "TN35"); text(680, 360, GLUT_BITMAP_HELVETICA_12, "3");
    text(720, 360, GLUT_BITMAP_HELVETICA_12, "M / TH"); text(775, 360, GLUT_BITMAP_HELVETICA_12, "07:00 - 08:50");
    text(925, 360, GLUT_BITMAP_HELVETICA_12, "F702");


    glColor3f(0.46f, 0.54f, 0.46f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(240, 100); glVertex2f(980, 100);
    glVertex2f(980, 405); glVertex2f(240, 405);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(240, 160); glVertex2f(980, 160);
    glVertex2f(240, 195); glVertex2f(980, 195);
    glVertex2f(240, 230); glVertex2f(980, 230);
    glVertex2f(240, 265); glVertex2f(980, 265);
    glVertex2f(240, 300); glVertex2f(980, 300);
    glVertex2f(240, 335); glVertex2f(980, 335);
    glVertex2f(240, 370); glVertex2f(980, 370);
    glEnd();

    glColor3f(0.46f, 0.54f, 0.46f);
    text(250, 395, GLUT_BITMAP_HELVETICA_12, "TOTAL UNITS: 18");

    glFlush();
}

void init() {
    glClearColor(0.98f, 0.98f, 0.94f, 1.0f);
    gluOrtho2D(0, 1000, 500, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1000, 500);
    glutCreateWindow("Student's Subject Schedule");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}