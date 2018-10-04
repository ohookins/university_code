#include "RGBpixmap.h"

typedef unsigned short ushort;
typedef unsigned long ulong;
fstream inf; //global in this file for convenience

//getshort
ushort getShort() //helper function
{
    char ic;
    ushort ip;
    inf.get(ic); ip=ic;
    inf.get(ic); ip |= ((ushort)ic << 8);
    return ip;
}

//getlong
ulong getLong() //helper function
{
    ulong ip = 0;
    char ic = 0;
    unsigned char uc = ic;
    inf.get(ic); uc=ic; ip=uc;
    inf.get(ic); uc=ic; ip |=((ulong)uc << 8);
    inf.get(ic); uc=ic; ip |=((ulong)uc << 16);
    inf.get(ic); uc=ic; ip |=((ulong)uc << 24);
    return ip;
}

//RGBpixmap::readBmpFile
int RGBpixmap:: readBMPFile(char * fname)
{
    inf.open(fname, ios::in|ios::binary);
    if(!inf){ cout << " can't open file: " << fname << endl;
      return 0;}
    int k, row, col, numPadBytes, nBytesInRow;
    char ch1, ch2;
    inf.get(ch1); inf.get(ch2);
    ulong fileSize = getLong();
    ushort reserved1 = getShort();
    ushort reserved2 = getShort();
    ulong offBits = getLong();
    ulong headerSize = getLong();
    ulong numCols = getLong();
    ulong numRows = getLong();
    ushort planes = getShort();
    ushort bitsPerPixel = getShort();
    ulong compression = getLong();
    ulong imageSize = getLong();
    ulong xPels = getLong();
    ulong yPels = getLong();
    ulong numLUTentries = getLong();
    ulong impColors = getLong();

    if(bitsPerPixel != 24)
    {
      cout << "not a 24-bit pixel image, or is compressed!\n";
      inf.close(); return 0;
    }

    nBytesInRow = ((3 * numCols + 3)/4) * 4;
    numPadBytes = nBytesInRow - 3 * numCols;
    nRows = numRows;
    nCols = numCols;
    pixel = new mRGB[nRows * nCols];
    if(!pixel) return 0; //out of memory
    long count = 0;
    char dum;
    for(row = 0; row < nRows; row++)
    {
      for(col = 0; col < nCols; col++)
      {
	char r,g,b;
	inf.get(b); inf.get(g); inf.get(r); //read bytes
	pixel[count].r = r; //place them in colours
	pixel[count].g = g;
	pixel[count++].b = b;
      }
      for(k=0; k < numPadBytes; k++) //skip pad bytes at row's end
	inf >> dum;
    }
    inf.close(); return 1; //success
}

void RGBpixmap::makeCheckerboard()
{
    nRows = nCols = 64;
    pixel = new mRGB[3 * nRows * nCols];
    if(!pixel) {cout << "out of memory!"; return;}
    long count = 0;
    for(int i = 0; i < nRows; i++)
    {
      for(int j = 0; j < nCols; j++)
      {
	int c = ((i&8)^(j&8))*255;
	pixel[count].r = c;	// red
	pixel[count].g = c;	// green
	pixel[count++].b = 0;	// blue
      }
    }
}

void RGBpixmap::setTexture(GLuint textureName)
{
    glBindTexture(GL_TEXTURE_2D,textureName);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,nCols,nRows,0,GL_RGB,GL_UNSIGNED_BYTE, pixel);
}

