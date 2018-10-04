#ifndef _RGBPIXMAP
#define _RGBPIXMAP
#include <GL/glut.h>
#include <fstream>
#include <assert.h>

//helper classes, IntPoint, IntRect
class IntPoint
{
public:
  int x,y;
};

class IntRect
{
public:
    IntRect() {l=0; r=100; b=0; t=100;}
    IntRect(int left, int right, int bottom, int top)
      {l = left; r = right; b = bottom; t = top;}
    void set(int left, int right, int bottom, int top)
      {l = left; r = right; b = bottom; t = top;}
    void draw(void);
    int l,r,b,t;
};

typedef unsigned char uchar;

class mRGB{
public: uchar r,g,b;
    mRGB(){r=g=b=0;}
    mRGB(mRGB& p){r = p.r; g = p.g; b= p.b;}
    mRGB(uchar rr, uchar gg, uchar bb){r = rr; g = gg; b = bb;}
    void set(uchar rr, uchar gg, uchar bb){r = rr; g = gg; b = bb;}
};

//RGBPixmap class
class RGBpixmap{
private:
    mRGB* pixel;

public:
    int nRows, nCols;
    RGBpixmap() {nRows = nCols = 0; pixel = 0;}
    RGBpixmap(int rows, int cols)
    {
      nRows = rows;
      nCols = cols;
      pixel = new mRGB[rows*cols];
    }

    int readBMPFile(char * fname);
    void makeCheckerboard();
    void setTexture(GLuint textureName);

    void freeIt()
    {
      delete []pixel; nRows = nCols = 0;
    }

// copy
void copy(IntPoint from, IntPoint to, int x, int y, int width, int height)
// copy a region of the display back onto the display
{
    if(nRows == 0 || nCols == 0) return;
    glCopyPixels(x,y,width,height,GL_COLOR);
}

// draw
void draw() // draw the pix map at current raster position
{
    if(nRows == 0 || nCols == 0) return;
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glDrawPixels(nCols, nRows, GL_RGB, GL_UNSIGNED_BYTE, pixel);
}

// read
int read(int x, int y, int wid, int ht)
{
// read a rectangle of pixels into this pixmap
    nRows = ht;
    nCols = wid;
    pixel = new mRGB[nRows *nCols]; if(!pixel) return -1;
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    glReadPixels(x,y, nCols, nRows, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    return 0;
}

// read from IntRect
int read(IntRect r)
{
// read a rectangle of pixels into this pixmap
    nRows = r.t - r.b;
    nCols = r.r - r.l;
    pixel = new mRGB[nRows * nCols]; if(!pixel) return -1;
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    glReadPixels(r.l,r.b,nCols,nRows,GL_RGB,GL_UNSIGNED_BYTE,pixel);
    return 0;
}

// setpixel
void setPixel(int x, int y, mRGB color)
{
    if(x>=0 && x < nCols && y >=0 && y < nRows)
      pixel[nCols * y + x] = color;
}

// getpixel
mRGB getPixel(int x, int y)
{
    mRGB bad(255,255,255);
    assert(x >=0 && x < nCols);
    assert(y >=0 && y < nRows);
    return pixel[nCols * y + x];
}

};
// end of RGBpixmap class
#endif
