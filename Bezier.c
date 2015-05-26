/**
 *	@author:			Boris Boiko
 *	@id:					sd45774
 *	@email:				b.sboiko@umbc.edu
 *	@description:	Animation following a Bezier spline
 *								Features a bouncing ball and lamp
 */
#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>

// CONSTANTS

float const START_TIME = 0.0; // animation start time
float const END_TIME = 4.0; // animation end time
int const BASIS_POINTS = 4; // four basis points
float const FPS = 30.0; // frames per second
int const BOUNCES = 4; // four curves to the bounces animation

// define the scene
int const LEFT = 0;
int const RIGHT = 40;
int const BOTTOM = -20;
int const TOP = 20;

// initial start window size
int WIN_WIDTH = 1024;
int WIN_HEIGHT = 1024;

// Point, x,y,z points, and rgb color
typedef struct Point {
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat r;
	GLfloat g;
	GLfloat b;
} POINT;

int control_points = 78; // all poitns
int ball_control_points = 13; // ball points
int lamp_control_points = 65; // lamp points
int selected_control = -1;
POINT *p; // control points
POINT *b; // basis points

// point used for selection
POINT selected_point;

// position of the bouncing ball and radius
POINT ball;
int radius = 2;

// position of the lamp and the joint rotations
POINT lamp;
float joint_1_rot;
float joint_2_rot;
float joint_3_rot;
float joint_4_rot;

//time of the animation
float time;
float delta_t = 1.0 / FPS; // inverse of FPS

// bools for user interaction
bool ease = false; // easeing on/off
bool scene1 = true; // which scene
bool hide = false; // option to hide the control points

/**
 * Hold the initialization of the ball control poinss
 */
void InitBallAnimation(void) {
		
	// first bounce
	(p + 0)->x = 0.0;
	(p + 0)->y = 10.0;
	(p + 1)->x = 10.0;
	(p + 1)->y = 12.0;
	(p + 2)->x = 10.0;
	(p + 2)->y = 0.0;
	(p + 3)->x = 10.0;
	(p + 3)->y = -10.0;

	// 3 is a shared point
	// second bounce
	(p + 4)->x = 12.0;
	(p + 4)->y = 8.0;
	(p + 5)->x = 18.0;
	(p + 5)->y = 8.0;
	(p + 6)->x = 20.0;
	(p + 6)->y = -10.0;

	// 6 is a shared point
	// second bounce
	(p + 7)->x = 22.0;
	(p + 7)->y = 2.0;
	(p + 8)->x = 28.0;
	(p + 8)->y = 2.0;
	(p + 9)->x = 30.0;
	(p + 9)->y = -10.0;

	// 9 is a shared point
	// second bounce
	(p + 10)->x = 32.0;
	(p + 10)->y = -2.0;
	(p + 11)->x = 38.0;
	(p + 11)->y = -2.0;
	(p + 12)->x = 40.0;
	(p + 12)->y = -10.0;

	int i;

	// set all z to 0, default color to red
	for (i = 0; i < control_points; i++) {

		(p + i)->z = 0.0;
		(p + i)->r = 1;
		(p + i)->g = 0;
		(p + i)->b = 0;
	}

	// starting position for the ball
	ball.x = -20;
	ball.y = 0;
	ball.z = 0;
}

/**
 * Init the position of the lamp
 */
void InitLampPosition(void) {

	// FIRST BOUNCE - POSITION
	(p + 13)->x = 0.0;
	(p + 13)->y = -10.0;
	(p + 14)->x = 2.0;
	(p + 14)->y = 10.0;
	(p + 15)->x = 8.0;
	(p + 15)->y = 10.0;
	(p + 16)->x = 10.0;
	(p + 16)->y = -10.0;

	// 16 is a shared point
	// second bounce
	(p + 17)->x = 12.0;
	(p + 17)->y = 10.0;
	(p + 18)->x = 18.0;
	(p + 18)->y = 10.0;
	(p + 19)->x = 20.0;
	(p + 19)->y = -10.0;

	// 20 is a shared point
	// second bounce
	(p + 20)->x = 22.0;
	(p + 20)->y = 10.0;
	(p + 21)->x = 28.0;
	(p + 21)->y = 10.0;
	(p + 22)->x = 30.0;
	(p + 22)->y = -10.0;

	// 22 is a shared point
	// second bounce
	(p + 23)->x = 32.0;
	(p + 23)->y = 10.0;
	(p + 24)->x = 38.0;
	(p + 24)->y = 10.0;
	(p + 25)->x = 40.0;
	(p + 25)->y = -10.0;

	int i;

	// set all z to 0, default color to red
	for (i = 13; i < 26; i++) {

		(p + i)->z = 0.0;
		(p + i)->r = 1;
		(p + i)->g = 0;
		(p + i)->b = 0;
	}

	// starting position and rotations for the lamp
	lamp.x = -20;
	lamp.y = -10;
	lamp.z = 0;
}

