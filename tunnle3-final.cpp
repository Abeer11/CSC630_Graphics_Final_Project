/*************************************************************************				
 *									
 *  Created by Abeer Rajhi on 11/15/11.				
 *  Copyright 2011 SJU. All rights reserved.			
 *	This is the program represents ours final project in the CSC 630.
 *	The project is a 3d game that takes a place in a tunnel some where
 *	in the fantasy world. the project is a combination between physics' 
 *	formulas and graphical generating images.  This project implements
 *	most the points that is covered in this course along with what we 
 *	learned through searching. Here some of the implementation we have:
 *		1-viewing.
 *		2-using the geometric objects.
 *		3-color and materials(state mangments).
 *		4-light and shading.
 *		5-texture mapping.
 *		6-transformation matrices.
 *		
 *
 *
 *	For details check readMe.txt.......	
 *
 *	Abeer Rajhi and Xingdong  		
 *************************************************************************/

//SHIPS

//#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream.h>
#include <string.h>
#include <math.h>
#include <time.h>
//#include <GL/glut.h>
#include <GLUT/glut.h>

#define WHITE_EYE_BALLRADIUS 0.5	/*radius of eye */
#define BLACK_EYE_BALLRADIUS 1.8	/*radius of eye */
#define BALLRADIUS 2				/*radius of eye */

#define NO_COINS 8					/*number of coins */

#define BRICK_SIZE 14
#define BRICK_NUMBER_OF_GROUND 100

#define Z_HEIGHT 1.9

#define ROTATE_ON_X	1.00
#define ROTATE_ON_Y	1.00
#define ROTATE_ON_Z	1.00

#define NO_FACES 30
#define NO_SIDES 30

#define SOLID 1
#define HOLE 0


#include "imageloader.h"

//==================Function Headers====================//

void initWindow(void);
void generateGround(void);
void generateCoins(void);
void changeSize(int, int);
void init(void);
void drawHeadPhones(void);
void display(void); 
void part1(void);
void part2(void);
void part3(void);
void createCircle(float radius);
void face(void);
void eye(void);
void ribbon(void);
void camera1(void);
void ground(void);
void glutSolidCube1(GLdouble);
void MoveBallForward(int);

//======================================================//
//===============The gloable variables==================//
//======================================================//


GLuint _textureId, _textureId3; //The id of wall's texture

GLuint _textureId2;				//The id of the texture of ground

int h,w;						//the window width and height

float yEndOfGround=-(BRICK_NUMBER_OF_GROUND*BRICK_SIZE);
float xEndOfGround=21, zBeginOfCeiling=30.0, zBeginOfGround=0.0;
float xBeginOfGround=-21, yBeginOfGround=0, zEndOfGround=-14;


float counter=-1000;

int insideTheWindow=0;

float PI=3.14159265;

float xMouse=0.50, yMouse=0.60;

float velocity =-0.3;	//the amount of velocity

bool pauseScreen=false;	//is it in a pause condition or not?

float	ballFacingAngle=0.00;

float	deltaMove  = 0.00;	//the new space that will pass as aparameter to
							// moveMe()

float	sideMove=0.0;

float	rotateAngleToMoveForwad=1.00;	//the angle 

bool DoneJumping=true;

float	ratio;	//the ratio of window width to its height

int button=GLUT_RIGHT_BUTTON;

float	x= 0,	//the position of the PinkBall and its camera
		y= 0,
		z= Z_HEIGHT;


float	ball_X = -2.00,		//the position of the MrBall and its camera
		ball_Y= yBeginOfGround,
		ball_Z = Z_HEIGHT;

bool reachMaxHeight=false;

float	lookAtX=0.00,		//the spot where Ball's camera looking at
		lookAtY=1.00,
		lookAtZ=1.50;

GLUquadricObj* qobj;

const float DEG2RAD = 3.14159/180;

float groundBricksArray[BRICK_NUMBER_OF_GROUND][3];

bool firstTime=true;

int random1;

//======================================================//
//===================== Functions ======================//
//======================================================//

