#ifndef __RAYTRACER_H
#define __RAYTRACER_H

#include "vector.h"

// color struct, RGB
typedef struct color {

	float r;
	float g;
	float b;
} COLOR;

// define different shader model enums
enum shader_model {
	
	LAMBERT_MODEL,
	PHONG_MODEL
};

// define different geometry types
enum geometry_type {

	SPHERE_GEO,
	PLANE_GEO
};

// define different light types
enum light_type {
	
	AREA_LIGHT,
	POINT_LIGHT
};

// define a shader
typedef struct shader {

	shader_model model; // what model
	COLOR diffuse; // diffuse color
	COLOR specular; // specular color (PHONG only)
	float power; // shininess (PHONG only)
} SHADER;

// define a triangle
typedef struct triangle {

	VECTOR a; // corners
	VECTOR b;
	VECTOR c;
	VECTOR normal; // normal direction
} TRIANGLE;

// define geometry
typedef struct geometry {

	geometry_type geo; // what type
	SHADER shader; // what shader does it use
	VECTOR pos; // (sphere) position
	float rad; // (sphere) radius
	float tri_count; // how many triangles
	TRIANGLE *triangles; // list of triangles
} GEOMETRY;

// define light
typedef struct light {

	light_type type; // type
	VECTOR pos; // position
	COLOR intensity; // color
} LIGHT;

// orthogonal view space
typedef struct view {
 
 	VECTOR normal; // view normal
	float dist; // camera to plane
	float width;
	float height;
	float left;
	float right;
	float bottom;
	float top;
	float near;
	float far;
} VIEW;

// define camera
typedef struct camera {

	VECTOR pos; // position of camera
	VECTOR forward; // forward direction
	VECTOR up; // up direction
	VIEW v; // a camera view space
} CAMERA;

#endif