/**
 * Init control points for joint 1 rotation
 */
void InitLampJoint1(void) {

	// - bounce 1
	(p + 26)->x = 0.0;
	(p + 26)->y = 0.0;
	(p + 27)->x = 4.0;
	(p + 27)->y = M_PI / 2.0;
	(p + 28)->x = 6.0;
	(p + 28)->y = M_PI / 2.0;
	(p + 29)->x = 10.0;
	(p + 29)->y = 0.0;

	// - bounce 2
	(p + 30)->x = 14.0;
	(p + 30)->y = M_PI / 2.0;
	(p + 31)->x = 16.0;
	(p + 31)->y = M_PI / 2.0;
	(p + 32)->x = 20.0;
	(p + 32)->y = 0.0;

	// - bounce 3
	(p + 33)->x = 24.0;
	(p + 33)->y = M_PI / 2.0;
	(p + 34)->x = 26.0;
	(p + 34)->y = M_PI / 2.0;
	(p + 35)->x = 30.0;
	(p + 35)->y = 0.0;

	// - bounce 4
	(p + 36)->x = 34.0;
	(p + 36)->y = M_PI / 2.0;
	(p + 37)->x = 36.0;
	(p + 37)->y = M_PI / 2.0;
	(p + 38)->x = 40.0;
	(p + 38)->y = 0.0;

	int i;

	// set all z to 0, default color to blue-green
	for (i = 26; i < 39; i++) {

		(p + i)->z = 0.0;
		(p + i)->r = 0;
		(p + i)->g = 1;
		(p + i)->b = 1;
	}

	joint_1_rot = 0;
}

/**
 * Init control points for joint 2 rotation
 */
void InitLampJoint2(void) {

	// - bounce 1
	(p + 39)->x = 0.0;
	(p + 39)->y = M_PI / 2.0;
	(p + 40)->x = 2.0;
	(p + 40)->y = -M_PI / 2.0;
	(p + 41)->x = 8.0;
	(p + 41)->y = -M_PI / 2.0;
	(p + 42)->x = 10.0;
	(p + 42)->y = M_PI / 2.0;

	// - bounce 2
	(p + 43)->x = 12.0;
	(p + 43)->y = -M_PI / 2.0;
	(p + 44)->x = 18.0;
	(p + 44)->y = -M_PI / 2.0;
	(p + 45)->x = 20.0;
	(p + 45)->y = M_PI / 2.0;

	// - bounce 3
	(p + 46)->x = 22.0;
	(p + 46)->y = -M_PI / 2.0;
	(p + 47)->x = 28.0;
	(p + 47)->y = -M_PI / 2.0;
	(p + 48)->x = 30.0;
	(p + 48)->y = M_PI / 2.0;

	// - bounce 4
	(p + 49)->x = 32.0;
	(p + 49)->y = -M_PI / 2.0;
	(p + 50)->x = 38.0;
	(p + 50)->y = -M_PI / 2.0;
	(p + 51)->x = 40.0;
	(p + 51)->y = M_PI / 2.0;

	int i;

	// set all z to 0, default color to purple
	for (i = 39; i < 52; i++) {

		(p + i)->z = 0.0;
		(p + i)->r = 1;
		(p + i)->g = 0;
		(p + i)->b = 1;
	}

	joint_2_rot = 60;
}

/**
 * Init controls for the lamp joint 3 rotation
 */
