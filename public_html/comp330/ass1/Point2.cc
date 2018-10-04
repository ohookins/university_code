#include <GL/glut.h>

/* Point2 class taken from "Computer Graphics Using OpenGL" textbook */

class Point2
{
public:
    Point2() {x = y = 0.0f;}	// constructor 1
    Point2(float xx, float yy) {x = xx; y = yy;} // constructor 2
    void set(float xx, float yy) {x = xx; y = yy;}
    GLfloat getX() {return x;}
    GLfloat getY() {return y;}
    void draw(void)
    {
      glBegin(GL_POINTS); // draw this point
      glVertex2f(x,y);
      glEnd();
    }

private:
    float x, y;
};
