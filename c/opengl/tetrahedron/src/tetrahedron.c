#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Clears the window and draws the tetrahedron.
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a white grid.
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);

    for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
        glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
        glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
    }

    glEnd();

    // Draw the tetrahedron.
    glBegin(GL_TRIANGLE_STRIP);

        glColor3f(1, 1, 1); glVertex3f(0, 2, 0);
        glColor3f(1, 0, 0); glVertex3f(-1, 0, 1);
        glColor3f(0, 1, 0); glVertex3f(1, 0, 1);
        glColor3f(0, 0, 1); glVertex3f(0, 0, -1.4);
        glColor3f(1, 1, 1); glVertex3f(0, 2, 0);
        glColor3f(1, 0, 0); glVertex3f(-1, 0, 1);

    glEnd();

    glFlush();
}

void init() {
    glClearColor(0.1, 0.39, 0.88, 1.0);
    glColor3f(1.0, 1.0, 1.0);

    // Tell the rendering engine not to draw backfaces.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Set the camera lens perspective.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-2, 2, -1.5, 1.5, 1, 40);

    // Set up transforms:
    // First, rotate 70 degrees around y
    // Then,  rotate 50 degrees around x
    // Then,  move the object back 3 units "into the screen".
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -3);
    glRotatef(50, 1, 0, 0);
    glRotatef(70, 0, 1, 0);
}

// Initializes GLUT, the display mode, and main window; registers callbacks;
// does application initialization; enters the main event loop.
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(80, 80);
    glutInitWindowSize(800, 600);
    glutCreateWindow("A Simple Tetrahedron");

    glutDisplayFunc(display);

    init();

    glutMainLoop();
}