void InitLampJoint3(void) {

	// - bounce 1
	(p + 52)->x = 0.0;
	(p + 52)->y = 2.5 * M_PI / 3.0;
	(p + 53)->x = 2.0;
	(p + 53)->y = 0;
	(p + 54)->x = 8.0;
	(p + 54)->y = 0;
	(p + 55)->x = 10.0;
	(p + 55)->y = 2.5 * M_PI / 3.0;

	// - bounce 2
	(p + 56)->x = 12.0;
	(p + 56)->y = 0;
	(p + 57)->x = 18.0;
	(p + 57)->y = 0;
	(p + 58)->x = 20.0;
	(p + 58)->y = 2.5 * M_PI / 3.0;

	// - bounce 3
	(p + 59)->x = 22.0;
	(p + 59)->y = 0;
	(p + 60)->x = 28.0;
	(p + 60)->y = 0;
	(p + 61)->x = 30.0;
	(p + 61)->y = 2.5 * M_PI / 3.0;

	// - bounce 4
	(p + 62)->x = 32.0;
	(p + 62)->y = 0;
	(p + 63)->x = 38.0;
	(p + 63)->y = 0;
	(p + 64)->x = 40.0;
	(p + 64)->y = 2.5 * M_PI / 3.0;

	int i;

	// set all z to 0, default color to green
	for (i = 52; i < 65; i++) {

		(p + i)->z = 0.0;
		(p + i)->r = 0;
		(p + i)->g = 1;
		(p + i)->b = 0;
	}

	joint_3_rot = 120;
}

/**
 * Init controls for the lamp joint 4 rotation
 */
void InitLampJoint4(void) {

	// - bounce 1
	(p + 65)->x = 0.0;
	(p + 65)->y = 2.0 * M_PI / 3.0;
	(p + 66)->x = 2.0;
	(p + 66)->y = M_PI / 2.0;
	(p + 67)->x = 8.0;
	(p + 67)->y = M_PI / 2.0;
	(p + 68)->x = 10.0;
	(p + 68)->y = 2.0 * M_PI / 3.0;

	// - bounce 2
	(p + 69)->x = 12.0;
	(p + 69)->y = M_PI / 2.0;
	(p + 70)->x = 18.0;
	(p + 70)->y = M_PI / 2.0;
	(p + 71)->x = 20.0;
	(p + 71)->y = 2.0 * M_PI / 3.0;

	// - bounce 3
	(p + 72)->x = 22.0;
	(p + 72)->y = M_PI / 2.0;
	(p + 73)->x = 28.0;
	(p + 73)->y = M_PI / 2.0;
	(p + 74)->x = 30.0;
	(p + 74)->y = 2.0 * M_PI / 3.0;

	// - bounce 4
	(p + 75)->x = 32.0;
	(p + 75)->y = M_PI / 2.0;
	(p + 76)->x = 38.0;
	(p + 76)->y = M_PI / 2.0;
	(p + 77)->x = 40.0;
	(p + 77)->y = 2.0 * M_PI / 3.0;
	int i;

	// set all z to 0, default color to orange
	for (i = 65; i < 78; i++) {

		(p + i)->z = 0.0;
		(p + i)->r = 1;
		(p + i)->g = .5;
		(p + i)->b = 0;
	}
	
	joint_4_rot = 120;
}

/**
 * Init all control points for the lamp
 */
void InitLampAnimation(void) {

	InitLampPosition(); // set position controls

	InitLampJoint1(); // joint 1 rot

	InitLampJoint2(); // joint 2 rot

	InitLampJoint3(); // joint 3 rot

	InitLampJoint4(); // joint 4 rot
}

/**
 * Initialize some stuff for the program
 */
void init(void)
{
  /* background color: black */
  glClearColor(0, 0, 0, 0);
  glShadeModel(GL_FLAT);

	p = (POINT *)malloc(control_points * sizeof(POINT)); // array of control points
	b = (POINT *)malloc(BASIS_POINTS * sizeof(POINT)); // array for basis points

	// init stuff for the ball animation
	InitBallAnimation();

	// init stuff for the lamp animation
	InitLampAnimation();

	time = 0; // time init to 0
}

/**
 * Free malloced memory
 */
