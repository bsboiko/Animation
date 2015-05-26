/**
 *	@author:			Boris Boiko
 *	@id:					sd45774
 *	@email:				b.sboiko@umbc.edu
 *	@description:	Raytracer for ambient light and point lights.
 * 								Using sphere and plane (triangle) geometryies.
 *								Phong shading and Lambert shading.
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vector.h"
#include "Raytracer.h"

/**
 * some global vars and colors
 */
unsigned int long screen_x;
unsigned int long screen_y;

int geo_count;
int light_count;

COLOR **image; // 2D color array
GEOMETRY *geometry; // geometry on the screen
CAMERA camera; // define 1 camera only
LIGHT *lights; // define the light

COLOR background_color = {0, 0, 0}; // black background
COLOR white_color = {1, 1, 1}; // white
COLOR gray_color = {.4, .4, .4}; // gray
COLOR blue_color = {.2, .3, .8}; // blue-ish color
COLOR red_color = {.8, .2, .2}; // red-ish color
COLOR green_color = {.2, .8, .2}; // green-ish color
COLOR specular_color = {1, 1, 0}; // specular color

/**
 * Init camera params
 */
void initCamera(void) {

	// position
	camera.pos.x = 5;
	camera.pos.y = 4;
	camera.pos.z = 3;

	// forward direction
	camera.forward.x = -5;
	camera.forward.y = -4;
	camera.forward.z = -3;

	// up direction
	camera.up.x = 0;
	camera.up.y = 1;
	camera.up.z = 0;

	// define view space
	camera.v.normal.x = 5;
	camera.v.normal.y = 4;
	camera.v.normal.z = 3;
	camera.v.dist = 5;
	camera.v.width = 2.5;
	camera.v.height = 2.5;
	camera.v.left = -camera.v.width / 2.0;
	camera.v.right = camera.v.width / 2.0;
	camera.v.bottom = -camera.v.height / 2.0;
	camera.v.near = 0;
	camera.v.far = 40;
	camera.v.top = camera.v.height / 2.0;
}

/**
 * Initialize all the lights
 */
void initLight(void) {

	// make n lights
	light_count = 4;
	lights = (LIGHT *)malloc(light_count * sizeof(LIGHT));

	// define area light
	(lights + 0)->type = AREA_LIGHT;
	(lights + 0)->intensity.r = 0.1;
	(lights + 0)->intensity.g = 0.1;
	(lights + 0)->intensity.b = 0.1;
	
	// white point light
	(lights + 1)->type = POINT_LIGHT;
	(lights + 1)->pos.x = 3;
	(lights + 1)->pos.y = 4;
	(lights + 1)->pos.z = 5;
	(lights + 1)->intensity.r = 1;
	(lights + 1)->intensity.g = 1;
	(lights + 1)->intensity.b = 1;
	
	// red point light up top
	(lights + 2)->type = POINT_LIGHT;
	(lights + 2)->pos.x = 0;
	(lights + 2)->pos.y = 10;
	(lights + 2)->pos.z = 0;
	(lights + 2)->intensity.r = 1;
	(lights + 2)->intensity.g = 0;
	(lights + 2)->intensity.b = 0;
	
	// green point light to right
	(lights + 3)->type = POINT_LIGHT;
	(lights + 3)->pos.x = 0;
	(lights + 3)->pos.y = 0;
	(lights + 3)->pos.z = -10;
	(lights + 3)->intensity.r = 0;
	(lights + 3)->intensity.g = 1;
	(lights + 3)->intensity.b = 0;
}

/**
 * Init the geometry
 */
