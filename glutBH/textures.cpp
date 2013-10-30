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
float ylight  =   0;  // Elevation of light

unsigned int SANDSTONE = 0;

int th=0;	//  Azimuth of view angle
int ph=0;	//  Elevation of view angle

unsigned char obilisk = 1;	// toggle objects displayed
unsigned char light = 1;	// light switch
unsigned char move = 1;		// light orbital

int fov = 50;	// field of view

float asp=1.0;	//  Aspemct ratio
float dim=5.0;	//  Size of world

float cubeSep = 0.05;	// user can change separation of sheres

static inline void Vertex(double th,double ph)
{
	double x = Sin(th)*Cos(ph);
	double y = Cos(th)*Cos(ph);
	double z =         Sin(ph);
  	 //  For a sphere at the origin, the position
  	 //  and normal vectors are the same
	glNormal3d(x,y,z);
	glVertex3d(x,y,z);
}

static void cube(float x, float y, float z,
	float dx, float dy, float dz, unsigned char c[])
{
	float white[] = {1,1,1,1};
	float black[] = {0,0,0,1};
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

	unsigned char r, g, b;
	r = c[0]; g = c[1]; b = c[2];

	glPushMatrix();
	{
   //  Offset
		glTranslated(x,y,z);
 		//glRotated(th,0,1,0);
		glScaled(dx,dy,dz);
   //  Cube
		glBegin(GL_QUADS);
   //  Front
		glColor3f(r,g,b);
		glNormal3f(0,0,1);
		glVertex3f(-1,-1, 1);
		glVertex3f(+1,-1, 1);
		glVertex3f(+1,+1, 1);
		glVertex3f(-1,+1, 1);
   //  Back
		glColor3f(r,g,b);
		glNormal3f(0,0,-1);
		glVertex3f(+1,-1,-1);
		glVertex3f(-1,-1,-1);
		glVertex3f(-1,+1,-1);
		glVertex3f(+1,+1,-1);
   //  Right
		glColor3f(1,1,1);
		glNormal3f(1,0,0);
		glVertex3f(+1,-1,+1);
		glVertex3f(+1,-1,-1);
		glVertex3f(+1,+1,-1);
		glVertex3f(+1,+1,+1);
   //  Left
		glColor3f(1,1,1);
		glNormal3f(-1,0,0);
		glVertex3f(-1,-1,-1);
		glVertex3f(-1,-1,+1);
		glVertex3f(-1,+1,+1);
		glVertex3f(-1,+1,-1);
   //  Top
		glColor3f(0.5,0.5,0.5);
		glNormal3f(0,1,0);
		glVertex3f(-1,+1,+1);
		glVertex3f(+1,+1,+1);
		glVertex3f(+1,+1,-1);
		glVertex3f(-1,+1,-1);
   //  Bottom
		glColor3f(0.5,0.5,0.5);
		glNormal3f(0,-1,0);
		glVertex3f(-1,-1,-1);
		glVertex3f(+1,-1,-1);
		glVertex3f(+1,-1,+1);
		glVertex3f(-1,-1,+1);
   //  End
		glEnd();
	}
   //  Undo transformations
	glPopMatrix();
}