void FreeMem(void) {

	free(p);
	free(b);
}

/**
 * Given control points, this will compute the basis points
 * All of these equations came from the notes and slides from
 * class, specifically the slides from Dr. Olano
 */
void BezierBasis(POINT p0, POINT p1, POINT p2, POINT p3) {

	// basis 0 equation [ -1 3 3 1 ]
	(b + 0)->x = -p0.x + 3 * p1.x - 3 * p2.x + p3.x;
	(b + 0)->y = -p0.y + 3 * p1.y - 3 * p2.y + p3.y;
	(b + 0)->z = -p0.z + 3 * p1.z - 3 * p2.z + p3.z;
	
	// basis 1 equation [ 3 -6 3 0 ]
	(b + 1)->x = 3 * p0.x - 6 * p1.x + 3 * p2.x;
	(b + 1)->y = 3 * p0.y - 6 * p1.y + 3 * p2.y;
	(b + 1)->z = 3 * p0.z - 6 * p1.z + 3 * p2.z;

	// basis 2 equation [ -3 3 0 0 ]
	(b + 2)->x = -3 * p0.x + 3 * p1.x;
	(b + 2)->y = -3 * p0.y + 3 * p1.y;
	(b + 2)->z = -3 * p0.z + 3 * p1.z;

	// basis 3 equation [ 1 0 0 0 ]
	(b + 3)->x = p0.x;
	(b + 3)->y = p0.y;
	(b + 3)->z = p0.z;
}

/**
 * Pass in pointers and calculate the position using bezier basis and a time param
 */
void GetPoint(float *x, float *y, float *z, float ani_time) {

	// [ t^3 t^2 t 1 ] * [ b0 b1 b2 b3 ] Transpose
	*x = pow(ani_time, 3) * (b + 0)->x + pow(ani_time, 2) * (b + 1)->x + (ani_time) * (b + 2)->x + (b + 3)->x;
	*y = pow(ani_time, 3) * (b + 0)->y + pow(ani_time, 2) * (b + 1)->y + (ani_time) * (b + 2)->y + (b + 3)->y;
	*z = pow(ani_time, 3) * (b + 0)->z + pow(ani_time, 2) * (b + 1)->z + (ani_time) * (b + 2)->z + (b + 3)->z;
}

/**
 * Convert radians to degrees
 */
void RadToDeg(float *rad) {

	*rad = (*rad * (180.0 / M_PI));
}

/**
 * Pass in x, y, z and apply easing (optional) and then get points
 */
void Animation(float *ani_x, float *ani_y, float *ani_z) {
	
	float ani_time = time;

	// apply a slight ease in function to the first bounce
	if (ease) {

		if (ani_time < 1.0) {

			ani_time = pow(ani_time, 2); // square time on the first bounce
		}
	}
	
	ani_time = fmodf(ani_time, 1.0); // set to to interval [0 1]

	GetPoint(ani_x, ani_y, ani_z, ani_time); // get the point and set to pointers
}

/**
 * Run the ball animation
 */
void BallAnimation(void) {

	// derive basis equations based on time
	if (time > 3) { // fourth bounce
		
		BezierBasis(*(p + 9), *(p + 10), *(p + 11), *(p + 12));
	} else if (time > 2) { // third bounce

		BezierBasis(*(p + 6), *(p + 7), *(p + 8), *(p + 9));
	} else if (time > 1) { // second bounce

		BezierBasis(*(p + 3), *(p + 4), *(p + 5), *(p + 6));
	} else if (time > 0) { // first bounce

		BezierBasis(*(p + 0), *(p + 1), *(p + 2), *(p + 3));
	}

	// get the correct point
	Animation(&ball.x, &ball.y, &ball.z);
}

/**
 * Animate the lamp. Includes the bounce and joint rotations
 */