void initGeometry(void) {

	// allocate geometry space
	geo_count = 4;
	geometry = (GEOMETRY *)malloc(geo_count * sizeof(GEOMETRY));

	// blue sphere with phong shading at (0 0 1)
	(geometry + 0)->geo = SPHERE_GEO;
	(geometry + 0)->shader.model = PHONG_MODEL;
	(geometry + 0)->shader.diffuse = blue_color;
	(geometry + 0)->shader.specular = specular_color;
	(geometry + 0)->shader.power = 50;
	(geometry + 0)->pos.x = 0;
	(geometry + 0)->pos.y = 0;
	(geometry + 0)->pos.z = 1;
	(geometry + 0)->rad = 0.5;
	
	// red sphere with phong shading at (0 0 -1);
	(geometry + 1)->geo = SPHERE_GEO;
	(geometry + 1)->shader.model = PHONG_MODEL;
	(geometry + 1)->shader.diffuse = red_color;
	(geometry + 1)->shader.specular = white_color;
	(geometry + 1)->shader.power = 50;
	(geometry + 1)->pos.x = 0;
	(geometry + 1)->pos.y = 0;
	(geometry + 1)->pos.z = -1;
	(geometry + 1)->rad = 0.5;
	
	// green sphere with phong shading at (-1 -0.5 0)
	(geometry + 2)->geo = SPHERE_GEO;
	(geometry + 2)->shader.model = PHONG_MODEL;
	(geometry + 2)->shader.diffuse = green_color;
	(geometry + 2)->shader.specular = white_color;
	(geometry + 2)->shader.power = 50;
	(geometry + 2)->pos.x = -1;
	(geometry + 2)->pos.y = -0.5;
	(geometry + 2)->pos.z = 0;
	(geometry + 2)->rad = 0.5;
	
	// gray plane below the spheres, corners from (-2.5, -1, 2.5) to (2.5, -1, 2.5). Lambert shading
	(geometry + 3)->geo = PLANE_GEO;
	(geometry + 3)->shader.model = LAMBERT_MODEL;
	(geometry + 3)->shader.diffuse = gray_color;
	(geometry + 3)->tri_count = 2;
	(geometry + 3)->triangles = (TRIANGLE *)malloc((geometry + 3)->tri_count * sizeof(TRIANGLE));
	((geometry + 3)->triangles + 0)->a.x = -2.5;
	((geometry + 3)->triangles + 0)->a.y = -1;
	((geometry + 3)->triangles + 0)->a.z = 2.5;
	((geometry + 3)->triangles + 0)->b.x = 2.5;
	((geometry + 3)->triangles + 0)->b.y = -1;
	((geometry + 3)->triangles + 0)->b.z = -2.5;
	((geometry + 3)->triangles + 0)->c.x = -2.5;
	((geometry + 3)->triangles + 0)->c.y = -1;
	((geometry + 3)->triangles + 0)->c.z = -2.5;
	((geometry + 3)->triangles + 0)->normal.x = 0;
	((geometry + 3)->triangles + 0)->normal.y = 1;
	((geometry + 3)->triangles + 0)->normal.z = 0;
	((geometry + 3)->triangles + 1)->a = ((geometry + 3)->triangles + 0)->b;
	((geometry + 3)->triangles + 1)->b = ((geometry + 3)->triangles + 0)->a;
	((geometry + 3)->triangles + 1)->c.x = 2.5;
	((geometry + 3)->triangles + 1)->c.y = -1;
	((geometry + 3)->triangles + 1)->c.z = 2.5;
	((geometry + 3)->triangles + 1)->normal.x = 0;
	((geometry + 3)->triangles + 1)->normal.y = 1;
	((geometry + 3)->triangles + 1)->normal.z = 0;
}

/**
 * Init the 2D image array
 */
void initImage(void) {

	// init screen y space
	image = (COLOR **)malloc(screen_y * sizeof(COLOR));

	int i, j;

	for (i = 0; i < screen_y; i++) {

		// make all screen x space
		*(image + i) = (COLOR *)malloc(screen_x * sizeof(COLOR));
	
		// init everything to background color initially
		for (j = 0; j < screen_x; j++) {
	
			(*(image + i) + j)->r = background_color.r;
			(*(image + i) + j)->g = background_color.g;
			(*(image + i) + j)->b = background_color.b;
		}
	}
}

/**
 * Init everything
 */
