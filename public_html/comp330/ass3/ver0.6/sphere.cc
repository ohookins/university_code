#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include "SDL.h"
#include "RGBpixmap.h"

int _windowHeight = 600; //setting up window size and scene
int _windowWidth = 800;

GLfloat _eyeX = 1.9; // eye coords for changing viewpoint
GLfloat _eyeY = 1.9;
GLfloat _eyeZ = 1.9;

Scene myScene;
RGBpixmap pix[6];

void init(void)
{
    myScene.read("scene.dat"); //SDL scene file

    glEnable(GL_LIGHTING); //enables lighting and 3 lights
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);

//some surface lighting properties
    GLfloat mat_ambient[] = {0.5,0.5,0.5,1.0};
    GLfloat mat_diffuse[] = {0.7,0.7,0.7,1.0};
    GLfloat mat_specular[] = {0.9,0.9,0.9,1.0};
    GLfloat mat_shininess[] = {50.0};

    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);

// bathes the room in an ambient dull blue light
    GLfloat ambient[] = {0.3, 0.3, 0.4, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

/*	Some Lighting Properties	*/
//light 0 (positions set in display routine)
    GLfloat lintensity0[] = {1.0,1.0,1.0,1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lintensity0);

    
//light 1 - spot
    GLfloat lintensity1[] = {0.0,1.0,0.0,1.0};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lintensity1);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.0);

//light 2
    GLfloat lintensity2[] = {0.2,0.2,1.0,1.0};
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lintensity2);

//light 3 - spot
    GLfloat lintensity3[] = {1.0,0.5,0.5,1.0};
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lintensity3);
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 60.);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 1.0);

// enabling various effects
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_ALWAYS); //should really be GL_LESS
    
    //loading some textures for walls, sofa, floor, ceiling
    pix[0].readBMPFile("carpet.bmp"); 
    pix[0].setTexture(2001);
    pix[1].readBMPFile("wall.bmp");
    pix[1].setTexture(2002);
    pix[2].readBMPFile("ceiling.bmp");
    pix[2].setTexture(2003);
    pix[3].readBMPFile("fabric.bmp");
    pix[3].setTexture(2004);
    pix[4].readBMPFile("fabric2.bmp");
    pix[4].setTexture(2005);
    pix[5].readBMPFile("fabric3.bmp");
    pix[5].setTexture(2006);


//set textures to modulate - ie lighting affects them
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

//set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-4,4,-4,4,-4,50);
    gluPerspective(20.0, 800.0/600.0, 0, -50); //set camera shape

//set up the modelview matrix    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void keyStuff(unsigned char key, int x, int y)
{
    switch(key)
    {
      case 'q': exit(0); break; //quit

      case '1':{ //look at scene from SE corner
	_eyeX=_eyeY=_eyeZ=1.9;
	glutPostRedisplay();
	} break;

      case '2':{ //look at scene from SW corner
	_eyeZ=_eyeY=1.9;
	_eyeX=-1.9;
	glutPostRedisplay();
	} break;

      case '3':{ //look at scene from NW corner
	_eyeX=_eyeZ=-1.9;
	_eyeY=1.9;
	glutPostRedisplay();
	} break;

      case '4':{ //look at scene from NE corner
	_eyeX=_eyeY=1.9;
	_eyeZ=-1.9;
	glutPostRedisplay();
	} break;
    }
}

void displayStuff(void)
{
    glClearColor(1.0,1.0,1.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-4,4,-4,4,-4,50);
    gluPerspective(20.0, 800.0/600.0, 0, -50); //set camera shape

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(_eyeX,_eyeY,_eyeZ,0.0,1.0,0.0,0.0,1.0,0.0);

// now set the light positions, in global coords
    GLfloat lposition0[] = {0.0f,2.0f,0.0f,1};
    glLightfv(GL_LIGHT0, GL_POSITION, lposition0);

    GLfloat lposition1[] = {-2.0f,2.0f,2.0f,1.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, lposition1);
    GLfloat dir1[] = { 1.0, -1.0, -1.0 };
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir1);

    GLfloat lposition2[] = {2.0,0.0,-2.0,1};
    glLightfv(GL_LIGHT2, GL_POSITION, lposition2);

    GLfloat lposition3[] = {0.2,1.5,-1.6,1};
    glLightfv(GL_LIGHT3, GL_POSITION, lposition3);
    GLfloat dir3[] = { 1.0, -0.4, -0.1 };
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, dir3);
   