void LampAnimation(void) {

	float a, b; // useless variables, just for temp usage

	// derive basis equations based on time
	if (time > 3) { // fourth bounce
		
		BezierBasis(*(p + 22), *(p + 23), *(p + 24), *(p + 25));

		// get the correct point
		Animation(&lamp.x, &lamp.y, &lamp.z);

		BezierBasis(*(p + 35), *(p + 36), *(p + 37), *(p + 38));

		// get the correct point
		Animation(&a, &joint_1_rot, &b);

		BezierBasis(*(p + 48), *(p + 49), *(p + 50), *(p + 51));

		// get the correct point
		Animation(&a, &joint_2_rot, &b);

		BezierBasis(*(p + 61), *(p + 62), *(p + 63), *(p + 64));

		// get the correct point
		Animation(&a, &joint_3_rot, &b);

		BezierBasis(*(p + 74), *(p + 75), *(p + 76), *(p + 77));

		// get the correct point
		Animation(&a, &joint_4_rot, &b);
	} else if (time > 2) { // third bounce

		BezierBasis(*(p + 19), *(p + 20), *(p + 21), *(p + 22));

		// get the correct point
		Animation(&lamp.x, &lamp.y, &lamp.z);

		BezierBasis(*(p + 32), *(p + 33), *(p + 34), *(p + 35));

		// get the correct point
		Animation(&a, &joint_1_rot, &b);

		BezierBasis(*(p + 45), *(p + 46), *(p + 47), *(p + 48));

		// get the correct point
		Animation(&a, &joint_2_rot, &b);

		BezierBasis(*(p + 58), *(p + 59), *(p + 60), *(p + 61));

		// get the correct point
		Animation(&a, &joint_3_rot, &b);

		BezierBasis(*(p + 71), *(p + 72), *(p + 73), *(p + 74));

		// get the correct point
		Animation(&a, &joint_4_rot, &b);
	} else if (time > 1) { // second bounce

		BezierBasis(*(p + 16), *(p + 17), *(p + 18), *(p + 19));

		// get the correct point
		Animation(&lamp.x, &lamp.y, &lamp.z);

		BezierBasis(*(p + 29), *(p + 30), *(p + 31), *(p + 32));

		// get the correct point
		Animation(&a, &joint_1_rot, &b);

		BezierBasis(*(p + 42), *(p + 43), *(p + 44), *(p + 45));

		// get the correct point
		Animation(&a, &joint_2_rot, &b);

		BezierBasis(*(p + 55), *(p + 56), *(p + 57), *(p + 58));

		// get the correct point
		Animation(&a, &joint_3_rot, &b);

		BezierBasis(*(p + 68), *(p + 69), *(p + 70), *(p + 71));

		// get the correct point
		Animation(&a, &joint_4_rot, &b);
	} else if (time > 0) { // first bounce

		BezierBasis(*(p + 13), *(p + 14), *(p + 15), *(p + 16));

		// get the correct point
		Animation(&lamp.x, &lamp.y, &lamp.z);

		BezierBasis(*(p + 26), *(p + 27), *(p + 28), *(p + 29));

		// get the correct point
		Animation(&a, &joint_1_rot, &b);

		BezierBasis(*(p + 39), *(p + 40), *(p + 41), *(p + 42));

		// get the correct point
		Animation(&a, &joint_2_rot, &b);

		BezierBasis(*(p + 52), *(p + 53), *(p + 54), *(p + 55));

		// get the correct point
		Animation(&a, &joint_3_rot, &b);

		BezierBasis(*(p + 65), *(p + 66), *(p + 67), *(p + 68));

		// get the correct point
		Animation(&a, &joint_4_rot, &b);
	}

	// convert all to degrees
	RadToDeg(&joint_1_rot);
	RadToDeg(&joint_2_rot);
	RadToDeg(&joint_3_rot);
	RadToDeg(&joint_4_rot);
}

/**
 * Idle function for my animation
 */
void Idle(void) {

	// loop back to the beginning
	if (time + delta_t >= END_TIME) {

		time = START_TIME;
	}
	
	time += delta_t; // move to next frame

	// pick which scene to animate and display
	if (scene1) {

		BallAnimation(); // the bouncing ball
	} else {

		LampAnimation(); // the bouncing lamp
	}

  glutPostRedisplay(); // redraw
}

/**
 * Draw the ground for the ball to bounce on
 */
void DrawGround(void) {

  glPushMatrix();

		// This draws a green line that signifies the ground
		glColor3f(0, 1, 0);
		glBegin(GL_LINES);

			glVertex3f(LEFT, BOTTOM / 2, 0);
			glVertex3f(RIGHT, BOTTOM / 2, 0);
		glEnd();
	glPopMatrix();
}