void init(void) {

	// set up screen space
	screen_x = 1024;
	screen_y = 1024;
	
	initCamera(); // init camera

	initLight(); // init lights

	initGeometry(); // init geo

	initImage(); // set up image
}

/**
 * Given (i j) coordinates, calculate the uv coordinates
 */
void calculateUV(int i, int j, float *u, float *v) {

	// equations from the book and class.
	*u = camera.v.left + ((camera.v.right - camera.v.left) * (i + 0.5)) / screen_x;
	*v = camera.v.bottom + ((camera.v.top - camera.v.bottom) * (j + 0.5)) / screen_y;
}

/**
 * check for a sphere intersection given origin and direction.
 * equation came from the book
 */
float calculateSphereIntersection(VECTOR o, VECTOR d, int id) {

	float a = dot(d, d); // dot of d and d
	float b = dot(d, subtract(o, (geometry + id)->pos)); // b for the equation
	VECTOR c = subtract(o, (geometry + id)->pos); // c for equation
	float discriminant = pow(b, 2) - a * (dot(c, c) - pow((geometry + id)->rad, 2));

	// check the discriminant
	if (discriminant < 0) {

		return -1;
	}

	// calculate two answers from quadratic equation
	float t = (-b + sqrt(discriminant)) / a;
	float t_ = (-b - sqrt(discriminant)) / a;

	return fmin(t, t_); // get min t
}

/**
 * Compute barycentric and check for triangle intersection
 */
float calculateTriangleIntersection(VECTOR o, VECTOR d, VECTOR a, VECTOR b, VECTOR c) {

	// calculate matrix A. From the book
	float A_a = a.x - b.x;
	float A_b = a.y - b.y;
	float A_c = a.z - b.z;
	float A_d = a.x - c.x;
	float A_e = a.y - c.y;
	float A_f = a.z - c.z;
	float A_g = d.x;
	float A_h = d.y;
	float A_i = d.z;

	// jkl, the uvs
	float j = a.x - o.x;
	float k = a.y - o.y;
	float l = a.z - o.z;

	// determinant of matrix A
	float A_det = A_a * (A_e * A_i - A_h * A_f) + A_b * (A_g * A_f - A_d * A_i) + A_c * (A_d * A_h - A_e * A_g);

	// calculate time from the book
	float T = -((A_f * (A_a * k - j * A_b) + A_e * (j * A_c - A_a * l) + A_d * (A_b * l -  k * A_c)) / A_det);

	// if time is out of bounds using view space
	if (T < camera.v.near || T > camera.v.far) {

		return -1; // not found
	}

	// calculate gamma from the book
	float G = (A_i * (A_a * k - j * A_b) + A_h * (j * A_c - A_a * l) + A_g * (A_b * l -  k * A_c)) / A_det;

	// gamma within (0, 1)
	if (G < 0 || G > 1) {

		return -1; // otherwise not found
	}

	// calculate beta from book
	float B = (j * (A_e * A_i - A_h * A_f) + k * (A_g * A_f - A_d * A_i) + l * (A_d * A_h -  A_e * A_g)) / A_det;

	// check beta bounc
	if (B < 0 || B > (1 - G)) {

		return -1;
	}

	return T; // found a point, return t
}

/**
 * Calculate plane intersection
 */
float calculatePlaneIntersection(VECTOR o, VECTOR d, int id, int *tri_id) {

	int i;

	float t;

	// go through all the triangles on the plane
	for (i = 0; i < (geometry + id)->tri_count; i++) {

		// look for intersection, store the triangle id by pointer reference
		t = calculateTriangleIntersection(o, d, ((geometry + id)->triangles + i)->a, ((geometry + id)->triangles + i)->b, ((geometry + id)->triangles + i)->c);
		*tri_id = i;

		if (t >= 0) {

			// found it
			return t;
		}
	}

	return -1; // found nothing
}

/**
 * Calculate intersection
 */
