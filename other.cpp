#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<gl\glut.h>
using namespace std;

#include <math.h>

#define PI 3.14159265

// 3D vector class
class CVector3D
{
public:
	float x, y, z;

	// Constructors
	CVector3D(void) { x = 0.0; y = 0.0; z = 0.0; }
	CVector3D(float x0, float y0, float z0)
	{
		x = x0; y = y0; z = z0;
	}
};
/*
	view_frame.u=CVector3D(0.0, 1.0, 0.0);
	view_frame.v=CVector3D(0.0, 0.0, 1.0);
	view_frame.n=CVector3D(1.0, 0.0, 0.0);

*/
// View reference frame class
class CViewFrame
{
public:
	float step; // step size
	float turn_a; // turn angle
	float pitch_a; // pitch angle
	float roll_a; // roll angle

	CVector3D P0; // View reference point
	CVector3D u; // unit vector in xv direction
	CVector3D v; // unit vector in yv direction
	CVector3D n; // unit vector in zv direction

	void move_up(void)
	{
		P0.x = P0.x + step * v.x;
		P0.y = P0.y + step * v.y;
		P0.z = P0.z + step * v.z;
	}

	void move_down(void)
	{
		P0.x = P0.x - step * v.x;
		P0.y = P0.y - step * v.y;
		P0.z = P0.z - step * v.z;
	}

	void move_left(void)
	{
		P0.x = P0.x - step * u.x;
		P0.y = P0.y - step * u.y;
		P0.z = P0.z - step * u.z;
	}

	void move_right(void)
	{
		P0.x = P0.x + step * u.x;
		P0.y = P0.y + step * u.y;
		P0.z = P0.z + step * u.z;
	}

	void move_forward(void)
	{
		P0.x = P0.x - step * n.x;
		P0.y = P0.y - step * n.y;
		P0.z = P0.z - step * n.z;

	}

	void move_backward(void)
	{
		P0.x = P0.x + step * n.x;
		P0.y = P0.y + step * n.y;
		P0.z = P0.z + step * n.z;
	}

	void turn_left(void)
	{
		u.x = u.x * cos(turn_a) - n.x * sin(turn_a);
		u.y = u.y * cos(turn_a) - n.y * sin(turn_a);
		u.z = u.z * cos(turn_a) - n.z * sin(turn_a);
		n.x = u.x * sin(turn_a) + n.x * cos(turn_a);
		n.y = u.y * sin(turn_a) + n.y * cos(turn_a);
		n.z = u.z * sin(turn_a) + n.z * cos(turn_a);
	}

	void turn_right(void)
	{
		u.x = u.x * cos(turn_a) + n.x * sin(turn_a);
		u.y = u.y * cos(turn_a) + n.y * sin(turn_a);
		u.z = u.z * cos(turn_a) + n.z * sin(turn_a);
		n.x = -u.x * sin(turn_a) + n.x * cos(turn_a);
		n.y = -u.y * sin(turn_a) + n.y * cos(turn_a);
		n.z = -u.z * sin(turn_a) + n.z * cos(turn_a);

	}

	void look_up(void)
	{
		v.x = v.x * cos(pitch_a) + n.x * sin(pitch_a);
		v.y = v.y * cos(pitch_a) + n.y * sin(pitch_a);
		v.z = v.z * cos(pitch_a) + n.z * sin(pitch_a);
		n.x = -v.x * sin(pitch_a) + n.x * cos(pitch_a);
		n.y = -v.y * sin(pitch_a) + n.y * cos(pitch_a);
		n.z = -v.z * sin(pitch_a) + n.z * cos(pitch_a);
	}

	void look_down(void)
	{
		v.x = v.x * cos(pitch_a) - n.x * sin(pitch_a);
		v.y = v.y * cos(pitch_a) - n.y * sin(pitch_a);
		v.z = v.z * cos(pitch_a) - n.z * sin(pitch_a);
		n.x = v.x * sin(pitch_a) + n.x * cos(pitch_a);
		n.y = v.y * sin(pitch_a) + n.y * cos(pitch_a);
		n.z = v.z * sin(pitch_a) + n.z * cos(pitch_a);
	}

