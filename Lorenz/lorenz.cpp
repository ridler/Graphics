// Modifiable Lorez Attractor:
// By Forrest Tagg Ridler | forrest.ridler@colorado.edu

// Animation structure based on gears.c from the previous assignment

#include <cstdlib>
#include <stdio.h>
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

static GLint win = 0;

// scale for zooming
double SCALE = 0.8;
// Font for bitmap screen writing
void* FONT = GLUT_BITMAP_HELVETICA_12;

// Lorenz parameters
double s  = 10;
double b  = 2.6666;
double r  = 28;

// amount parameters change per key hit
double dS = 0.1;
double dB = 0.01;
double dR = 0.1;

// toggling variable for displaying parameters on screen
char showInfo = 1;

// rotation variables
static float view_rotx = 0.0, view_roty = 0.0, view_rotz = 0.0;

// write a formatted string to the screen
// from http://stackoverflow.com/questions/4917403/displaying-variables-in-glut
inline void renderBitmapString(float x, float y, void *font, char *string) 
{  
	char *c;
	glRasterPos2f(x,y);
	for (c=string; *c != '\0'; c++) 
		{ glutBitmapCharacter(font, *c); }
}

// draw the attractor and print the current equation parameters
static void draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	{
		glRotatef(view_rotx, 1.0, 0.0, 0.0);
		glRotatef(view_roty, 0.0, 1.0, 0.0);
		glRotatef(view_rotz, 0.0, 0.0, 1.0);

		glPushMatrix();

		{
			// draw x, y, z axis:
			glBegin(GL_LINES);
			glColor3f(1,0,0);
			glVertex3f(-50,0,0);
			glVertex3f(50,0,0);
			glColor3f(0,1,0);
			glVertex3f(0,-50,0);
			glVertex3f(0,50,0);
			glColor3f(0,0,1);
			glVertex3f(0,0,-50);
			glVertex3f(0,0,50);
			glEnd();

			unsigned int i;
			// 'smoothness' parameters:
			int its = 50000;
			double dt = 0.001;

			// define start coordinates:
			double x = 1;
			double y = 1;
			double z = 1;

			// draw attractor:
			glColor3f(1,1,1);
			glBegin(GL_LINE_STRIP);
			glVertex3f(x, y, z);

			for(i = 0; i < its; i++)
			{
				double dx = s*(y-x);
				double dy = x*(r-z)-y;
				double dz = x*y - b*z;
				x += dt*dx;
				y += dt*dy;
				z += dt*dz;
				glVertex3f(x, y, z);
			}
			glEnd();
		}
		glPopMatrix();
	}
	glPopMatrix();

	// display current parameters:
	if(showInfo)
	{
		char buffer[64];
		sprintf(buffer, "s = %f\t\tb = %f\t\tr = %f", s, b, r);
		renderBitmapString(0, 0, FONT, buffer);
	}

	glutSwapBuffers();
}

	// user can change equation parameters, z-axis viewing angle, and exit
static void key(unsigned char k, int x, int y)
{
	switch (k)
	{
		case 's':
		s -= dS;
		break;
		case 'S':
		s += dS;
		break;
		case 'b':
		b -= dB;
		break;
		case 'B':
		b += dB;
		break;
		case 'r':
		r -= dR;
		break;
		case 'R':
		r += dR;
		break;
		case '+':
		glScaled(1./SCALE, 1./SCALE, 1./SCALE);
		break;
		case '-':
		glScaled(SCALE, SCALE, SCALE);
		break;
		case 'z':
		view_rotz += 5.0;
		break;
		case 'Z':
		view_rotz -= 5.0;
		break;
		case 'i':
		showInfo = !showInfo;
		break;
		case 27:  // Escape
		exit(0);
		break;
		default:
		return;
	}
	glutPostRedisplay();
}

	// user can change viewing angle with arrow keys
static void special(int k, int x, int y)
{
	switch (k)
	{
		case GLUT_KEY_UP:
		view_rotx += 5.0;
		break;
		case GLUT_KEY_DOWN:
		view_rotx -= 5.0;
		break;
		case GLUT_KEY_LEFT:
		view_roty += 5.0;
		break;
		case GLUT_KEY_RIGHT:
		view_roty -= 5.0;
		break;
		default:
		return;
	}
	glutPostRedisplay();
}

	// Handle window shaping and reshaping
static void reshape(int width, int height)
{
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

		// Frustum From: http://openglut.sourceforge.net/lorenz_8c.html
	double ar =  width * 1.0 / height;
	if( ar > 1.0 )
		{ glFrustum( -ar, ar, -1.0, 1.0, 2.0, 500.0 ); }
	else
		{ glFrustum( -1.0, 1.0, -1/ar, 1/ar, 2.0, 500.0 ); }

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -80.0);
}

	// Print instructions on startup
inline void intro()
{
	printf("\nStrange Attractor Instructions:\n\n");
	printf("'+' = zoom in\n");
	printf("'-' = zoom out\n");
	printf("'z/Z' = rotate z-axis\n");
	printf("arrow keys = natural rotation\n");
	printf("'s' = decrease s parameter (increase with 'S')\n");
	printf("'b' = decrease b parameter (increase with 'B')\n");
	printf("'r' = decrease r parameter (increase with 'R')\n");
	printf("'i' = toggle on-screne display of the parameters\n");
	printf("'ESC' = terminate program\n");
	printf("\nPress ENTER to continue.\n");
	getchar();
	return;
}

int main(int argc, char *argv[])
{
	//intro();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	glutInitWindowPosition(1, 1);
	glutInitWindowSize(600, 600);
	win = glutCreateWindow("Lorenz");
	glEnable(GL_NORMALIZE);

	glutDisplayFunc(draw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutSpecialFunc(special);

	glutMainLoop();
	return 0;
}