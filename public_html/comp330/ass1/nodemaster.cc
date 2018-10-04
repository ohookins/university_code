#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <list.h>
#include "Node.h"
#include "Canvas.h"

list<Node> _NodeList;
int ScreenWidth = 800;
int ScreenHeight = 600;
float _WidthRatio = 1.0;
float _HeightRatio = 1.0;
int _Mode = 0;
int _MenuMain;	// top level menu
Canvas cvs(800,600,"NodeMaster"); // the canvas, default size 800x600
int _CurrentShape; //current shape for purposes of new or changing nodes
int _CurrentColor; //current color for ""	""	""	""
int _CurrentID = 0; //current node id for addition in node list
void makeNode(float X, float Y, int store);

void menuMain(int id)
{
// only use for changing a node's name - this is the only non-sub-menu
    if(id == 1)
      std::cout << "Changing node's name\n";
}

void menuColour(int id)
{
    switch(id)
    {
      case 0: {cvs.setColor(0,0,0); _CurrentColor=0;} break; //black
      case 1: {cvs.setColor(0,0,1); _CurrentColor=1;} break; //blue
      case 2: {cvs.setColor(0,1,0); _CurrentColor=2;} break; //green
      case 3: {cvs.setColor(0,1,1); _CurrentColor=3;} break; //cyan
      case 4: {cvs.setColor(1,0,0); _CurrentColor=4;} break; //red
      case 5: {cvs.setColor(1,0,1); _CurrentColor=5;} break; //magenta
      case 6: {cvs.setColor(1,1,0); _CurrentColor=6;} break; //yellow
      default: {cvs.setColor(0,0,0); _CurrentColor=0;} break; //black again
    }
}

void menuShape(int id)
{
    switch(id)
    {
      case 1: _CurrentShape = 1; break; //no. of sides. therefore circle
      case 3: _CurrentShape = 3; break; //triangle
      case 4: _CurrentShape = 4; break; //square
      default: _CurrentShape = 1; break; //circle again
    }
}

void menuLines(int id)
{
    switch(id)
    {
      case 0: std::cout << "Non-directional" << endl; break;
      case 1: std::cout << "Uni-directional" << endl; break;
      case 2: std::cout << "Bi-directional" << endl; break;
    }
}

void modeChange(int newmode)
{
    _Mode = newmode;
    if(_Mode == 0)
    {
      if(_MenuMain != 0) glutDestroyMenu(_MenuMain);
      //destroys any previous menu, in preparation for new one

      _MenuMain = glutCreateMenu(menuMain);		// menu headings
      int _MenuColour = glutCreateMenu(menuColour);
      int _MenuShape = glutCreateMenu(menuShape);

      glutSetMenu(_MenuColour);		//setting up Colour submenu
      glutAddMenuEntry("Black",0);
      glutAddMenuEntry("Blue",1);
      glutAddMenuEntry("Green",2);
      glutAddMenuEntry("Cyan",3);
      glutAddMenuEntry("Red",4);
      glutAddMenuEntry("Magenta",5);
      glutAddMenuEntry("Yellow",6);

      glutSetMenu(_MenuShape);		//setting up Shape submenu
      glutAddMenuEntry("Circle",1);
      glutAddMenuEntry("Triangle",3);
      glutAddMenuEntry("Square",4);

      glutSetMenu(_MenuMain);	//binding submenus to main menu
      glutAddSubMenu("Colour", _MenuColour);
      glutAddSubMenu("Shape", _MenuShape);
    }
    else if(_Mode == 1) // change node properties. same as new node essentially
    {
      glutDestroyMenu(_MenuMain);
      _MenuMain = glutCreateMenu(menuMain);
      int _MenuColour = glutCreateMenu(menuColour);
      int _MenuShape = glutCreateMenu(menuShape);

      glutSetMenu(_MenuColour);		//setting up Colour submenu
      glutAddMenuEntry("Black",0);
      glutAddMenuEntry("Blue",1);
      glutAddMenuEntry("Green",2);
      glutAddMenuEntry("Cyan",3);
      glutAddMenuEntry("Red",4);
      glutAddMenuEntry("Magenta",5);
      glutAddMenuEntry("Yellow",6);

      glutSetMenu(_MenuShape);		//setting up Shape submenu
      glutAddMenuEntry("Circle",1);
      glutAddMenuEntry("Triangle",3);
      glutAddMenuEntry("Square",4);

      glutSetMenu(_MenuMain);	//binding submenus to main menu
      glutAddSubMenu("Colour", _MenuColour);
      glutAddSubMenu("Shape", _MenuShape);
      glutAddMenuEntry("Change name",1);  // for changing the name of a node
    }
    else if(_Mode == 2) // connect nodes. has arrow number selection.
    {
      glutDestroyMenu(_MenuMain);
      _MenuMain = glutCreateMenu(menuLines);
      glutAddMenuEntry("Non-directional", 0);
      glutAddMenuEntry("Uni-directional", 1);
      glutAddMenuEntry("Bi-directional", 2);
    }
    else if(_Mode == 3) //delete node. doesn't really require a menu
    {
      glutDestroyMenu(_MenuMain);
      _MenuMain = glutCreateMenu(menuMain);
    }
}

