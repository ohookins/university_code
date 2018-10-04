#include <GL/glut.h>
#include "IntRect.cc"
#include "RealRect.cc"
#include "Point2.cc"

class Canvas {		//from textbook
public:
    Canvas(int width, int height, char* windowTitle); //constructor
    void setWindow(GLfloat l, GLfloat r, GLfloat b, GLfloat t);
    void setViewport(GLint l, GLint r, GLint b, GLint t);
    IntRect getViewport(void);  //divulge the viewport data
    RealRect getWindow(void); //divulge the window data
    GLfloat getWindowAspectRatio(void);
    void clearScreen();
    void setBackgroundColor(GLfloat r, GLfloat g, GLfloat b);
    void setColor(GLfloat r, GLfloat g, GLfloat b);
    void lineTo(GLfloat x, GLfloat y);
    void lineTo(Point2 p);
    void moveTo(GLfloat x, GLfloat y);
    void moveTo(Point2 p);
    void moveRel(float dx, float dy);
    void lineRel(float dx, float dy);
    void filledTri(float cx, float cy, float radius);
    void filledSqr(float cx, float cy, float radius);
    void filledCir(float cx, float cy, float radius);

private:
    Point2 CP;		// current position in the world
    IntRect viewport;	// the current window
    RealRect window;	// the current viewport

};