float calculateIntersection(VECTOR o, VECTOR d, int id, int *tri_id) {

	// check for sphere
	if ((geometry + id)->geo == SPHERE_GEO) {

		return calculateSphereIntersection(o, d, id);
	} else if ((geometry + id)->geo == PLANE_GEO) { // check for plane
	
		return calculatePlaneIntersection(o, d, id, tri_id);
	} else {

		return -1; // nothing
	}
}

/**
 * For shadows, check if a geometry is blocking a light source
 */
bool lightBlocked(VECTOR src, VECTOR dir) {

	int i, tri_id;

	float t;

	src = add(src, multiply(dir, 0.00001));

	// go through all the geo
	for (i = 0; i < geo_count; i++) {

		// if intersection is found, we need shadow
		if (calculateIntersection(src, dir, i, &tri_id) >= 0) {

			return true;
		}
	}

	return false; // light is not blocked
}

/**
 * Compute the shader
 */
COLOR calculateShader(VECTOR point, int geo_id, int tri_id) {

	COLOR L = {0, 0, 0}; // start color at nothing

	// point to the camera
	VECTOR v = normalize(subtract(camera.pos, point));

	VECTOR normal, l, h;

	// compute normal of the point
	if ((geometry + geo_id)->geo == SPHERE_GEO) {
	
		// sphere is center to point, normalize with radius
		normal = divide(subtract(point, (geometry + geo_id)->pos), (geometry + geo_id)->rad);
	} else if ((geometry + geo_id)->geo == PLANE_GEO) {

		// plane, just get the normal of the triangle
		normal = ((geometry + geo_id)->triangles + tri_id)->normal;
	}

	int i;

	// go through every light
	for (i = 0; i < light_count; i++) {

		// area light just gets added
		if ((lights + i)->type == AREA_LIGHT) {

			// add the light color to the diffuse color
			L.r += (geometry + geo_id)->shader.diffuse.r * (lights + i)->intensity.r;
			L.g += (geometry + geo_id)->shader.diffuse.g * (lights + i)->intensity.g;
			L.b += (geometry + geo_id)->shader.diffuse.b * (lights + i)->intensity.b;

		} else if ((lights + i)->type == POINT_LIGHT) { // point light
	
			l = normalize(subtract((lights + i)->pos, point)); // direction from point to the light, normalized
	
			// check if light is blocked
			if (!lightBlocked(point, l)) {
				
				// lambert and phong get diffuse color
				if ((geometry + geo_id)->shader.model == LAMBERT_MODEL || (geometry + geo_id)->shader.model == PHONG_MODEL) {
		
					float d = fmax(0, dot(normal, l)); // dot of normal and light dir
			
					// add the color
					L.r += (geometry + geo_id)->shader.diffuse.r * (lights + i)->intensity.r * d;
					L.g += (geometry + geo_id)->shader.diffuse.g * (lights + i)->intensity.g * d;
					L.b += (geometry + geo_id)->shader.diffuse.b * (lights + i)->intensity.b * d;
			
				}
	
				// phong gets specular shading
				if ((geometry + geo_id)->shader.model == PHONG_MODEL) {
			
					h = normalize(add(v, l)); // bisect of light and view dirs
			
					float s = pow(fmax(0, dot(normal, h)), (geometry + geo_id)->shader.power);
			
					L.r += (geometry + geo_id)->shader.specular.r * (lights + i)->intensity.r * s;
					L.g += (geometry + geo_id)->shader.specular.g * (lights + i)->intensity.g * s;
					L.b += (geometry + geo_id)->shader.specular.b * (lights + i)->intensity.b * s;
				}
			}
		}
	}

	// bound color up to max of 1
	L.r = fmin(1, L.r);
	L.g = fmin(1, L.g);
	L.b = fmin(1, L.b);

	return L;
}

/**
 * Calculate the image pixel colors
 */