/**
 * This will draw all of the control points as small squares
 */
void DrawControlPoints(void) {

	int i, start, end;

	glPushMatrix();
	
		// bouncing ball control points
		if (scene1) {

			start = 0;
			end = ball_control_points;
		} else { // for the lamp

			start = ball_control_points;
			end = control_points;
		}

		// loop over all the control points
		for (i = start; i < end; i++) {
	
			if (i == selected_control) {

				glColor3f(1, 1, 0); // yellow for selected
			} else {
			
				// color of the point
				glColor3f((p + i)->r, (p + i)->g, (p + i)->b);
			}
			
			// draw the actual point
			glBegin(GL_POINTS);

				glVertex3f((p + i)->x, (p + i)->y, 0);
			glEnd();

			// draw a bounding box around the point
			glBegin(GL_LINE_LOOP);

				glVertex3f((p + i)->x - 0.5, (p + i)->y - 0.5, 0);
				glVertex3f((p + i)->x - 0.5, (p + i)->y + 0.5, 0);
				glVertex3f((p + i)->x + 0.5, (p + i)->y + 0.5, 0);
				glVertex3f((p + i)->x + 0.5, (p + i)->y - 0.5, 0);
			glEnd();
		}
	glPopMatrix();
}

/**
 * This will draw the points that represent the animation curve
 */
void DrawAnimationPoints(void) {

	int i, offset;
	float point_x, point_y, point_z, j, init_time, new_time, ani_time, temp_time;

	ani_time = FPS;

	init_time = START_TIME; // set time to start

	// draw all the points
	glBegin(GL_POINTS);
	
	// draw for all the bounces
	for (i = 0; i < BOUNCES; i++) {

		temp_time = init_time;

		glColor3f(1, 0, 0); // red for position

		if (scene1) {

			offset = i * 3;
		} else {

			offset = (i * 3) + ball_control_points;
		}

		// get the bezier basis for each separate bezier curve. End and start points are shared
		BezierBasis(*(p + offset), *(p + offset + 1), *(p + offset + 2), *(p + offset + 3));

		// for the curve, draw all the points
		for (j = 0; j < ani_time; j++) {

			init_time += delta_t;

			new_time = init_time;

			// ease in function on the first bounce
			if (ease) {

				if (init_time < 1.0) {

					new_time = pow(init_time, 2); // square time on the first bounce
				}
			}
			new_time = fmodf(new_time, 1.0); // set to to interval [0 1]

			// get point at that time, draw it
			GetPoint(&point_x, &point_y, &point_z, new_time);
			glVertex3f(point_x, point_y, point_z);
		}

		// draw rotation curves for the lamp animation
		if (!scene1) {

			int k;

			// there are 4 different joints to control
			for (k = 0; k < 4; k++) {

				init_time = temp_time;

				if (k == 0) {

					glColor3f(0, 1, 1); // color for joint 1, blue-ish
				} else if (k == 1) {

					glColor3f(1, 0, 1); // color for joint 2
				} else if (k == 2) {

					glColor3f(0, 1, 0); // color for joint 3
				} else if (k == 3) {

					glColor3f(1, .5, 0); // color for joint 4
				}

				offset = (i * 3) + (ball_control_points * (k + 2));

				// get the bezier basis for each separate bezier curve. End and start points are shared
				BezierBasis(*(p + offset), *(p + offset + 1), *(p + offset + 2), *(p + offset + 3));
		
				// for the curve, draw all the points
				for (j = 0; j < ani_time; j++) {
		
					init_time += delta_t;
		
					new_time = init_time;
		
					// ease in function on the first bounce
					if (ease) {
		
						if (init_time < 1.0) {
		
							new_time = pow(init_time, 2); // square time on the first bounce
						}
					}
					new_time = fmodf(new_time, 1.0); // set to to interval [0 1]
		
					// get point at that time, draw it
					GetPoint(&point_x, &point_y, &point_z, new_time);
					glVertex3f(point_x, point_y, point_z);
				}
				
			}
		}
	}
	glEnd();
}

