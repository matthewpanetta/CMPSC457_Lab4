/* CMPSC457 lab 4 start file
Your header:

.....

*/

#include <cstdlib>
#include <GL/freeglut.h>
#include <iostream>
#include <cstring>
using namespace std;

const int milliseconds_per_frame = 50;  // Time you would LIKE per frame
// The actual time can be longer.

int width;   // Current width of window; value is maintained by reshape().
int height;  // Current height of window; value is maintained by reshape().

float x_rotate = 0.0;   // Amount of rotation of the entire scene about
//    the x-axis.
float y_rotate = 0.0;   // Amount of rotation of the entire scene about
//    the y-axis.  This is applied BEFORE the x-rotation.

float rotation_factor = 7.0;

float x_rotate_global = 0.0;
float y_rotate_global = 0.0;

bool ortho = false;     // True if an orthographic projection is used,
//   false for a perspective projection.

bool animating = false;  // The value is toggled when user hits the "A" key.


/**
*  Called in the main program to give you a chance to do initialization.
*  Here, I decided to turn on anti-aliasing of lines.
*/
void init() {

	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}


/**
*  You can call this to output a string.  The characters are scaled
*  to the size specified; the size is given in drawing coordinates.
*  The x,y parameters give the location of the left endpoint of the
*  base of the string.  The string is drawn in the xy-plane.
*/
/*
void drawString(const char *str, double x=0, double y=0, double size=1.0) {
glPushMatrix();
glTranslatef(x,y,0);
glScalef(size/153.0,size/153.0,1.0);
int lineCt = 0;
int len = strlen(str);
for (int i = 0; i < len; i++) {
if (str[i] == '\n') {
lineCt++;
glPopMatrix();
glPushMatrix();
glTranslatef(x,y-size*1.15*lineCt,0);
glScalef(size/153.0,size/153.0,1.0);
}
else {
glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
}
}
glPopMatrix();
}

*/
/**
*  This will be called over and over to drive the animation.
*  Each time it is called, it triggers a new frame, which in
*  turn triggers another call to timer() in a loop unitl the
*  user turns off the animation.
*/
void timer(int id) {
	// The arms and legs use the same rotation logic.
	// Rather than rotate indefinitely, this creates a pendulum effect, which looks like the robot is walking.
	if (x_rotate >= 56)
		rotation_factor = -7;	// When the arms/legs get to a certain point, they start to reverse direction.
	else if (x_rotate <= -56)
		rotation_factor = 7;

	x_rotate += rotation_factor;
	glutPostRedisplay();
}


/**
*  Called in the "display()" function before drawing anything.
*  This function sets up the projection transformation and
*  viewing transformations that are used for the drawing.  The
*  projection is a perspective transformation that preserves
*  aspect ratio and displays at least the range of xy-values
*  from -10 to 10 in the xy-plane.  The viewpoint is at z=50
*  on the z-axis, with near and far clipping distances at 30
*  and 70.  The overall scene rotation specified in x_rotate,
*  y_rotate is also specified here.
*/
void initTransformation() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspect = ((double)height) / ((double)width);
	if (ortho) {
		if (aspect >= 1)
			glOrtho(-16, 16, -16 * aspect, 16 * aspect, 30, 70);
		else
			glOrtho(-16 / aspect, 16 / aspect, -16, 16, 30, 70);
	}
	else {
		if (aspect >= 1)
			glFrustum(-10, 10, -10 * aspect, 10 * aspect, 30, 70);
		else
			glFrustum(-10 / aspect, 10 / aspect, -10, 10, 30, 70);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 50, 0, 0, 0, 0, 1, 0);
	
	// If not animating, rotate normally
	if (!animating) {
		glRotatef(x_rotate, 1, 0, 0); 
		glRotatef(y_rotate, 0, 1, 0);
	}
	else {
		glRotatef(x_rotate_global, 1, 0, 0);	// Else, keep the same position
		glRotatef(y_rotate_global, 0, 1, 0);
	}
}


/**
*  This funciton is called in "display()" to draw the objects
*  in the scene.  The scene here shows a car.
*/
void drawObjects() {

	glScalef(3, 3, 3);  // A general object scaling (which I applied
	//   since the object was originally much too small).

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);

	// Right Eye
	glColor3f(0.8, 0.8, 0);		// The robot will have yellow eyes
	glPushMatrix();
	glTranslatef(.2, 2.3, 0.6);
	glScalef(0.15, 0.15, 0.1);
	glutWireSphere(1, 12, 12);
	glPopMatrix();

	// Left Eye
	glPushMatrix();
	glTranslatef(-.2, 2.3, 0.6);
	glScalef(0.15, 0.15, 0.1);
	glutWireSphere(1, 12, 12);
	glPopMatrix();

	// Teeth Line - Bottom
	glPushMatrix();					// This is the black outline on the robot's teeth
	glColor3f(0, 0, 0);
	glTranslatef(0, 1.7, 0.55);
	glScalef(0.7, 0.15, 0.06);
	glutWireCube(1);
	glPopMatrix();

	// Teeth Line - Top
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0, 1.85, 0.55);
	glScalef(0.7, 0.15, 0.06);
	glutWireCube(1);
	glPopMatrix();

	// Mouth
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0, 1.8, 0.55);
	glScalef(0.7, 0.3, 0.06);
	glutSolidCube(1);
	glPopMatrix();

	// Left Arm
	glPushMatrix();

	if (animating) {
		glRotatef(x_rotate, 1, 0, 0);	// Animate the arms and legs using x_rotate.
	}

	glColor3f(1, 0, 0);
	glTranslatef(-1.25, -0.3, 0);
	glScalef(0.45, 3.3, 1);
	glutSolidCube(1);
	glPopMatrix();

	// Right Arm
	glPushMatrix();

	if (animating) {
		glRotatef(x_rotate*-1, 1, 0, 0);
	}

	glColor3f(1, 0, 0);
	glTranslatef(1.25, -0.3, 0);
	glScalef(0.45, 3.3, 1);
	glutSolidCube(1);


	glPopMatrix();

	// Left Leg
	glPushMatrix();

	if (animating) {
		glRotatef((x_rotate*-1) * 0.35, 1, 0, 0);	// use a smaller rotation range for the legs
	}

	glColor3f(1, 0, 0);
	glTranslatef(-0.5, -3, 0);
	glScalef(0.65, 3, 1.2);
	glutSolidCube(1);
	glPopMatrix();

	// Right Leg
	glPushMatrix();

	if (animating) {
		glRotatef(x_rotate * 0.35, 1, 0, 0);
	}

	glColor3f(1, 0, 0);
	glTranslatef(0.5, -3, 0);
	glScalef(0.65, 3, 1.2);
	glutSolidCube(1);
	glPopMatrix();

	// Base
	glColor3f(1, 0, 0);
	glPushMatrix();
	glScalef(2, 3, 2);
	glutSolidCube(1);
	glPopMatrix();

	// Head
	glPushMatrix();
	glColor3f(1, 0, 0);
	glScalef(1, 1, 1);
	glTranslatef(0, 2, 0);
	glutSolidCube(1);
	glPopMatrix();
}


