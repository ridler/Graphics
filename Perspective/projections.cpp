// Scenes without textures or lighting
// By Forrest Tagg Ridler | forrest.ridler@colorado.edu

// Animation structure and basic shapes based on ex8.c
// Projection set-up and calculations based on ex9.c

#include <cstdlib>
#include <cmath>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int th=0;	//  Azimuth of view angle
int ph=0;	//  Elevation of view angle

unsigned char obilisk = 1;	// toggle objects displayed
unsigned char spheres = 0;	// toggle objects displayed
unsigned char perspec = 0;	// toggle display mode

int fov = 50;	// field of view

float asp=1.0;	//  Aspemct ratio
float dim=5.0;	//  Size of world

float cubeSep = 0.05;	// user can change separation of sheres

//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))


 static inline void Vertex(double th,double ph)
 { glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph)); }

 static void sphere(float x, float y, float z, float r)
 {
 	const unsigned char d = 5;
 	short th,ph;
 	glPushMatrix();
 	{
 		glTranslated(x,y,z);
 		glScaled(r,r,r);
   		//  Latitude bands
 		for (ph=-90;ph<90;ph+=d)
 		{
 			glBegin(GL_QUAD_STRIP);
 			for (th=0;th<=360;th+=d)
 			{
 				Vertex(th,ph);
 				Vertex(th,ph+d);
 			}
 			glEnd();
 		}
 	}
 	glPopMatrix();
 }

// creates cubical lattice of spheres
 static void sphereLattice(float x, float y, float z,
 	float r, char dim, float dist)
 {
 	dist += 2*r;
 	for(unsigned char i = 0; i < dim; i++)
 	{
 		for(unsigned char j = 0; j < dim; j++)
 		{
 			for(unsigned char k = 0; k < dim; k++)
 			{
 				switch(i%3)
 				{
 					case 0: glColor3f(1,0,0); break;
 					case 1: glColor3f(0,1,0); break;
 					case 2: glColor3f(0,0,1); break;
 					default: exit(1); break;
 				}
 				sphere(dist*(x+i), dist*(y+j), dist*(z+k), r);
 			}
 		}
 	}
 }

 static void cube(float x, float y, float z,
 	float dx, float dy, float dz, unsigned char c[])
 {
   //  Save transformation
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
 		glVertex3f(-1,-1, 1);
 		glVertex3f(+1,-1, 1);
 		glVertex3f(+1,+1, 1);
 		glVertex3f(-1,+1, 1);
   //  Back
 		glColor3f(r,g,b);
 		glVertex3f(+1,-1,-1);
 		glVertex3f(-1,-1,-1);
 		glVertex3f(-1,+1,-1);
 		glVertex3f(+1,+1,-1);
   //  Right
 		glColor3f(1,1,1);
 		glVertex3f(+1,-1,+1);
 		glVertex3f(+1,-1,-1);
 		glVertex3f(+1,+1,-1);
 		glVertex3f(+1,+1,+1);
   //  Left
 		glColor3f(1,1,1);
 		glVertex3f(-1,-1,-1);
 		glVertex3f(-1,-1,+1);
 		glVertex3f(-1,+1,+1);
 		glVertex3f(-1,+1,-1);
   //  Top
 		glColor3f(0.5,0.5,0.5);
 		glVertex3f(-1,+1,+1);
 		glVertex3f(+1,+1,+1);
 		glVertex3f(+1,+1,-1);
 		glVertex3f(-1,+1,-1);
   //  Bottom
 		glColor3f(0.5,0.5,0.5);
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
 	glPushMatrix();
 	{
 		glTranslated(x,y,z);
 		glScaled(dx,dy,dz);

 		glBegin(GL_TRIANGLES);
 		glNormal3f(1,0,1); 

 		glColor3f(1,1,1);
		// first triangle
		glVertex3f(1,0,1);
		glNormal3f(0,2,0);
		glVertex3f(0,2,0);
		glNormal3f(-1,0,1);
		glVertex3f(-1,0,1);
		glNormal3f(-1,0,1); 

		glColor3f(r,g,b);
		// second triangle
		glVertex3f(-1,0,1);
		glNormal3f(0,2,0);
		glVertex3f(0,2,0);
		glNormal3f(-1,0,-1);
		glVertex3f(-1,0,-1);
		glNormal3f(-1,0,-1);

		glColor3f(1,1,1);
		// third triangle
		glVertex3f(-1,0,-1);
		glNormal3f(0,2,0);
		glVertex3f(0,2,0);
		glNormal3f(1,0,-1);
		glVertex3f(1,0,-1);
		glNormal3f(1,0,-1); 

		glColor3f(r,g,b);
		// last triangle
		glVertex3f(1,0,-1);
		glNormal3f(0,2,0);
		glVertex3f(0,2,0);
		glNormal3f(1,0,1);
		glVertex3f(1,0,1);
		glEnd();

		// bottom square
		glColor3f(.5,.5,.5);
		glBegin(GL_TRIANGLES);
		glNormal3f(-1,0,1);
		glVertex3f(-1,0,1);
		glNormal3f(-1,0,-1);
		glVertex3f(-1,0,-1);
		glNormal3f(1,0,1);
		glVertex3f(1,0,1);
		glNormal3f(1,0,1);
		glVertex3f(1,0,1);
		glNormal3f(-1,0,-1);
		glVertex3f(-1,0,-1);
		glNormal3f(1,0,-1);
		glVertex3f(1,0,-1);
		glEnd();
	}
	glPopMatrix();
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
   if (perspec) { gluPerspective(fov,asp,dim/4,4*dim); }
   //  Orthogonal projection
   else
   	{ glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim); }

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
   //  Set view angle
	if (perspec)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

	if(spheres) { sphereLattice(0,0,0,0.1,3,cubeSep); }
	else if(obilisk)
	{
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
	}
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
 		case 'm': obilisk = !obilisk; spheres = !spheres; break;
 		case 'c': cubeSep -= 0.01; break;
 		case 'C': cubeSep += 0.01; break;
 		case '+': fov--; break;
 		case '-': fov++; break;
 		case 'p': perspec = !perspec;
 		default: break;
 	}
 	// make it more of a zoom-in feature:
 	if(fov < 0) { fov = 0; }

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
 	glutMainLoop();
 	return 0;
 }