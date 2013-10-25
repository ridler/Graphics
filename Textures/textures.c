// Scenes with modifiable lighting
// By Forrest Tagg Ridler | forrest.ridler@colorado.edu

// Animation structure and basic shapes based on ex8.c
// Projection set-up and calculations based on ex9.c
// Lighting parameters and switch based on ex13.c

#include "CSCIx229.h"

// parameters for lighting
unsigned char one       =   1;  // Unit value
unsigned char distance  =   5;  // Light distance
float inc 		        =   1;  // Ball increment
unsigned char smooth    =   1;  // Smooth/Flat shading
short local    			=   0;  // Local Viewer Model
unsigned char emission  =   0;  // Emission intensity (%)
unsigned char ambient   =  30;  // Ambient intensity (%)
unsigned char diffuse   = 100;  // Diffuse intensity (%)
unsigned char specular  =   0;  // Specular intensity (%)
unsigned short shininess =  2;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
short zh        =  90;  // Light azimuth
float ylight = 0;  // Elevation of light

// textures
unsigned int SANDSTONE, EARTH, COPPER, SUN;

int th=0;	//  Azimuth of view angle
int ph=0;	//  Elevation of view angle

unsigned char obilisk = 1;	// toggle objects displayed
unsigned char move = 1;		// light orbital

int fov = 50;	// field of view

float asp=1.0;	//  Aspemct ratio
float dim=5.0;	//  Size of world

static inline void Vertex(double th,double ph)
{
	double x = Sin(th)*Cos(ph);
	double y = Cos(th)*Cos(ph);
	double z =         Sin(ph);
	glNormal3d(x,y,z);
	glTexCoord2d(th/360.0,ph/180.0+0.5);
	glVertex3d(x,y,z);
}

static void sphere(float x, float y, float z, float r,
	unsigned int tex)
{
	const unsigned char d = 5;
	short t, p;

	glTranslated(x,y,z);
	glScaled(r,r,r);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
   		//  Latitude bands
	for (p=-90;p<90;p+=d)
	{
		glBegin(GL_QUAD_STRIP);
		for (t=0;t<=360;t+=d)
		{
			Vertex(t,p);
			Vertex(t,p+d);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}

static void ball(double x,double y,double z,double r)
{
	glPushMatrix();
	glTranslated(x,y,z);
	glScaled(r,r,r);
	glColor3f(1,1,1);
	glutSolidSphere(1.0,16,16);
	glPopMatrix();
}

static void cube(float x, float y, float z,
	float dx, float dy, float dz, unsigned int tex)
{
	glPushMatrix();

	glTranslated(x,y,z);
	glScaled(dx,dy,dz);

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   //	Front
	glBindTexture(GL_TEXTURE_2D,tex);
	glBegin(GL_QUADS);
	glNormal3f( 0, 0, 1);
	glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
	glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
	glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
	glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
	glEnd();
   //	Back
	glBindTexture(GL_TEXTURE_2D,tex);
	glBegin(GL_QUADS);
	glNormal3f( 0, 0,-1);
	glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
	glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
	glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
	glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
	glEnd();
   //  Right
	glBindTexture(GL_TEXTURE_2D,tex);
	glBegin(GL_QUADS);
	glNormal3f(+1, 0, 0);
	glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
	glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
	glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
	glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
	glEnd();
   //  Left
	glBindTexture(GL_TEXTURE_2D,tex);
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
	glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
	glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
	glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
	glEnd();
   //  Top
	glBindTexture(GL_TEXTURE_2D,tex);
	glBegin(GL_QUADS);
	glNormal3f( 0,+1, 0);
	glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
	glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
	glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
	glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
	glEnd();

	glDisable(GL_TEXTURE_2D);
   //  Bottom
	glBegin(GL_QUADS);
	glNormal3f( 0,-1, 0);
	glVertex3f(-1,-1,-1);
	glVertex3f(+1,-1,-1);
	glVertex3f(+1,-1,+1);
	glVertex3f(-1,-1,+1);
	glEnd();
   //  Undo transformations and textures
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

// Pyramid idea from : http://www.planetsourcecode.com/vb/scripts/ShowCode.asp?txtCodeId=10693&lngWId=3
static void pyramid(float x, float y, float z,
	float dx, float dy, float dz, unsigned int tex)
{
	glPushMatrix();
	{	
		glTranslated(x,y,z);
		glScaled(dx,dy,dz);

		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

		glBegin(GL_TRIANGLES);
		glBindTexture(GL_TEXTURE_2D,tex);
		glNormal3f(0,2,4);
		glTexCoord2f(0,0); glVertex3f(1,0,1);
		glTexCoord2f(0,1); glVertex3f(0,2,0);
		glTexCoord2f(1,1); glVertex3f(-1,0,1);
		glEnd();

		glBegin(GL_TRIANGLES);
		glBindTexture(GL_TEXTURE_2D,tex);
		glNormal3f(-4,2,0);
		glTexCoord2f(0,0); glVertex3f(-1,0,1);
		glTexCoord2f(0,1); glVertex3f(0,2,0);
		glTexCoord2f(1,1); glVertex3f(-1,0,-1);
		glEnd();

		glBegin(GL_TRIANGLES);
		glBindTexture(GL_TEXTURE_2D,tex);
		glNormal3f(0,2,-4);
		glTexCoord2f(0,0); glVertex3f(-1,0,-1);
		glTexCoord2f(0,1); glVertex3f(0,2,0);
		glTexCoord2f(1,1); glVertex3f(1,0,-1);
		glEnd();

		glBegin(GL_TRIANGLES);
		glBindTexture(GL_TEXTURE_2D,tex);
		glNormal3f(4,2,0);
		glTexCoord2f(0,0); glVertex3f(1,0,-1);
		glTexCoord2f(0,1); glVertex3f(0,2,0);
		glTexCoord2f(1,1); glVertex3f(1,0,1);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
}

// position is (x, y, z), d=dimension of base, h=height,
// e = "pointienss", c = color
static void drawObilisk(float x, float y, float z,
	float d, float h, float e, unsigned int cubeTex,
	unsigned int pyrTex)
{
	cube(x, y, z ,d, h, d, cubeTex);
	pyramid(x, y+h, z, d, h/e, d, pyrTex);
}

static void project()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
   //  Perspective transformation
	gluPerspective(fov,asp,dim/4,4*dim);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	
	double Ex = -2*dim*Sin(th)*Cos(ph);
	double Ey = +2*dim        *Sin(ph);
	double Ez = +2*dim*Cos(th)*Cos(ph);
	gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);

	float Ambient[]   = {0.3,0.3,0.3,1.0};
	float Diffuse[]   = {1,1,1,1};
	float Specular[]  = {1,1,0,1};
	float white[]     = {1,1,1,1};
      //  Light direction
	float Position[]  = {5*Cos(zh),ylight,5*Sin(zh),1};
      //  Draw light position as ball
	ball(Position[0],Position[1],Position[2] , 0.1);
      //  Enable lighting with normalization
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
      //  glColor sets ambient and diffuse color materials
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
      //  Enable light
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
	glLightfv(GL_LIGHT0,GL_POSITION,Position);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,32.0f);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);

		// row1
	drawObilisk(-1,0,0,0.15,0.75,4,SANDSTONE,SANDSTONE);
	drawObilisk(0,0,0,0.15,0.75,4,COPPER,COPPER);
	drawObilisk(1,0,0,0.15,0.75,4,SANDSTONE,SANDSTONE);
		// row2
	drawObilisk(-1,0,0.75,0.15,0.75,4,SANDSTONE,SANDSTONE);
	drawObilisk(0,0,0.75,0.15,0.75,4,COPPER,COPPER);
	drawObilisk(1,0,0.75,0.15,0.75,4,SANDSTONE,SANDSTONE);
		// row3
	drawObilisk(-1,0,1.5,0.15,0.75,4,SANDSTONE,SANDSTONE);
	drawObilisk(0,0,1.5,0.15,0.75,4,COPPER,COPPER);
	drawObilisk(1,0,1.5,0.15,0.75,4,SANDSTONE,SANDSTONE);
		// house?
	drawObilisk(0,0,-1.75,1,0.75,5,EARTH,EARTH);
	// SUN
	sphere(0,2,-4, 1, SUN);
   //  Render the scene
	glFlush();
   //  Make the rendered scene visible
	glutSwapBuffers();
}