//This method takes image objects and converts it into an openGL texture, 
//and returns the id of that texture....
GLuint loadTexture(Image* image) 
{
	GLuint textureId;
	glGenTextures(1, &textureId);	//this function takes two arguments, the first 
									//part is how many textures we need. The second
									//part is an array to sort the IDs for the
									//texture.
	
	glBindTexture(GL_TEXTURE_2D, textureId);	//use the ID of the texture to be edit
												//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D, 0,           
				 GL_RGB,                       //the format uses for the image
				 image->width, image->height,  //Width and height(allways
				 0,                            //The border of the image
				 
				 GL_RGB,					   //GL_RGB, because pixels are stored
												// in RGB format
				 
				 GL_UNSIGNED_BYTE,			   //GL_UNSIGNED_BYTE, because pixels are
												// stored as unsigned numbers
				 
				 image->pixels);               //pixel is the array that has all data
												// for the texture.
	return textureId; //Returns the id of the texture
}


//this function to resize the window
void changeSize(int w1, int h1)
{    
    if(h1 == 0)
        h1 = 1;
	
    w = w1;
    h = h1;
    ratio = 1.0f * w / h;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
	gluPerspective(80,ratio,0.1,1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	camera1();
	
}
//move your camera
void camera1()
{
	gluLookAt(ball_X, ball_Y+8, 4.5, 
			  lookAtX+ball_X, lookAtY+ball_Y+2, lookAtZ,
			  0.00,0.00,1.00);
}

//this func to draw the PinkBall
void drawPinkBall() 
{
	
	glPushMatrix();
		glTranslatef(x, y, z);//move this object to this value
		ribbon();
	
		glPushMatrix();
			glRotatef(90, 0, 0, ROTATE_ON_Z);
			glTranslatef(-0.75, 1.4, 0.8);
			glColor3f(0, 0, 0);
			glPushMatrix();
			glRotatef(80,ROTATE_ON_X,0,0);
				glutSolidTorus(0.05, 0.5, NO_SIDES, NO_FACES);
			glPopMatrix();
			eye();
		glPopMatrix();
	
		glPushMatrix();
			glRotatef(90, 0, 0, ROTATE_ON_Z);
			glTranslatef(0.75, 1.4, 0.8);
			glPushMatrix();
			glRotatef(80,ROTATE_ON_X,0,0);
				glutSolidTorus(0.05, 0.5, NO_SIDES, NO_FACES);
			glPopMatrix();
			eye();
		glPopMatrix();
	
		glColor3f(1.00, 0.80, 0.80);
		glRotated(90, 0.0, 0.0, ROTATE_ON_Z);
		glPushMatrix();
			glutSolidSphere(2, NO_SIDES, NO_FACES);//first torus
			glRotated(90, 0.0, ROTATE_ON_Y, .0);	
		glPopMatrix();
	glPopMatrix();
	
	
}

void generateGround(void)
{
	for(int i= 0 ;i<BRICK_NUMBER_OF_GROUND; i++)
	{	
		random1= rand()%5;
		
		if(random1==1)
		{
			groundBricksArray[i][0]=-14;
			groundBricksArray[i][1]=counter;
			i++;
			groundBricksArray[i][0]=0;
			groundBricksArray[i][1]=counter;
		}
		
		else if(random1==2)
		{
			groundBricksArray[i][0]=0;
			groundBricksArray[i][1]=counter;
			i++;
			groundBricksArray[i][0]=14;
			groundBricksArray[i][1]=counter;
			
		}
		
		else if(random1==3)
		{
			groundBricksArray[i][0]=14;
			groundBricksArray[i][1]=counter;
			
		}
		
		else  
		{
			groundBricksArray[i][0]=-14;
			groundBricksArray[i][1]=counter;
			i++;
			groundBricksArray[i][0]=0;
			groundBricksArray[i][1]=counter;
			i++;
			groundBricksArray[i][0]=14;
			groundBricksArray[i][1]=counter;
			
		}
		
		counter+=14;
	}
}

void generateCoins(void){
	
}

//to set the values of the ground coodinate, and then draw it.
void ground()
{
	for(int i=0; i<BRICK_NUMBER_OF_GROUND; i++)
	{
		glPushMatrix();
		glTranslatef(groundBricksArray[i][0], groundBricksArray[i][1], -7);
		glutSolidCube1(BRICK_SIZE);
		glPopMatrix();
		
	}
	
}

static void drawBox(GLfloat size, GLenum type)
{
	static GLfloat n[6][3] =
	{
		{-1.0,  0.0,  0.0},
		{ 0.0,  1.0,  0.0},
		{ 1.0,  0.0,  0.0},
		{ 0.0, -1.0,  0.0},
		{ 0.0,  0.0,  1.0},
		{ 0.0,  0.0, -1.0}
	};
	static GLint faces[6][4] =
	{
		{0, 1, 2, 3},
		{3, 2, 6, 7},
		{7, 6, 5, 4},
		{4, 5, 1, 0},
		{5, 6, 2, 1},
		{7, 4, 0, 3}
	};
	GLfloat v[8][3];
	GLint i;
	
	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;
	
	for (i = 5; i >= 0; i--) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBegin(type);
		glNormal3fv(&n[i][0]);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3fv(&v[faces[i][0]][0]);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3fv(&v[faces[i][1]][0]);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3fv(&v[faces[i][2]][0]);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3fv(&v[faces[i][3]][0]);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}

void glutSolidCube1(GLdouble size)
{
	drawBox(size, GL_QUADS);
}

void coins()
{
	float coinColor[]={1,.8,0};
	GLfloat SpecularLight[] = {1.0, 1.0, 1.0};
	
	glLightfv (GL_LIGHT0, GL_SPECULAR, SpecularLight); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,coinColor );
	glPushMatrix();
	glTranslatef(-3, 1, 2);
	glPushMatrix();
	glRotatef(-90, ROTATE_ON_X, 0, 0);
	glColor3f(1,.8,0);
	glPushMatrix();
	gluQuadricDrawStyle(qobj, GLU_FILL); /* flat shaded */
	gluQuadricNormals(qobj, GLU_SMOOTH);
	gluCylinder(qobj, 1.0, 1.0, 0.5, 40, 3);
	glPopMatrix();
	
	
	glPushMatrix();
	glTranslatef(0.00, 0.00, 0.50);
	glPushMatrix();
	glRotatef(0, 0, ROTATE_ON_Y, 0);
	
	
	createCircle(1);
	glPopMatrix();
	glPopMatrix();
	
	glPushMatrix();
	glPushMatrix();
	glColor3f(1,.8,0);
	createCircle(1);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void ribbon(void)
{
	glPushMatrix();
	glTranslatef(0, 0, 2);
	glRotated(10, 0, ROTATE_ON_Y, ROTATE_ON_Z);
	glColor3f(1, 0, 0);
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0.5);
	glVertex3f(0, 1, -0.5);
	
	glVertex3f(0, 0, 0);
	glVertex3f(0, -1, 0.5);
	glVertex3f(0, -1, -0.5);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	
}

