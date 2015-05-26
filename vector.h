#ifndef __VECTOR_H
#define __VECTOR_H

#include <math.h>

// vector, xyz
typedef struct vector {

	float x;
	float y;
	float z;
} VECTOR;

// add two vectors
VECTOR add(VECTOR a, VECTOR b);

// subtract two vectors
VECTOR subtract(VECTOR a, VECTOR b);

// scalar multiplication
VECTOR multiply(VECTOR a, float b);

// scalar division
VECTOR divide(VECTOR a, float b);

// dot product
float dot(VECTOR a, VECTOR b);

// cross product
VECTOR cross(VECTOR a, VECTOR b);

// normalize vector
VECTOR normalize(VECTOR a);

// vector magnitude
float magnitude(VECTOR a);

#endif