// process arrow key input to change view
void special(int key,int x,int y)
{
	switch(key)
	{
		case GLUT_KEY_RIGHT: th += 5; break;
		case GLUT_KEY_LEFT : th -= 5; break;
		case GLUT_KEY_UP   : ph += 5; break;
		case GLUT_KEY_DOWN : ph -= 5; break;
		default: break;
	}
   //  Keep angles to +/-360 degrees
	th %= 360;
	ph %= 360;
	project();
	glutPostRedisplay();
}

// process keyboard input not related to arrow keys:
void key(unsigned char k, int x, int y)
{
	switch(k)
	{
		case 27 : exit(0); break;
		case '+': fov--; break;
		case '-': fov++; break;
		case 's': move = !move; break;
		case '<': ylight++; break;
		case '>': ylight--; break;
		case 'v': inc -= 0.1; break;
		case 'V': inc += 0.1; break;
		default : break;
	}
 	// make it more of a zoom-in feature:
	if(fov < 0) { fov = 0; }
	if(inc < 0) { inc = 0; }

	project();
	glutPostRedisplay();
}

void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
	asp = (height>0) ? (double)width/height : 1;

	glViewport(0,0, width,height);
	project();
}

void idle()
{
	if(move)
	{
   //  Elapsed time in seconds
		double t = inc*glutGet(GLUT_ELAPSED_TIME)/(1000.0);
		zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
		glutPostRedisplay();
	}
	else return;
}

int main(int argc,char* argv[])
{
	glutInit(&argc,argv);
	glutInitWindowSize(600,600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("Objects with Projections");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);
	SANDSTONE = LoadTexBMP("AshySandstone.bmp");
	EARTH = LoadTexBMP("earth.bmp");
	COPPER = LoadTexBMP("copper.bmp");
	SUN = LoadTexBMP("sun.bmp");
	glutMainLoop();
	return 0;
}