/**
 * This will draw a circle of radius r at the origin
 */
void DrawCircle(float r) {

	int i;

	glPushMatrix();

		glBegin(GL_TRIANGLE_FAN); // triangle fan to make a circle

			glVertex3f(0, 0, 0); // origin
			
			// points go around the circle
			for (i = 0; i <= 32; i++) {

				glVertex3f(r * cos(i * (2.0f * M_PI) / 32), r * sin(i * (2.0f * M_PI) / 32), 0);
			}
		glEnd();
	glPopMatrix();
}

/**
 * This will draw a ball at given x and y coordinates
 */
void DrawBall(void) {

	glPushMatrix();

		// translate based on the bezier curve time parameter
		glTranslatef(ball.x, ball.y + radius, 0);
	
		glColor3f(1, 1, 0); // yellow

		DrawCircle(radius); // draw the circle
	glPopMatrix();
}

/**
 * Draw a simple 2D version of the pixar lamp
 */
void DrawLamp(void) {

	glPushMatrix();

		// draw the base of the lamp
		glTranslatef(lamp.x, lamp.y, 0);
		glRotatef(joint_1_rot, 0, 0, -1); // joint 1, lamp rotation

		glColor3f(1, 1, 0); // yellow
		
		// the base
		glBegin(GL_TRIANGLES);

			glVertex3f(-4, 0, 0);
			glVertex3f(4, 0, 0);
			glVertex3f(0, 2, 0);
		glEnd();

		glLineWidth(10); // make lines thicker

		glPushMatrix();

			// rotate by joint 2 rotation and move up
			glTranslatef(0, 2, 0);
			glRotatef(joint_2_rot, 0, 0, -1);

			DrawCircle(0.25); // draw the second joint

			// draw the lamp arm
			glBegin(GL_LINES);
			
				glVertex3f(0, 0, 0);
				glVertex3f(0, 4, 0);
			glEnd();

			glPushMatrix();
	
				// rotate joint 3 and move up
				glTranslatef(0, 4, 0);
				glRotatef(joint_3_rot, 0, 0, 1);
				
				DrawCircle(0.25); // joint 3

				glBegin(GL_LINES);

					glVertex3f(0, 0, 0);
					glVertex3f(0, 4, 0);
				glEnd();

				glPushMatrix();

					// rotate joint 4 and move up
					glTranslatef(0, 4, 0);
					glRotatef(joint_4_rot, 0, 0, -1);

					DrawCircle(0.25); // joint 4
	
					// draw the light of the lamp
					glBegin(GL_TRIANGLES);

						glVertex3f(-3, -3, 0);
						glVertex3f(3, -3, 0);
						glVertex3f(0, 0, 0);
					glEnd();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();

		glLineWidth(1); // restore line width
	glPopMatrix(); 
}

/**
 * Using ortho view, naively get the world position from screen
 * World draw from 0 to 40 horizontal, -20 to 20 vertical.
 * Use ratio on screen to get world coordinate
 */
void ScreenToWorld(int x, int y, float *world_x, float *world_y) {

	// normalize origin to bottom left
	y = WIN_HEIGHT - y;

	// convert point to range [0 1] by getting percentage. Normalize to [-5 5] for y. Multiply by window dim
	*world_x = (((float) x) / ((float) WIN_WIDTH)) * ((float) (RIGHT - LEFT));
	*world_y = ((((float) y) / ((float) WIN_HEIGHT)) - 0.5) * ((float) (TOP - BOTTOM));
}

/**
 * This will check if a control point has been selected
 */
void SelectControlPoint(int x, int y) {

	float world_x, world_y; // world coordinates

	ScreenToWorld(x, y, &world_x, &world_y); // get world coordinates

	int i, start, end;

	if (scene1) {

		start = 0;
		end = ball_control_points;
	} else {

		start = ball_control_points;
		end = control_points;
	}

	// go through all points, first fit is a match
	for (i = start; i < end; i++) {

		// select point, provide some buffer space
		if ((fabs((p + i)->x - world_x) <= 1) && (fabs((p + i)->y - world_y) <= 1)) {

			// set index and the world coordinates
			selected_control = i;
			selected_point.x = world_x;
			selected_point.y = world_y;
			selected_point.z = 0;
			return; // found one, just quit
		}
	}

	selected_control = -1; // not found
}

