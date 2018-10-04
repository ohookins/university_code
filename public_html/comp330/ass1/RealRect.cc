#include <GL/glut.h>
/* RealRect code taken from "Computer Graphics Using OpenGL" textbook */

class RealRect
{
public:
    RealRect() {l = 0; r = 100; b = 0; t = 100;} //constructors
    RealRect(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top)
      {l = left; r = right; b = bottom; t = top;}
    void set(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top)
      {l = left; r = right; b = bottom; t = top;}
    void draw(void); // draw this rectangle using OpenGL
    float getL(){return l;}
    float getR(){return r;}
    float getB(){return b;}
    float getT(){return t;}

private:
    GLfloat l, r, b, t;

};
    