/*    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);*/

    glEnable(GL_TEXTURE_2D);

    glPushMatrix();


//floor
	glBindTexture(GL_TEXTURE_2D,2001);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex3f(-2.0f,0.0f,2.0f);
	glTexCoord2f(1.0,0.0); glVertex3f(2.0f,0.0f,2.0f);
	glTexCoord2f(1.0,1.0); glVertex3f(2.0f,0.0f,-2.0f);
	glTexCoord2f(0.0,1.0); glVertex3f(-2.0f,0.0f,-2.0f);
	glEnd();

//left wall
	glBindTexture(GL_TEXTURE_2D,2002);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex3f(-2.0f,0.0f,2.0f);
	glTexCoord2f(1.0,0.0); glVertex3f(-2.0f,0.0f,-2.0f);
	glTexCoord2f(1.0,1.0); glVertex3f(-2.0f,2.0f,-2.0f);
	glTexCoord2f(0.0,1.0); glVertex3f(-2.0f,2.0f,2.0f);
	glEnd();

//rear wall
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex3f(-2.0f,0.0f,-2.0f);
	glTexCoord2f(1.0,0.0); glVertex3f(2.0f,0.0f,-2.0f);
	glTexCoord2f(1.0,1.0); glVertex3f(2.0f,2.0f,-2.0f);
	glTexCoord2f(0.0,1.0); glVertex3f(-2.0f,2.0f,-2.0f);
	glEnd();

//right wall
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex3f(2.0f,0.0f,-2.0f);
	glTexCoord2f(1.0,0.0); glVertex3f(2.0f,0.0f,2.0f);
	glTexCoord2f(1.0,1.0); glVertex3f(2.0f,2.0f,2.0f);
	glTexCoord2f(0.0,1.0); glVertex3f(2.0f,2.0f,-2.0f);
	glEnd();

//front wall
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex3f(2.0f,0.0f,2.0f);
	glTexCoord2f(1.0,0.0); glVertex3f(-2.0f,0.0f,2.0f);
	glTexCoord2f(1.0,1.0); glVertex3f(-2.0f,2.0f,2.0f);
	glTexCoord2f(0.0,1.0); glVertex3f(2.0f,2.0f,2.0f);
	glEnd();

//ceiling
	glBindTexture(GL_TEXTURE_2D,2003);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex3f(2.0f,2.0f,2.0f);
	glTexCoord2f(1.0,0.0); glVertex3f(-2.0f,2.0f,2.0f);
	glTexCoord2f(1.0,1.0); glVertex3f(-2.0f,2.0f,-2.0f);
	glTexCoord2f(0.0,1.0); glVertex3f(2.0f,2.0f,-2.0f);
	glEnd();