	void roll_left(void)
	{
		u.x = u.x * cos(roll_a) + v.x * sin(roll_a);
		u.y = u.y * cos(roll_a) + v.y * sin(roll_a);
		u.z = u.z * cos(roll_a) + v.z * sin(roll_a);
		v.x = -u.x * sin(roll_a) + v.x * cos(roll_a);
		v.y = -u.y * sin(roll_a) + v.y * cos(roll_a);
		v.z = -u.z * sin(roll_a) + v.z * cos(roll_a);
	}

	void roll_right(void)
	{
		u.x = u.x * cos(roll_a) - v.x * sin(roll_a);
		u.y = u.y * cos(roll_a) - v.y * sin(roll_a);
		u.z = u.z * cos(roll_a) - v.z * sin(roll_a);
		v.x = u.x * sin(roll_a) + v.x * cos(roll_a);
		v.y = u.y * sin(roll_a) + v.y * cos(roll_a);
		v.z = u.z * sin(roll_a) + v.z * cos(roll_a);
	}
};

CViewFrame view_frame;

// Initialization function
void init(void)
{
	static GLfloat light_ambient[] = { 0.01, 0.01, 0.01, 1.0 };
	static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH); // Set shading model

	// Set light source properties for light source #0
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHTING); // Enable lighting
	glEnable(GL_LIGHT0); // Enable light source #0
	glEnable(GL_DEPTH_TEST); // Enable depth buffer test
	glEnable(GL_NORMALIZE); // Enable auto normalization
	glEnable(GL_CULL_FACE); // Enable face culling

	// Set initial properties for view reference frame
	view_frame.P0 = CVector3D(500.0, 0.0, 100.0);
	view_frame.u = CVector3D(0.0, 1.0, 0.0);
	view_frame.v = CVector3D(0.0, 0.0, 1.0);
	view_frame.n = CVector3D(1.0, 0.0, 0.0);
	view_frame.step = 2;
	view_frame.turn_a = PI / 18;
	view_frame.pitch_a = PI / 18;
	view_frame.roll_a = PI / 18;
}

// Function to draw chess board on xy plane
void draw_chess_board(float sx, float sy, float sz, int nx, int ny)
//draw_chess_board(400.0, 400.0, 40.0, 4, 4);
// sx, sy, sz: size of the chess board
// nx, ny: Number of chess grids in x and y direction
{
	static GLfloat mat1_color[] = { 0.8, 0.8, 0.8, 1.0 };
	static GLfloat mat2_color[] = { 0.2, 0.2, 0.2, 1.0 };
	int i, j, iflag, jflag;
	float x, y, dx, dy;
	float* pcolor;

	dx = sx / (float)nx;
	dy = sy / (float)ny;

	for (i = 0; i < nx; ++i)
	{
		iflag = i % 2;
		x = (i + 0.5) * dx - 0.5 * sx;
		for (j = 0; j < ny; ++j)
		{
			jflag = j % 2;
			y = (j + 0.5) * dy - 0.5 * sy;
			if (iflag == jflag) pcolor = mat1_color;
			else pcolor = mat2_color;
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pcolor);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pcolor);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
			glPushMatrix();
			glTranslatef(x, y, 0.0);
			glScalef(dx, dy, sz);
			glutSolidCube(1.0);
			glPopMatrix();
		}
	}

}

