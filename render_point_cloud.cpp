// Paul R. Cannon
// paul_r_cannon@yahoo.com
// 240-344-6081

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <iostream>


// for convenience
using namespace std;


// prototypes
void myInit(void);
void drawDisplay(void);
void resizeDisplay(int width, int height);
void mouseMotion(int mouseX, int mouseY);
void mouseFunction(int button, int state, int mouseX, int mouseY);
bool loadPoints(istream &is);


// user defined types

struct Coords3D {
    double x;
    double y;
    double z;
    Coords3D(void){}
    Coords3D(double x, double y, double z): x(x), y(y), z(z){}
};

struct ColorRGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    ColorRGB(void){}
    ColorRGB(uint8_t r, uint8_t g, uint8_t b): r(r), g(g), b(b){}
};

struct Point
{
    Coords3D coords;
    ColorRGB color;
    Point(void){}
    Point(const Coords3D &coords, const ColorRGB &color): coords(coords), color(color){}
    Point(float x, float y, float z, int r, int g, int b): coords(x, y, z), color(r, g, b){}
};


// globals

// tunable parameters
double _rotationMotionScale = 0.25;
double _translationMotionScale = 1.0;
double _wheelTranslationIncrement = 10;
double _pointSize = 1.0;
bool   _drawCoordinateAxes = true;
double _coordinateAxesLength = 50;

// state for callbacks
vector<Point> _points;
Coords3D _translation;
Coords3D _rotation;
int _savedMouseX;
int _savedMouseY;
int _savedMouseButton;


int main(int argc, char *argv[])
{
    // initialize GLUT
    glutInit(&argc, argv);
    myInit();

    // check usage
    if (argc != 2)
    {
        cerr << "Usage: render_point_cloud <points_file>" << endl;
        return 1;
    }

    // try to load in the points from the points file
    cout << "try to open file" << endl;
    fstream fin(argv[1], fstream::in);
    if (!fin)
    {
        cerr << "error: couldn't open file \"" << argv[1] << "\" for input; exiting...";
        return 1;
    }
    cout << "file opened" << endl;
    if (!loadPoints(fin))
    {
        cerr << "error: problem loading points from \"" << argv[1] << "\"; exiting...";
        return 1;
    }
    fin.close();
    cerr << _points.size() << " points successfully loaded" << endl;

//    // output some points for verification
//    cout.setf(ios_base::fixed);
//    cout.precision(6);
//    cout.setf(ios_base::showpoint);
//    for (int point_i = _points.size() - 2; point_i < _points.size(); point_i++)
//    {
//        cout << setw(11) << _points[point_i].coords.x << ' ';
//        cout << setw(11) << _points[point_i].coords.y << ' ';
//        cout << setw(11) << _points[point_i].coords.z << ' ';
//        cout << setw(3) << static_cast<unsigned>(_points[point_i].color.r) << ' ';
//        cout << setw(3) << static_cast<unsigned>(_points[point_i].color.g) << ' ';
//        cout << setw(3) << static_cast<unsigned>(_points[point_i].color.b) << ' ';
//        cout << endl;
//    }

    // start GLUT event loop
    glutMainLoop();

    return 0;
}


void myInit(void)
{
    // set display mode
    // GLUT_RGB: RGBframebuffer
    // GLUT_DOUBLE: double-buffered mode
    // GLUT_DEPTH: depth buffering
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    // create display window
    glutInitWindowSize(640, 480);
    glutCreateWindow("Point Cloud Display Window");

    // register callbacks
    glutDisplayFunc(drawDisplay);
    glutReshapeFunc(resizeDisplay);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseFunction);

    // set background color to black
    glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
}


void drawDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // apply current translation and rotate
    glLoadIdentity();
    glTranslatef(_translation.x, _translation.y, _translation.z);
    glRotatef( (int) _rotation.x, 1, 0, 0 );
    glRotatef( (int) _rotation.y, 0, 1, 0 );
    glRotatef( (int) _rotation.z, 0, 0, 1 );

    // set point size
    glPointSize(_pointSize);

    // enable vertex and color array client states
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );

    // set vertex and color pointers
    glVertexPointer(3, GL_DOUBLE, sizeof(Point), _points.data());
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(Point), reinterpret_cast<uint8_t *>(_points.data()) + sizeof(Coords3D));

    // draw point cloud
    glDrawArrays(GL_POINTS, 0, _points.size());

    // disable vertex and color array client states
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    // draw coordinate axes
    if (_drawCoordinateAxes)
    {
        // set color to green
        glColor4f(0.0F, 1.0F, 0.0F, 0.0F);

        // draw axis labels
        glRasterPos3f(_coordinateAxesLength, 0.0F, 0.0F);
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'X');
        glRasterPos3f(0.0F, _coordinateAxesLength, 0.0F);
        glutBitmapCharacter( GLUT_BITMAP_8_BY_13, 'Y');
        glRasterPos3f(0.0F, 0.0F, _coordinateAxesLength);
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'Z');

        // draw axes
        glBegin(GL_LINES);
        glVertex3i(0, 0, 0);
        glVertex3i(_coordinateAxesLength, 0, 0);
        glEnd();
        glBegin(GL_LINES);
        glVertex3i(0, 0, 0);
        glVertex3i(0, _coordinateAxesLength, 0);
        glEnd();
        glBegin(GL_LINES);
        glVertex3i(0, 0, 0);
        glVertex3i(0, 0, _coordinateAxesLength);
        glEnd();
    }

    glutSwapBuffers();
}


void resizeDisplay(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, static_cast<double>(width)/height, 0, 200);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}


void mouseMotion(int mouseX, int mouseY)
{
    if (_savedMouseButton == GLUT_LEFT_BUTTON)
    {
            _rotation.x -= _rotationMotionScale*(mouseY - _savedMouseY);
            _rotation.y -= _rotationMotionScale*(mouseX - _savedMouseX );
            glutPostRedisplay();
    }
    else if (_savedMouseButton == GLUT_MIDDLE_BUTTON)
    {
            _rotation.x -= _rotationMotionScale*(mouseY - _savedMouseY);
            _rotation.z -= _rotationMotionScale*(mouseX - _savedMouseX);
            glutPostRedisplay();
    }
    else if ( _savedMouseButton == GLUT_RIGHT_BUTTON )
    {
        _translation.y -= _translationMotionScale*(mouseY - _savedMouseY);
        _translation.x += _translationMotionScale*(mouseX - _savedMouseX);
        glutPostRedisplay();
    }
    _savedMouseX = mouseX;
    _savedMouseY = mouseY;
}


void mouseFunction(int button, int state, int mouseX, int mouseY)
{
    if (state == GLUT_DOWN)
    {
        if (button == GLUT_LEFT_BUTTON || button == GLUT_MIDDLE_BUTTON || button == GLUT_RIGHT_BUTTON)
        {
            _savedMouseButton = button;
            _savedMouseX = mouseX;
            _savedMouseY = mouseY;
        }
        else if (button == 3) // move mouse wheel forward
        {
            _translation.z += _wheelTranslationIncrement;
            glutPostRedisplay();
        }
        else if (button == 4) // move mouse wheel backward
        {
            _translation.z -= _wheelTranslationIncrement;
            glutPostRedisplay();
        }
    }
}


bool loadPoints(istream &is)
{
    string line;
    float x, y, z;
    int r, g, b;
    int npoints = 0;

    // first pass: verify data and count points

    // read a line from standard input
    getline(is, line);

    // while a line was successfully read in
    while (is)
    {
        // we have line; try to read the data for a point
        stringstream ss(line);
        ss >> x >> y >> z >> r >> g >> b;
        if (!ss)
        {
            // line is incomplete; assume that we are done
            break;
        }

        // we have a complete set of data; verify data integrity
        if (!isfinite(x) || !isfinite(y) || !isfinite(x))
        {
            cerr << "error: x, y, or z not finite on line " << npoints + 1 << endl;
            return false;
        }
        if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
        {
            cerr << "error: r, b, or g not within [0, 255]" << endl;
            return false;
        }

        // we have a valid point; count it
        npoints++;

        // read a line from standard input
        getline(is, line);
    }

    // allocate space for points
    _points.resize(npoints);

    // reset the stream
    is.clear();
    is.seekg(0);

    // second pass: read in the points
    for (int point_i = 0; point_i < npoints; point_i++)
    {
        // read a line from the input stream
        getline(is, line);

        // read the data for a point
        stringstream ss(line);
        ss >> x >> y >> z >> r >> g >> b;

        // assign the point
        _points[point_i] = Point(x, y, -z, r, g, b);
    }

    // report success
    return true;
}