//nicely textured sofa. hard to draw though
	glBindTexture(GL_TEXTURE_2D,2006);	// left side
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex3f(2.0f,0.0f,-1.5f);
	glTexCoord2f(1.0,0.0); glVertex3f(1.0f,0.0f,-1.5f);
	glTexCoord2f(1.0,1.0); glVertex3f(1.0f,1.0f,-1.5f);
	glTexCoord2f(0.0,1.0); glVertex3f(2.0f,1.0f,-1.5f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,2005);	// front left arm
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex3f(1.0f,0.0f,-1.5f);
	glTexCoord2f(1.0,0.0); glVertex3f(1.0f,0.0f,-1.3f);
	glTexCoord2f(1.0,1.0); glVertex3f(1.0f,1.0f,-1.3f);
	glTexCoord2f(0.0,1.0); glVertex3f(1.0f,1.0f,-1.5f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,2004);	// upper left arm
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex3f(1.0f,1.0f,-1.5f);
	glTexCoord2f(1.0,0.0); glVertex3f(1.0f,1.0f,-1.3f);
	glTexCoord2f(1.0,1.0); glVertex3f(2.0f,1.0f,-1.3f);
	glTexCoord2f(0.0,1.0); glVertex3f(2.0f,1.0f,-1.5f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,2006);	// inner left arm
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex3f(1.0f,0.5f,-1.3f);
	glTexCoord2f(1.0,0.0); glVertex3f(2.0f,0.5f,-1.3f);
	glTexCoord2f(1.0,1.0); glVertex3f(2.0f,1.0f,-1.3f);
	glTexCoord2f(0.0,1.0); glVertex3f(1.0f,1.0f,-1.3f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,2005);	// front base
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex3f(1.0f,0.0f,-1.3f);
	glTexCoord2f(1.0,0.0); glVertex3f(1.0f,0.0f,1.3f);
	glTexCoord2f(1.0,1.0); glVertex3f(1.0f,0.5f,1.3f);
	glTexCoord2f(0.0,1.0); glVertex3f(1.0f,0.5f,-1.3f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,2004);	// seat
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex3f(1.0f,0.5f,-1.3f);
	glTexCoord2f(1.0,0.0); glVertex3f(1.0f,0.5f,1.3f);
	glTexCoord2f(1.0,1.0); glVertex3f(2.0f,0.5f,1.3f);
	glTexCoord2f(0.0,1.0); glVertex3f(2.0f,0.5f,-1.3f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,2005);	// back rest
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex3f(1.9f,0.5f,-1.3f);
	glTexCoord2f(1.0,0.0); glVertex3f(1.9f,0.5f,1.3f);
	glTexCoord2f(1.0,1.0); glVertex3f(1.9f,1.0f,1.3f);
	glTexCoord2f(0.0,1.0); glVertex3f(1.9f,1.0f,-1.3f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,2006);	// inner right arm
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex3f(2.0f,0.5f,1.3f);
	glTexCoord2f(1.0,0.0); glVertex3f(1.0f,0.5f,1.3f);
	glTexCoord2f(1.0,1.0); glVertex3f(1.0f,1.0f,1.3f);
	glTexCoord2f(0.0,1.0); glVertex3f(2.0f,1.0f,1.3f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,2004);	// top right arm
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex3f(2.0f,1.0f,1.3f);
	glTexCoord2f(1.0,0.0); glVertex3f(1.0f,1.0f,1.3f);
	glTexCoord2f(1.0,1.0); glVertex3f(1.0f,1.0f,1.5f);
	glTexCoord2f(0.0,1.0); glVertex3f(2.0f,1.0f,1.5f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,2005);	// front right arm
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex3f(1.0f,0.0f,1.3f);
	glTexCoord2f(1.0,0.0); glVertex3f(1.0f,0.0f,1.5f);
	glTexCoord2f(1.0,1.0); glVertex3f(1.0f,1.0f,1.5f);
	glTexCoord2f(0.0,1.0); glVertex3f(1.0f,1.0f,1.3f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,2006);	// outer right arm
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex3f(1.0f,0.0f,1.5f);
	glTexCoord2f(1.0,0.0); glVertex3f(2.0f,0.0f,1.5f);
	glTexCoord2f(1.0,1.0); glVertex3f(2.0f,1.0f,1.5f);
	glTexCoord2f(0.0,1.0); glVertex3f(1.0f,1.0f,1.5f);
	glEnd();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
/*
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
*/
	myScene.drawSceneOpenGL();
	glColor3f(1,1,1);

//draw some spheres on the axes and the origin
	glPushMatrix();
	glutSolidSphere(0.1,30,30);
	glTranslated(1.0,0.0,0.0);
	glutSolidSphere(0.1,30,30);
	glTranslated(-1.0,1.0,0.0);
	glutSolidSphere(0.1,30,30);
	glTranslated(0.0,-1.0,1.0);
	glutSolidSphere(0.1,30,30);
	glTranslated(0.0,0.0,-1.0);	
	glPopMatrix();

//draw a sphere in the ceiling which looks like it is emanating light
    glPushMatrix();
    GLfloat no_emission[] = {0.0,0.0,0.0,1.0};
    GLfloat some_emission[] = {0.8,0.8,0.6,1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, some_emission);
    glTranslated(0.0,2.0,0.0);
    glutSolidSphere(0.1,30,30);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    glPopMatrix();
	
    glFlush();
}
    
int main(int argc, char**argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(_windowWidth, _windowHeight);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Reflective Sphere");
    init();    
    glutDisplayFunc(displayStuff);
    glutKeyboardFunc(keyStuff);
    glutMainLoop();
    return 0;
}