// Function to draw chess pieces
void draw_chess_piece(float sx, float sy, float sz, int nx, int ny)
// sx, sy, sz: size of the chess board
// nx, ny: Number of chess grids in x and y direction
{
	static GLfloat mat_color[4][4] = {
		{ 1.0, 0.0, 0.0, 1.0 },
		{ 0.0, 1.0, 0.0, 1.0 },
		{ 0.0, 0.0, 1.0, 1.0 },
		{ 1.0, 1.0, 0.0, 1.0 } };
	float x, y, dx, dy, size;

	dx = sx / (float)nx;
	dy = sy / (float)ny;
	if (dx < dy) size = dx;
	else size = dy;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Draw a sphere
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[0]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
	x = (0 + 0.5) * dx - 0.5 * sx;
	y = (2 + 0.5) * dy - 0.5 * sy;
	glPushMatrix();
	glTranslatef(x, y, 0.5 * (sz + size));
	glutSolidSphere(0.5 * size, 10, 10);
	glPopMatrix();

	// Draw a torus
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[1]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
	x = (1 + 0.5) * dx - 0.5 * sx;
	y = (0 + 0.5) * dy - 0.5 * sy;
	glPushMatrix();
	glTranslatef(x, y, 0.5 * (sz + size));
	glRotatef(-90, 1.0, 0.0, 0.0);
	glutSolidTorus(0.1 * size, 0.4 * size, 10, 20);
	glPopMatrix();

	// Draw a cone
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
	x = (2 + 0.5) * dx - 0.5 * sx;
	y = (1 + 0.5) * dy - 0.5 * sy;
	glPushMatrix();
	glTranslatef(x, y, 0.5 * sz);
	glutSolidCone(0.4 * size, size, 10, 5);
	glPopMatrix();

	// Draw a rectangular solid
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[3]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[3]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
	x = (3 + 0.5) * dx - 0.5 * sx;
	y = (3 + 0.5) * dy - 0.5 * sy;
	glPushMatrix();
	glTranslatef(x, y, 0.5 * (sz + 0.9 * size));
	glScalef(0.9 * dx, 0.9 * dy, 0.9 * size);
	glutSolidCube(1.0);
	glPopMatrix();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// Display callback function
void display(void)
{
	static GLfloat light_pos[4] = { 200.0, 200.0, 200.0, 1.0 };

	// Clear frame buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix(); //Save current ModelView matrix

	// Set viewing transformation matrix
	CVector3D look_at;
	look_at.x = view_frame.P0.x - view_frame.n.x;
	look_at.y = view_frame.P0.y - view_frame.n.y;
	look_at.z = view_frame.P0.z - view_frame.n.z;
	gluLookAt(view_frame.P0.x, view_frame.P0.y, view_frame.P0.z,
		look_at.x, look_at.y, look_at.z,
		view_frame.v.x, view_frame.v.y, view_frame.v.z);

	// Set light source position
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	// Draw the scene
	draw_chess_board(400.0, 400.0, 40.0, 4, 4);
	draw_chess_piece(400.0, 400.0, 40.0, 4, 4);

	glPopMatrix(); //Restore ModelView matrix

	glutSwapBuffers(); // Swap front and back buffer
}

// Reshape callback function
void reshape(int w, int h)
{
	float wsize = 500.0;

	// Set viewport as the entire program window
	glViewport(0, 0, w, h);

	// Set symmetric perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)h, 10.0, 100000.0);


	// Reset modelview transformation matrix to identity
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


// Keyboard callback function
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0); break;
	case 'w':
		view_frame.move_forward();
		glutPostRedisplay(); break;
	case 's'://move backward
		view_frame.move_backward();
		glutPostRedisplay(); break;
	case 'a'://move left
		view_frame.move_left();
		glutPostRedisplay(); break;
	case 'd'://move right
		view_frame.move_right();
		glutPostRedisplay(); break;
	case 'q'://roll left
		view_frame.roll_left();
		glutPostRedisplay(); break;
	case 'e'://roll right
		view_frame.roll_right();
		glutPostRedisplay(); break;
	}
}

// Special key callback function
void special_key(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		view_frame.turn_left();
		glutPostRedisplay(); break;
	case GLUT_KEY_RIGHT://turn right
		view_frame.turn_right();
		glutPostRedisplay(); break;
	case GLUT_KEY_UP://look up
		view_frame.look_up();
		glutPostRedisplay(); break;
	case GLUT_KEY_DOWN://look down
		view_frame.look_down();
		glutPostRedisplay(); break;
	case GLUT_KEY_PAGE_UP://move up
		view_frame.move_up();
		glutPostRedisplay(); break;
	case GLUT_KEY_PAGE_DOWN://move down
		view_frame.move_down();
		glutPostRedisplay(); break;
	}
}

// Main program
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 300);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("3D Scene Roaming");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special_key);
	glutMainLoop();
	return 0;
}