//this func to draw MrBall character
void drawBall() 
{
	glColor3f(1.00, 1.00, 0.30);
	glPushMatrix();
	glTranslatef(ball_X, ball_Y, ball_Z);//move this object to this value
	glPushMatrix();
	glRotated(-rotateAngleToMoveForwad, ROTATE_ON_X, 0.0, 0.0);
	glPushMatrix();
	glRotated(90, 0.0, ROTATE_ON_Y, 0.0);
	glutSolidSphere(2, NO_SIDES, NO_FACES); //first torus	
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	
	drawHeadPhones();
	
	//right eyeBall.
	glPushMatrix();
	glTranslatef(ball_X,ball_Y, ball_Z+0.4);
	glRotatef(ballFacingAngle-17.5, 0.00, 0.00, ROTATE_ON_Z);
	glTranslatef(0.0, 1.70, 0.0);//but the eye on the yellow ball
	eye();
	glPopMatrix();
	
	//left eyeBall
	glPushMatrix();
	glTranslatef(ball_X,ball_Y, ball_Z+0.4);
	glRotatef(ballFacingAngle+17.5, 0.00, 0.00, ROTATE_ON_Z);
	glTranslatef(0.0, 1.70, 0.0);
	eye();
	glPopMatrix();
	
}



GLvoid CALLBACK errorCallback()
{
	const GLubyte *estring;
	
	//estring = gluErrorString(errorCode);
	//fprintf(stderr, "Quadric Error: %s\n", estring);
	exit(0);
}

void face(void)
{
	glPushMatrix();
	glTranslatef(0.50, 0.00, 0.40);
	eye();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.50, 0.00, 0.40);
	eye();
	glPopMatrix();
}

void eye(void)
{
	//glPushMatrix();
	glColor3f(1.00, 1.00, 1.00);
	glutSolidSphere(0.5, NO_SIDES, NO_FACES);//whiteBall
	glTranslatef(-xMouse+0.5, +0.35, yMouse-0.5);
	glColor3f(0.00, 0.00, 0.00);
	glutSolidSphere(0.18, NO_SIDES, NO_FACES);//blackBall
	//glPopMatrix();
	
	glutPostRedisplay();
}

