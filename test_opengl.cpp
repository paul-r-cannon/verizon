// Paul R. Cannon
// paul_r_cannon@yahoo.com
// 240-344-6081

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>



void myInit (void)
{
    glClearColor(1.0,1.0,1.0,0.0); // sets background color to white
    glColor3f(0.0f,0.0f,0.0f); // setsthe drawing colour
    glPointSize(4.0); // sets a point to be 4x4 pixels
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 500.0, 0.0, 400.0); // the display area in world coordinates.
}


void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT); // clears the screen
    glBegin(GL_POINTS);
    glVertex2i (100, 50);
    glVertex2i (100, 130);
    glVertex2i (150, 130);
    glEnd();
    glFlush(); // sends all output to display;
}


int main (int argc, char **argv)
{
    glutInit(&argc, argv); // to initialize the toolkit;
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // sets the display mode
    glutInitWindowSize (640, 480); // sets the window size
    glutInitWindowPosition (10, 10); // sets the starting position for the window
    glutCreateWindow ("OpenGL Display Window"); // creates the window and sets the title
    glutDisplayFunc (myDisplay);

    myInit(); // additional initializations as necessary

    glutMainLoop(); // go into a loop until event occurs

    return 0;
}