/**
 * Get offest and move the control point around
 */
void MoveControlPoint(int x, int y) {

	// world coord and offset
	float world_x, world_y, offset_x, offset_y;

	// get world coordinates
	ScreenToWorld(x, y, &world_x, &world_y);
	
	// offset from last position
	offset_x = (world_x - selected_point.x); 
	offset_y = (world_y - selected_point.y);

	// move the control point
	(p + selected_control)->x += offset_x;
	(p + selected_control)->y += offset_y;

	// set current position
	selected_point.x = world_x;
	selected_point.y = world_y;

	// bound the motion with the x
	if ((p + selected_control)->x < LEFT) {

		(p + selected_control)->x = LEFT;
		selected_point.x -= offset_x;
	} else if ((p + selected_control)->x > RIGHT) {

		(p + selected_control)->x = RIGHT;
		selected_point.x -= offset_x;
	}

	// bound within the y
	if ((p + selected_control)->y < BOTTOM) {

		(p + selected_control)->y = BOTTOM;
		selected_point.y -= offset_y;
	} else if ((p + selected_control)->y > TOP) {

		(p + selected_control)->y = TOP;
		selected_point.y -= offset_y;
	}
}

/**
 * Display callback. Draws the curves, points, and object
 */
void Display(void) {
	
	// clear buffers
  glClear(GL_COLOR_BUFFER_BIT);
  
	DrawGround(); // draw the ground
	
	// option to hide the controls
	if (!hide) {

		DrawControlPoints(); // draw all control points

		DrawAnimationPoints(); // draw the path for animation
	}

	// which scene
	if (scene1) {
		
		DrawBall(); // draw the ball
	} else {

		DrawLamp(); // draw the pixar lamp
	}

  glutSwapBuffers();
}

/**
 * Reshape callback to redraw the camera and keep the window dims up to date
 */
void Reshape(int width, int height) {

	// update window dimensions
	WIN_WIDTH = width;
	WIN_HEIGHT = height;

	// redraw camera
  glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
	glOrtho(LEFT, RIGHT, BOTTOM, TOP, 0, 20); // 2D ortho camera.
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

/**
 * Used to track a mouse down and selection
 */
void Mouse(int button, int state, int x, int y) {

	// left mouse up will deselect
	if (button == GLUT_LEFT_BUTTON) {

		if (state == GLUT_UP) {

			// deselect the control
			selected_control = -1;
		}
	}
}

/**
 * Used to track the mouse move after press,
 * manipulation of control point
 */
void Motion(int x, int y) {
  
	// if something selected, and not hiding
	if (selected_control >= 0 && !hide) {

		MoveControlPoint(x, y); // move a control point
	}
}

/**
 * Passive motion to detect hover over a point
 */
void PassiveMotion(int x, int y) {

	// select if controls not hidden
	if (!hide) {
	
		SelectControlPoint(x, y); // hover over a point
	}
}

/**
 * Keyboard operations
 */
void Keyboard(unsigned char key, int x, int y) {
  
	// look at the keys
	switch(key) {

		case 27: // exit the program
			FreeMem();
	  	exit(0);
	  	break;
		case 'e': // easing function on/off
			ease = !ease;
			break;
		case 's': // switch scene
			scene1 = !scene1;
			Idle(); // redraw to catch up with other animation
			break;
		case 'h': // show/hide controls
			hide = !hide;
			break;
		default:
			break;
  }
}

/**
 * Init stuff, get glut ready
 */
int main(int argc, char ** argv) {

  glutInit(&argc, argv);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
 	glutCreateWindow("My Animation");

  init();

 	glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutMouseFunc(Mouse); // for mouse down event
  glutMotionFunc(Motion); // for manipulation
	glutPassiveMotionFunc(PassiveMotion); // for selection
  glutKeyboardFunc(Keyboard);
	glutIgnoreKeyRepeat(1); // to prevent repeated input
	glutIdleFunc(Idle);

  glutMainLoop();

	FreeMem();
  return 0;
}