void drawHeadPhones()
{	
	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(ball_X, ball_Y, ball_Z);
	glRotatef(ballFacingAngle, 0.00, 0.00, ROTATE_ON_Z);
	part1();
	part2();
	part3();
	glPopMatrix();
	
}
void part1(void)
{
	double eq[4]={-4, 1, 3, 0};
	glPushMatrix();
	glRotated(90, ROTATE_ON_X, 0.0, 0.0);
	glPushMatrix();
	glPushMatrix();
	glRotated(-78, 0.0, 0.0, ROTATE_ON_Z);
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, eq);
	glutSolidTorus(0.3, 1.9, NO_SIDES, NO_FACES);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void part2(void)
{
	glColor3f(0.00, 0.00, 0.00);
	glPushMatrix();
	glTranslatef((2.00), 0.00, 0.00);
		glPushMatrix();
			glRotated(90, 0.0, ROTATE_ON_Y, 0.0);
			glutSolidTorus(0.2, 0.5, NO_SIDES, NO_FACES);
		glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef((-2.00), 0.00, 0.00);
	glPushMatrix();
	glRotated(90, 0.0, ROTATE_ON_Y, 0.0);
	glutSolidTorus(0.2, 0.5, NO_SIDES, NO_FACES);
	glPopMatrix();
	glPopMatrix();
	
}

void part3(void)
{
	glColor3f(1.00, 0.00, 0.00);
	gluQuadricDrawStyle(qobj, GLU_FILL); /* flat shaded */
	gluQuadricNormals(qobj, GLU_SMOOTH);
	
	glPushMatrix();
	glTranslatef(-2.3, 0.00, 0.00);
	glPushMatrix();
	glRotated(90, 0.0, ROTATE_ON_Y, 0.0);
	createCircle(0.5);
	gluCylinder(qobj, 0.5, 0.5, 4.63, 40, 3);
	glPopMatrix();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(2.33, 0.00, 0.00);
	glPushMatrix();
	glRotated(90, 0.0, ROTATE_ON_Y, 0.0);
	createCircle(0.5);
	glPopMatrix();
	glPopMatrix();
}



//this method will draw the circle that locate on the 
//side of the headphones.
void createCircle(float radius )
{
	int i;
	
	//This loop will calculate the values of (x, y)
	// whick is a point on the Circle
	glBegin(GL_TRIANGLE_FAN);
	
	glNormal3d(-1, 0, 0);
	for (i=0; i <= 360; i+=4)
	{
		float degInRad = i*DEG2RAD;
		glVertex3f((cos(degInRad)*radius),(sin(degInRad)*radius),0.00);
	}
	glEnd();
	
}

void init() 
{
	
	qobj = gluNewQuadric();//GLvoid
	gluQuadricCallback(qobj, GLU_ERROR, errorCallback);
	
	glColor3f(0.90, 0.90, 0.90);
	glEnable (GL_BLEND);//enable blending 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//to control how the blending occurs.
	
	glEnable(GL_DEPTH_TEST);//enable depth	
	glEnable(GL_LIGHTING);	//enable light
	glEnable(GL_SMOOTH);	//enable smooth shading
	glEnable(GL_LIGHT0);	//enable light 0
	glEnable(GL_NORMALIZE);	//enable nurmal vector
	glEnable(GL_COLOR_MATERIAL);//by using this thing will show in color not on gray scale.
	glDepthFunc(GL_LESS);
	
	glEnable(GL_SMOOTH);
	glShadeModel(GL_SMOOTH);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	
	
	//specify the light position
	GLfloat lightpos[] = {1.000,//the x value
		5.000,//the y value
		10.00,//the z value
		0.00};// this parameter means the light is at infinity
	
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);//specify the light0 position
	
	GLfloat color1[] = {0.80, 0.80, 0.80, 1.00};
	GLfloat color2[]  = {0.00, 0.80, 0.80, 1.00};
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color2);//light that comes from all directions
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color1);
	
	GLfloat shininess[] = {50};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	
	
	Image* image = loadBMP("vtr3.bmp");//make the image file into image object
	_textureId = loadTexture(image);//load the the image as a texture
	delete image;					//then delete the image, because the image already 
									//sorted in the pixel array
	
	image = loadBMP("vtr5.bmp");//make the image file into image object
	_textureId2 = loadTexture(image);//load the the image as a texture
	delete image;					//then delete the image, because the image already 
									//sorted in the pixel array
	
	image = loadBMP("wall2.bmp");//make the image file into image object
	_textureId3 = loadTexture(image);//load the the image as a texture
	delete image;
	
	generateGround();
	
	generateCoins();
	
}