// Pyramid idea from : http://www.planetsourcecode.com/vb/scripts/ShowCode.asp?txtCodeId=10693&lngWId=3
static void pyramid(float x, float y, float z,
	float dx, float dy, float dz, unsigned char c[])
{
	unsigned char r, g, b;
	r = c[0]; g = c[1]; b = c[2];

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, SANDSTONE);

	glPushMatrix();
	{
		glTranslated(x,y,z);
		glScaled(dx,dy,dz);

		glBegin(GL_TRIANGLES);

		// Normals calculated using cross product of vectors
		// pointing along the edges of the triangles.

		glColor3f(1,1,1);
		glNormal3f(0,2,4);
		// first triangle
		glVertex3f(1,0,1);
		glVertex3f(0,2,0);
		glVertex3f(-1,0,1);

		glColor3f(r,g,b);
		glNormal3f(-4,2,0);
		// second triangle
		glVertex3f(-1,0,1);
		glVertex3f(0,2,0);
		glVertex3f(-1,0,-1);

		glColor3f(1,1,1);
		glNormal3f(0,2,-4);
		// third triangle
		glVertex3f(-1,0,-1);
		glVertex3f(0,2,0);
		glVertex3f(1,0,-1);

		glColor3f(r,g,b);
		glNormal3f(4,2,0);
		// last triangle
		glVertex3f(1,0,-1);
		glVertex3f(0,2,0);
		glVertex3f(1,0,1);
		glEnd();

		// bottom square
		glBegin(GL_TRIANGLES);
		glColor3f(.5,.5,.5);
		glNormal3f(0,-1,0);
		glVertex3f(-1,0,1);
		glVertex3f(-1,0,-1);
		glVertex3f(1,0,1);
		glVertex3f(1,0,1);
		glVertex3f(-1,0,-1);
		glVertex3f(1,0,-1);
		glEnd();
	}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

// position is (x, y, z), d=dimension of base, h=height,
// e = "pointienss", c = color
static void drawObilisk(float x, float y, float z,
	float d, float h, float e, unsigned char c[])
{
	cube(x, y, z ,d, h, d, c);
	pyramid(x, y+h, z, d, h/e, d, c);
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

      //  Flat or smooth shading
	glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  if(light) block from ex13.c

        //  Translate intensity to color vectors
	float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
	float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
	float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
	float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
	glColor3f(1,1,1);
	sphere(Position[0],Position[1],Position[2] , 0.1);
        //  OpenGL should normalize normal vectors
	glEnable(GL_NORMALIZE);
        //  Enable lighting
	glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
	glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
	glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
	glLightfv(GL_LIGHT0,GL_POSITION,Position);

	unsigned char red[] = {1,0,0};
	unsigned char blue[] = {0,0,1};
	unsigned char purp[] = {1,0,1};
		// row1
	drawObilisk(-1,0,0,0.15,0.75,4,red);
	drawObilisk(0,0,0,0.15,0.75,4,blue);
	drawObilisk(1,0,0,0.15,0.75,4,red);
		// row2
	drawObilisk(-1,0,0.75,0.15,0.75,4,blue);
	drawObilisk(0,0,0.75,0.15,0.75,4,red);
	drawObilisk(1,0,0.75,0.15,0.75,4,blue);
		// row3
	drawObilisk(-1,0,1.5,0.15,0.75,4,red);
	drawObilisk(0,0,1.5,0.15,0.75,4,blue);
	drawObilisk(1,0,1.5,0.15,0.75,4,red);
		// house?
	drawObilisk(0,0,-1.75,1,0.75,5,purp);

   //  White
	glColor3f(1,1,1);
   //  Render the scene
	glFlush();
   //  Make the rendered scene visible
	glutSwapBuffers();
}

// process arrow key input to change view
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
	if (key == GLUT_KEY_RIGHT)
		th += 5;
   //  Left arrow key - decrease angle by 5 degrees
	else if (key == GLUT_KEY_LEFT)
		th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
	else if (key == GLUT_KEY_UP)
		ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
	else if (key == GLUT_KEY_DOWN)
		ph -= 5;
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
		case 27: exit(0); break;
		case 'c': cubeSep -= 0.01; break;
		case 'C': cubeSep += 0.01; break;
		case '+': fov--; break;
		case '-': fov++; break;
		case 'l': light = !light; break;
		case 's': move = !move; break;
		case '<': ylight++; break;
		case '>': ylight--; break;
		case 'v': inc -= 0.1; break;
		case 'V': inc += 0.1; break;
		default: break;
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
	glutMainLoop();
	return 0;
}