/* Point2 class taken from "Computer Graphics Using OpenGL" textbook */

class Point2
{
public:
    Point2() {x = y = 0.0f;}	// constructor 1
    Point2(GLfloat xx, GLfloat yy) {x = xx; y = yy;} // constructor 2
    void set(GLfloat xx, GLfloat yy) {x = xx; y = yy;}
    GLfloat getX() {return x;}
    GLfloat getY() {return y;}
    void draw(void)
    {
      glBegin(GL_POINTS); // draw this point
      glVertex2f(x,y)l
      glEnd();
    }

private;
    GLfloat x, y;
};