//this function moves MrBall and the camera forward
void moving()
{
	deltaMove=velocity;
	rotateAngleToMoveForwad+=(((deltaMove/4)*(360))/(4*PI));
	
	//to move the ball to the left or to the right
	if((sideMove>0 && ball_X<17.96) ||( sideMove<0 && ball_X>-17.80))
		ball_X  = ball_X  + sideMove;
	
	ball_Y = ball_Y + deltaMove;
	
	glLoadIdentity();
	camera1();	
}


//the drawing start from here...
void display(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	
	glBegin(GL_QUADS);
	glColor3f(0.5, 0.5, 0.5);//the ceiling
	glNormal3d(1.00, 0.00, 0.00);//normalized vector that needs for lighting
	glVertex3f(xBeginOfGround,  yBeginOfGround , zBeginOfCeiling);
	glVertex3f( xEndOfGround,  yBeginOfGround , zBeginOfCeiling);
	glVertex3f( xEndOfGround,  yEndOfGround, zBeginOfCeiling);
	glVertex3f(xBeginOfGround,  yEndOfGround, zBeginOfCeiling);
	glEnd();

	ground();
	glColor3f(1,1,1);
	//======================= right Wall ===========================//
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glBegin(GL_QUADS);
	glNormal3d(0.00, 1.00, 0.00);//normalized vector that needs for lighting
	glTexCoord2f(0.0, 0.0);
	glVertex3f(xBeginOfGround, yBeginOfGround, zBeginOfGround);
	glTexCoord2f(2.0, 0.0);
	glVertex3f(xBeginOfGround, yBeginOfGround, zBeginOfCeiling);
	glTexCoord2f(2.0, 50.0);
	glVertex3f(xBeginOfGround,  yEndOfGround, zBeginOfCeiling);
	glTexCoord2f(0.0, 50.0);
	glVertex3f(xBeginOfGround,  yEndOfGround, zBeginOfGround);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	
	//======================= Left Wall ===========================//
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glBegin(GL_QUADS);
	glNormal3d(0.00, 1.00, 0.00);//normalized vector that needs for lighting
	glTexCoord2f(0.0, 0.0);
	glVertex3f( xEndOfGround, yEndOfGround, zBeginOfGround);
	glTexCoord2f(2.0, 0.0);
	glVertex3f( xEndOfGround, yEndOfGround, zBeginOfCeiling);
	glTexCoord2f(2.0, 50.0);
	glVertex3f( xEndOfGround, yBeginOfGround, zBeginOfCeiling);
	glTexCoord2f(0.0, 50.0);
	glVertex3f( xEndOfGround, yBeginOfGround, zBeginOfGround);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	//======================= Part of the hole ===========================//
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	//left part
	glBegin(GL_QUADS);
	glNormal3d(0.00, 1.00, 0.00);//normalized vector that needs for lighting
	glTexCoord2f(0.0, 0.0);
	glVertex3f( xEndOfGround, yEndOfGround, zBeginOfGround);
	glTexCoord2f(2.0, 0.0);
	glVertex3f( xEndOfGround, yEndOfGround,  zEndOfGround);
	glTexCoord2f(2.0, 100.0);
	glVertex3f( xEndOfGround, yBeginOfGround, zEndOfGround);
	glTexCoord2f(0.0, 100.0);
	glVertex3f( xEndOfGround, yBeginOfGround, zBeginOfGround);
	glEnd();
	
	//right part
	
	glBegin(GL_QUADS);
	glNormal3d(0.00, 1.00, 0.00);//normalized vector that needs for lighting
	//glColor3f(0.65, 0.65, 0.6);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(xBeginOfGround, yBeginOfGround, zBeginOfGround);
	glTexCoord2f(2.0, 0.0);
	glVertex3f(xBeginOfGround, yBeginOfGround, zEndOfGround);
	glTexCoord2f(2.0, 100.0);
	glVertex3f(xBeginOfGround,  yEndOfGround,  zEndOfGround);
	glTexCoord2f(0.0, 100.0);
	glVertex3f(xBeginOfGround,  yEndOfGround, zBeginOfGround);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	//Draw balls

	drawBall();
	
	drawPinkBall();
	
	coins();
	
	firstTime=false;
	
    glutSwapBuffers();
}

