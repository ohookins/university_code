#include <GL/glut.h>
#include "Point2.cc"
#include "IntRect.cc"
#include "RealRect.cc"
#include <math.h>

class Canvas
{

public:
    Canvas(int width, int height, char* windowTitle); //constructor
    void setWindow(float l, float r, float b, float t);
    void setViewport(int l, int r, int b, int t);
    IntRect getViewport(void);  //divulge the viewport data
    RealRect getWindow(void); //divulge the window data
    float getWindowAspectRatio(void);
    void clearScreen();
    void setBackgroundColor(float r, float g, float b);
    void setColor(float r, float g, float b);
    void lineTo(float x, float y);
    void lineTo(Point2 p);
    void moveTo(float x, float y);
    void moveTo(Point2 p);
    void moveRel(float dx, float dy);
    void lineRel(float dx, float dy);
    void filledTri(float cx, float cy, float radius);
    void filledSqr(float cx, float cy, float radius);
    void filledCir(float cx, float cy, float radius);
    void arrowLine(float x1, float y1, float x2, float y2, int arrows);

private:
    Point2 CP;		// current position in the world
    IntRect viewport;	// the current window
    RealRect window;	// the current viewport

};

/********************Canvas Constructor******************/
Canvas:: Canvas(int width, int height, char* windowTitle)
{
    char* argv[1];	//dummy argument list for glutInit()
    char dummyString[8];
    argv[0] = dummyString; // hook up the pointer
    int argc = 1;	// to satisfy glutInit()

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(20,20);
    glutCreateWindow(windowTitle); //open the screen window
    setWindow(0, (float)width, 0, (float)height); //default world window
    setViewport(0, width, 0, height); //default viewport
    CP.set(0.0f, 0.0f);	//initialise the CP to (0,0)
}

void Canvas:: clearScreen()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Canvas:: setBackgroundColor(float r, float g, float b)
{
    glClearColor(r, g, b, 0.0);
}

void Canvas:: setColor(float r, float g, float b)
{
    glColor3f(r, g, b);
}

float Canvas:: getWindowAspectRatio(void)
{
    return((window.getR()-window.getL())/(window.getT()-window.getB()));
}

/****************** moveTo ******************************/
void Canvas:: moveTo(float x, float y)
{
    CP.set(x, y);
}

void Canvas:: moveTo(Point2 p)
{
    CP.set((p.getX()),(p.getY()));
}

/******************* lineTo ****************************/
void Canvas:: lineTo (float x, float y)
{
    glBegin(GL_LINES);
      glVertex2f((GLfloat) CP.getX(), (GLfloat) CP.getY());
      glVertex2f((GLfloat) x, (GLfloat) y);	// draw the line
    glEnd();
    CP.set(x,y);	// update the CP
    glFlush();
}

void Canvas:: lineTo (Point2 p)
{
    glBegin(GL_LINES);
      glVertex2f((GLfloat) CP.getX(), (GLfloat) CP.getY());
      glVertex2f((GLfloat) p.getX(), (GLfloat) p.getY()); //draw the line
    glEnd();
    CP.set(p.getX(),p.getY());  //update the CP
    glFlush();
}

/******************* set Window *************************/
void Canvas:: setWindow(float l,  float r, float b, float t)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D((GLdouble)l, (GLdouble)r, (GLdouble)b, (GLdouble)t);
    window.set (l,r,b,t);
}

/******************** set Viewport **********************/
void Canvas:: setViewport(GLint l, GLint r, GLint b, GLint t)
{
    glViewport(l, b, r-l, t-b);
}

void Canvas:: moveRel(float dx, float dy)
{
    CP.set(CP.getX() + dx, CP.getY() + dy);
}

void Canvas:: lineRel(float dx, float dy)
{
    float x = CP.getX() + dx, y = CP.getY() + dy;
    lineTo(x,y);
    CP.set(x,y);
}

/******************** filled shapes **********************/
void Canvas:: filledTri(float cx, float cy, float radius)
{
    CP.set(cx,cy);
    moveRel(0,radius);
    glBegin(GL_POLYGON);
      glVertex2f(CP.getX(),CP.getY());
      moveRel(radius, -2*radius);
      glVertex2f(CP.getX(),CP.getY());
      moveRel(-2*radius, 0);
      glVertex2f(CP.getX(),CP.getY());
    glEnd();
    CP.set(cx,cy);
    glFlush();
}

void Canvas:: filledCir(float cx, float cy, float radius)
{
    float pi = 3.14159265;
    CP.set(cx,cy);
    moveRel(0,radius);
    double angle = 0;
    double angleInc = (2*pi)/100;
    glBegin(GL_POLYGON);
      glVertex2f(CP.getX(),CP.getY());
      for(int k = 0; k < 101; k++)
      {
	angle += angleInc;
	moveTo(radius* cos(angle) + cx, radius * sin(angle) + cy);
        glVertex2f(CP.getX(),CP.getY());
      }
    glEnd();
    CP.set(cx,cy);
    glFlush();
}

void Canvas:: filledSqr(float cx, float cy, float radius)
{
    CP.set(cx,cy);
    moveRel(radius,radius);
    glBegin(GL_POLYGON);
      glVertex2f(CP.getX(),CP.getY());
      moveRel(0,-2*radius);
      glVertex2f(CP.getX(),CP.getY());
      moveRel(-2*radius,0);
      glVertex2f(CP.getX(),CP.getY());
      moveRel(0,2*radius);
      glVertex2f(CP.getX(),CP.getY());
    glEnd();
    CP.set(cx,cy);
    glFlush();
}

void Canvas:: arrowLine(float x1, float y1, float x2, float y2, int arrows)
{
    moveTo(x1,y1);
    float gradient = (y2-y1)/(x2-x1);
    float x3,y3;
    float vert,hori;
   // hori = sqr(25 - ((hori*gradient)*(hori*gradient)));
}
