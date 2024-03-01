//---------------------------------------
// Program: main.cpp
// Project: 1
// Author:  Matthew Butler
// Date:    2/12/2024
//---------------------------------------
#include <iostream>
#include <vector>
#include <cmath>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

// Define a struct to hold x and y coordinates
struct Point {
    float x, y;
};

// Define a vector to hold all of the points
vector<Point> points;

// Flag to determine if the user is drawing or not
bool isDrawing = false;

// Current point index for drawing
int currentPointIndex = 0;

//---------------------------------------
// Function to draw a circle
//---------------------------------------
void drawCircle(Point center, float radius) {
    const int num_segments = 100;
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float dx = radius * cos(theta);
        float dy = radius * sin(theta);
        glVertex2f(dx + center.x, dy + center.y);
    
    }
    glEnd();
}

//---------------------------------------
// Function to draw a line
//---------------------------------------
void drawLine(Point p1, Point p2) {
    glColor3f(1.0, 0.0, 0.0); // Set the color to red
    glBegin(GL_LINES);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glEnd();
}

//---------------------------------------
// Timer callback function
//---------------------------------------
void timer(int extra) {
    if (currentPointIndex < points.size()) {
        glutPostRedisplay();
        glutTimerFunc(1000, timer, 0);
    }
}
//---------------------------------------
// Keyboard Callback
//---------------------------------------
void keyboard(unsigned char key, int x, int y) {
    // if the user presses the e key, print all points to the console
    if (key == 'e') {
        for (int i = 0; i < points.size(); i++) {
            cout << "Point " << i << ": (" << points[i].x << ", " << points[i].y << ")" << endl;
        }
    }
}

//---------------------------------------
// Mouse callback function
//---------------------------------------
void mouse(int button, int state, int x, int y) {
    // When the left mouse button is pressed, start drawing and saving points
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        isDrawing = true;
        Point p;

        // x coordinate is normalized to the range [-1, 1]
        p.x = (float)x / (float)glutGet(GLUT_WINDOW_WIDTH) * 2 - 1;

        // y coordinate is normalized to the range [-1, 1]
        p.y = (float)(glutGet(GLUT_WINDOW_HEIGHT) - y) / (float)glutGet(GLUT_WINDOW_HEIGHT) * 2 - 1;

        // Clear the points vector before adding any more points
        points.clear();

        // Add the point to the vector
        points.push_back(p);
    }

    // When the left mouse button is released, stop drawing
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        isDrawing = false;
        currentPointIndex = 0;
        glutTimerFunc(0, timer, 0);
        Point p;

        // x coordinate is normalized to the range [-1, 1]
        p.x = (float)x / (float)glutGet(GLUT_WINDOW_WIDTH) * 2 - 1;

        // y coordinate is normalized to the range [-1, 1]
        p.y = (float)(glutGet(GLUT_WINDOW_HEIGHT) - y) / (float)glutGet(GLUT_WINDOW_HEIGHT) * 2 - 1;

        // Add the point to the vector
        points.push_back(p);

        glutPostRedisplay();
    }
}

//---------------------------------------
// Mouse motion callback function
//---------------------------------------
void mouseMotion(int x, int y) {
    // If the user is drawing, update the current point
    if (isDrawing) {
        Point p;

        // x coordinate is normalized to the range [-1, 1]
        p.x = (float)x / (float)glutGet(GLUT_WINDOW_WIDTH) * 2 - 1;

        // y coordinate is normalized to the range [-1, 1]
        p.y = (float)(glutGet(GLUT_WINDOW_HEIGHT) - y) / (float)glutGet(GLUT_WINDOW_HEIGHT) * 2 - 1;

        // Update the last point in the vector
        if (!points.empty()) {
            points.back() = p;
        }
    }

    glutPostRedisplay();
}

//---------------------------------------
// Display callback function
//---------------------------------------
void display() {
    // Clear the window
    glClearColor(0.5f, 0.5f, 1.0f, 1.0f); // Set the clear color to blue
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the flight path
    for (size_t i = 1; i < points.size(); i++) {
        drawLine(points[i - 1], points[i]);
    }

    // Draw a circle at the current point
    if (currentPointIndex < points.size()) {
        drawCircle(points[currentPointIndex], 0.05f);
        currentPointIndex++;
    }

    glutSwapBuffers();

    cout << "Display Function Called" << endl;
}

//---------------------------------------
// Init function
//---------------------------------------
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);    
    glOrtho(-w/500.0, w/500.0, -h/500.0, h/500.0, -1, 1);
    glEnable(GL_DEPTH_TEST);
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[]) {
    // Initialize OpenGL
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Freehand Drawing");

    // Set the display callback
    glutDisplayFunc(display);

    // Set the mouse callback
    glutMouseFunc(mouse);

    // Set the mouse motion callback
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);

    init();

    cout << ("Press 'e' to print all points to the console\n") << endl;

    // Main loop
    glutMainLoop();
    return 0;
}