/****findme*****/
float freq=1.0;
float amplitude = Z_HEIGHT;
float decay = 0.5;
double ttime = 0;
double timeBeginJump = 0;
/**************/

void zJump(int value){
	
    ttime = (glutGet(GLUT_ELAPSED_TIME)-timeBeginJump) / 900.0;
	
    float posSin = fabs(sin(freq*ttime*2*PI));
    ball_Z= amplitude+3*amplitude*posSin/exp(decay*ttime);

	if(ttime < 8){
		glutTimerFunc(1, zJump, 0);
	}else {
		DoneJumping = true;
	}

	
}

//this function to track the input from keyboard
void normalKeys(unsigned char key, int x, int y) 
{	
	if(!pauseScreen)//do not activat the keyboard function
		//unless the key is P or p.
		switch (key) 
	{
		case 27 :
		case 'q':
		case 'Q':
			exit(0);
			break;

		case 'p'://pause the program
		case 'P':
			pauseScreen=true;
			break;
			
			case ' ':
			if(ball_Z<=Z_HEIGHT+0.3){
				timeBeginJump = glutGet(GLUT_ELAPSED_TIME);
				if(DoneJumping)
					zJump(0);
				DoneJumping=false;

			}
			//if (DoneJumping) 
//			{
                
            //}
			break;
			
	}
	else if(pauseScreen && (key=='p' || key=='P'))
		pauseScreen=false;
}

//this function to track the special key input from keyboard
void specialKeys(int key, int x, int y)
{
	if(!pauseScreen)
		switch (key) 
	{
        case GLUT_KEY_LEFT  : sideMove= 0.5 ;break;//turn left
        case GLUT_KEY_RIGHT : sideMove= -0.5 ;break;//turn right
        case GLUT_KEY_UP    :
			break;
        case GLUT_KEY_DOWN  : 
			break;
    }
}

//this function to track if any key was released or not
//if any set it value to zero.
void releaseSpecialKeys(int key, int x, int y) 
{	
	if(!pauseScreen)
		switch (key) 
	{
		case GLUT_KEY_LEFT : 
			sideMove = 0.00;
			break;
		case GLUT_KEY_RIGHT : 
			sideMove = 0.00;
			break;
		case GLUT_KEY_UP :    
			//deltaMove = 0.00;
			break;
		case GLUT_KEY_DOWN : 
			//deltaMove = 0.00;
			break;
	}
}

//this function to track the mouse
void mouseHandeling(int button, int state,int xMouseTracker, int yMouseTracker)
{
	if(button == GLUT_LEFT_BUTTON && state==GLUT_DOWN)
		button= GLUT_LEFT_BUTTON;
	
	if(button == GLUT_MIDDLE_BUTTON && state==GLUT_DOWN)//pause screen
		button= GLUT_MIDDLE_BUTTON;
	
	
	glutPostRedisplay();
	
	changeSize(w,h);
}

void IsMouseInTheWindow( int state)
{
	if(state!=GLUT_LEFT)
		insideTheWindow=1;
	else
		insideTheWindow=0;
}

void mouseTracker(int xMouseTracker, int yMouseTracker)
{
	if (insideTheWindow)
	{
		yMouse=(((float)w-yMouseTracker)/w);
		xMouse=((float)xMouseTracker/w);
	}
}

void rotateBall(int value)
{
	if(ballFacingAngle != 180)
	{
		ballFacingAngle+=4;
		glutTimerFunc(5, rotateBall, 0);
	}

}

void MoveBallForward(int value)
{
    moving();
	glutTimerFunc(1, MoveBallForward, 0);
}


//all the callback func
void initWindow() 
{
	
	glutMouseFunc(mouseHandeling);
	
    glutKeyboardFunc(normalKeys);
    glutSpecialFunc(specialKeys);
    glutSpecialUpFunc(releaseSpecialKeys);
	glutEntryFunc(IsMouseInTheWindow);
	glutPassiveMotionFunc(mouseTracker);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(changeSize);
	glutTimerFunc(5, rotateBall, 0);
	glutTimerFunc(5000, MoveBallForward, 0);
	
	
    init();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,500);
    glutCreateWindow("Assignment 3");
	
    initWindow();
	
    glutMainLoop();
	
    return(0);
} 