void redraw()
{
// Redrawing the entire contents of the list in case of screen damage etc.
    int tempcolor = _CurrentColor; // these need to be saved before
    int tempshape = _CurrentShape; // redrawing, and then restored
				   // to preserve menu selection.
    list<Node>::iterator it;
    it = _NodeList.begin();
    for( int i = 0; i < _NodeList.size(); i++)
    {
      menuColour((*it).Colour);
      menuShape((*it).Shape);
      makeNode((*it).X,(*it).Y,0);
      it++;
    }
    //processing nodes first, then lines.

    menuColour(tempcolor);
    menuShape(tempshape);
}
    
void makeNode(float X, float Y, int store)
/* Drawing the new node to the screen and adding it to the list
   if the 'store' variable is 1. If 0, just draw it to screen */
{
    switch(_CurrentShape)
    {
      case 1:	// draw a circle, and push it onto back of list.
      {
	cvs.filledCir(X,Y,5);
	Node n(++_CurrentID,_CurrentShape,_CurrentColor,"",X,Y);
	if(store) _NodeList.push_back(n);
      } break;

      case 3:	// draw a triangle etc.
      {
	cvs.filledTri(X,Y,5);
	Node n(++_CurrentID,_CurrentShape,_CurrentColor,"",X,Y);
	if(store) _NodeList.push_back(n);
      } break;

      case 4:	// draw a square etc.
      {
	cvs.filledSqr(X,Y,5);
	Node n(++_CurrentID,_CurrentShape,_CurrentColor,"",X,Y);
	if(store) _NodeList.push_back(n);
      } break;

      default:	// draw a circle etc (default shape)
      {
	cvs.filledCir(X,Y,5);
	Node n(++_CurrentID,1,_CurrentColor,"",X,Y);
	if(store) _NodeList.push_back(n);
      } break;
    }
}

void mouseStuff(int button, int state, int mouseX, int mouseY)
{
    int X = mouseX;
    int Y = (ScreenHeight - mouseY); //fixing upside-down mouse coord
    if((button==GLUT_LEFT_BUTTON)&&(state==GLUT_DOWN))
    {
      switch(_Mode)
      {
	case 0: makeNode(X, Y, 1); break;
	case 1: /*change properties*/ ; break;
	case 2: /*connect nodes*/ ; break;
	case 3: /*delete node*/ ; break;
      }
    }
}

void keyStuff(unsigned char key, int x, int y)
{
    switch(key)		// where changing mode, this changes the menus
    {
// 'q' key for quitting the program
      case 113: exit(0); break;

// 'n' key for changing mode to 'new node'. this is the default mode.
      case 110: if(_Mode != 0){modeChange(0);} break;

// 'p' key for changing mode to 'Properties of node'.
      case 'p':	if(_Mode != 1){modeChange(1);} break;

// 'c' key for changing mode to 'Connect nodes'.
      case 99: if(_Mode != 2){modeChange(2);} break;

// 'd' key for changing mode to 'Delete nodes'.
      case 100:	if(_Mode != 3){modeChange(3);} break;

      default: if(_Mode != 0){modeChange(0);} break;
    }
}

void displayStuff(void)
{   
    glClear(GL_COLOR_BUFFER_BIT); // do something with this
    redraw();
    glFlush(); // to stop window clearing - like redisplay stuff
}

void reshapeStuff(int width, int height)
{
/* allows for keeping the nodes to the same size and aspect */
    ScreenWidth = width;
    ScreenHeight = height;
    cvs.setWindow(0, (float)width, 0, (float)height);
    cvs.setViewport(0, width, 0, height);
    _WidthRatio = (800/ScreenWidth);
    _HeightRatio = (600/ScreenHeight);
}

int main(int argc, char**argv)
{
    cvs.setWindow(0.0, 800.0, 0.0, 600.0);
    cvs.setViewport(0, 800, 0, 600);
    cvs.setBackgroundColor(1.0,1.0,1.0);
    cvs.clearScreen();
    cvs.setColor(0,0,0); // sets default color.
    modeChange(0); // sets up menus for default mode, creating nodes.
    glutAttachMenu(GLUT_RIGHT_BUTTON);	//binding main menu to mouse right
    glutReshapeFunc(reshapeStuff);
    glutDisplayFunc(displayStuff);
    glutMouseFunc(mouseStuff);
    glutKeyboardFunc(keyStuff);
    glutMainLoop();
    return 0;
}







