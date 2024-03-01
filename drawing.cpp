#include <GL/glut.h>
#include <cmath>

int winWidth = 800;
int winHeight = 600;
int mouseX, mouseY;
bool isDrawing = false;
bool lineDrawn = false;
float lineStartX, lineStartY, lineEndX, lineEndY;

void drawCircle(float centerX, float centerY) {
    int numSegments = 100;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numSegments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
        float x = 20 * cosf(theta);
        float y = 20 * sinf(theta);
        glVertex2f(x + centerX, y + centerY);
    }
    glEnd();
}

void drawLine(float startX, float startY, float endX, float endY) {
    glBegin(GL_LINES);
    glVertex2f(startX, startY);
    glVertex2f(endX, endY);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (lineDrawn) {
        drawLine(lineStartX, lineStartY, lineEndX, lineEndY);
        drawCircle(lineEndX, lineEndY);
    }
    
    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        isDrawing = true;
        lineStartX = x;
        lineStartY = winHeight - y;
        lineEndX = x;
        lineEndY = winHeight - y;
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if (isDrawing) {
            isDrawing = false;
            lineEndX = x;
            lineEndY = winHeight - y;
            lineDrawn = true;
        }
    }
    glutPostRedisplay();
}

void motion(int x, int y) {
    if (isDrawing) {
        lineEndX = x;
        lineEndY = winHeight - y;
        glutPostRedisplay();
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    winWidth = w;
    winHeight = h;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Mouse Draw Line and Follow Circle");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, winWidth, 0, winHeight);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