/**
*  Called whenever the windows needs to be redrawn.  It should
*  completely redraw the image that is to be displayed in the window.
*  To force it to be called, call the suboutine  glutPostRedisplay().
*/
void display() {
	if (animating) {
		// Cause display to be called again after milliseconds_per_frame.
		glutTimerFunc(milliseconds_per_frame, timer, 1);
	}
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	initTransformation();           // Setup projection and view.
	drawObjects();
	glFlush();
	glutSwapBuffers();  // Makes the drawing appear on the screen!
}


/**
*  Called when the user changes the size of the window.
*  It should not be necessary to modify this.
*/
void reshape(int new_width, int new_height) {
	glViewport(0, 0, new_width, new_height);
	height = new_height;
	width = new_width;
}


/**
*  If you uncomment the glutMouseFunc() call in the the main program,
*  then this will be called whenever the user presses or releases
*  any of the buttons on the mouse.
*/
void mouse(int button, int state, int x, int y) {
}


/**
*  If you uncomment the glutMotionFunc() call in the the main program,
*  then this will be called whenever the user moves the mouse.
*/
void motion(int x, int y) {
}


/**
*  If you uncomment the glutKeyboardFunc() call in the the main program,
*  then this will be called whenever the user types a character on
*  the keyboard.
*/
void keyboard(unsigned char key, int x, int y) {
	if (key == 27)
		exit(0);
	else if (key == 'a' || key == 'A') {
		// When a is pressed and the scene is not in animation mode, take a snapshot of the current x_rotate values (these are used in the initTransformation() method.
		// This allows the robot to stay in the same position while animating.
		if (!animating) {
			x_rotate_global = x_rotate;
			y_rotate_global = y_rotate;
		}
		else {								// When a is pressed and the scene is in animation mode, reset the x & y rotate variables
			x_rotate = x_rotate_global;		// so the robot doesn't appear to jump to a new position.
			y_rotate = y_rotate_global;
		}

		animating = !animating;
		glutPostRedisplay();
	}
	else if (key == 'p' || key == 'P') {
		ortho = !ortho;
		if (!animating)
			glutPostRedisplay();
	}
}


/**
*  If you uncomment the glutSpecialFunc() call in the the main program,
*  then this will be called when the user presses certain special
*  keys on the keyboard, such  as the arrow keys and function keys.
*/
void special(int key, int x, int y) {
	if (animating)
		return;
	if (key == GLUT_KEY_LEFT) {
		y_rotate -= 15;
		glutPostRedisplay();
	}
	else if (key == GLUT_KEY_RIGHT) {
		y_rotate += 15;
		glutPostRedisplay();
	}
	else if (key == GLUT_KEY_UP) {
		x_rotate += 15;
		glutPostRedisplay();
	}
	else if (key == GLUT_KEY_DOWN) {
		x_rotate -= 15;
		glutPostRedisplay();
	}
	else if (key == GLUT_KEY_HOME) {
		x_rotate = y_rotate = 0;
		glutPostRedisplay();
	}
}


/**
*  If you uncomment the glutIdleFunc() call in the the main program,
*  then this will be called whenever the program does not have any
*  other events to process.  It is can be called repeatedly
*  while the program is idle.
*/
void idle() {
}


int main(int argc, char **argv) {

	cout << "\nInstructions:\n";
	cout << "   Arrow Keys rotate the object.\n";
	cout << "   The Home key restores the object to its default orientation.\n";
	cout << "   The 'A' key starts and stops an animation.\n";
	cout << "        (Arrow and Home keys don't function during animation.)\n";
	cout << "   The 'P' key switches between perspective and orthographic "
		<< "projection.\n";
	cout << "   The Escape key ends the program.\n\n";

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);       // Set the size of the window here.
	glutInitWindowPosition(150, 50);    // Upper left corner of window.
	glutCreateWindow("OpenGL Window"); // Title displayed in window title bar.

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	//glutMouseFunc(mouse);       // Uncomment to enable mouse handling.
	//glutMotionFunc(motion);     // Uncomment to enable mouse motion handling. 
	glutKeyboardFunc(keyboard); // Uncomment to enable ASCII key handling.
	glutSpecialFunc(special);   // Uncomment to enable special key handling.
	//glutIdleFunc(idle);         // Uncomment to enable the idle function.

	init();
	glutMainLoop();
}