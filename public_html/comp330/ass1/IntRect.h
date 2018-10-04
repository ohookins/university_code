/* IntRect code taken from "Computer Graphics Using OpenGL" textbook */

class IntRect
{
public:
    IntRect() {l = 0; r = 100; b = 0; t = 100;) //constructors
    IntRect(GLint left, GLint right, GLint bottom, GLint top)
      {l = left; r = right; b = bottom; t = top;}
    void set(GLint left, GLint right, GLint bottom, GLint top)
      {l = left; r = right; b = bottom; t = top;}
    void draw(void); // draw this rectangle using OpenGL

private:
    GLint l, r, b, t;

};
    