void calculateColor(void) {

	printf("\nRendering Image.\n");

	// track progress of rendering
	int current_percent = 0;
	float new_percent = 0.0;

	int i, j, k, geo_id, tri_id, temp_id;

	float u, v, t, t_min;

	VECTOR s; // source
	VECTOR d = normalize(camera.forward); // normalize the viewing direction
	VECTOR n = normalize(camera.v.normal); // normal for view plane

	// u, v, and n are all orthogonal to each other.
	VECTOR u_ = normalize(cross(camera.up, n)); //camera up cross n, normalized
	VECTOR v_ = cross(n, u_); // get v
	VECTOR temp;

	COLOR c; // color at a pixel

	// go through screen vertically
	for (i = 0; i < screen_y; i++) {

		// go through x points
		for (j = 0; j < screen_x; j++) {

			calculateUV(j, i, &u, &v); // calculate uv coordinates of (x,y)
			
			// calculate the source point
			temp = add(multiply(u_, u), multiply(v_, v));
			s = add(camera.pos, temp);

			geo_id = 0;
			t = calculateIntersection(s, d, geo_id, &tri_id);

			// look for an intersection
			for (k = 0; k < geo_count; k++) {

				t_min = calculateIntersection(s, d, k, &temp_id);

				// check for smallest point
				if ((t_min >= 0 && t_min < t) || (t < 0 && t_min >=0)) {

					t = t_min; // time
					geo_id = k; // geometry
					tri_id = temp_id; // triangle
				}
			}
	
			// if t is valid, just as a precaution
			if (t >= 0) {
		
				// calculate color from the shader at a point on temp
				temp = add(s, multiply(d, t));
				c = calculateShader(temp, geo_id, tri_id);
	
				// set that pixel color to shaders color
				(*(image + i) + j)->r = c.r;
				(*(image + i) + j)->g = c.g;
				(*(image + i) + j)->b = c.b;
			}
	
			// progress tracking
			new_percent = 100 * ((float)((i * screen_x) + j) / (float)(screen_y * screen_x));
	
			if (new_percent >= current_percent) {
	
				current_percent = (int)new_percent + 1;
				printf("%d%%\n\033[F", current_percent);
			}
		}
	}

	printf("\n\nDone Rendering.\n");
}

/**
 * Create a P3 magic number PPM file
 */
void writePPM(void) {

	printf("\nWriting to PPM file.\n");

	// track progress
	int current_percent = 0;
	float new_percent = 0.0;

	int i, j;

	// open a ppm file
	FILE *fp = fopen("sphere.ppm", "wb");

	if (fp == NULL) {
		
		fprintf(stderr, "Could not open file. Exiting.");
		exit(-1);
	}

	// write P3 with dim of screen
	fprintf(fp, "P3\n%lu %lu\n255\n", screen_x, screen_y);

	for (i = screen_y - 1; i >= 0; i--) {

		for (j = 0; j < screen_x; j++) {

			// write color
			fprintf(fp, "%u %u %u  ", (int)(255 * (*(image + i) + j)->r), (int)(255 * (*(image + i) + j)->g), (int)(255 * (*(image + i) + j)->b));

			// track progress
			new_percent = 100 * ((float)(((screen_y - i) * screen_x) + j) / (float)(screen_y * screen_x));

			if (new_percent >= current_percent) {

				current_percent = fmin(100, (int)new_percent + 1);
				printf("%d%%\n\033[F", current_percent);
			}
		}

		fprintf(fp, "\n");
	}

	fclose(fp);

	printf("\n\nDone Writing.\n");
}

/**
 * Free data
 */
void freeData(void) {

	int i;

	for (i = 0; i < screen_y; i++) {

		free(*(image + i));
	}

	free(image);

	free(lights);

	// free the triangles
	for (i = 0; i < geo_count; i++) {

		if ((geometry + i)->geo == PLANE_GEO) {

			free((geometry + i)->triangles);
		}
	}
	free(geometry);
}

/**
 * Main routine
 */
int main(int argc, char ** argv) {

	init(); // init data

	calculateColor(); // get color for each pixel

	writePPM(); // write to file

	freeData(); // free data

  return 0;
}
