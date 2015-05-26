#include "vector.h"

// vector addition
VECTOR add(VECTOR a, VECTOR b) {

	VECTOR add;

	add.x = a.x + b.x;
	add.y = a.y + b.y;
	add.z = a.z + b.z;

	return add;
}

// vector subtraction
VECTOR subtract(VECTOR a, VECTOR b) {

	VECTOR subtract;

	subtract.x = a.x - b.x;
	subtract.y = a.y - b.y;
	subtract.z = a.z - b.z;

	return subtract;
}

// scaler multiplication
VECTOR multiply(VECTOR a, float b) {

	VECTOR multiply;

	multiply.x = a.x * b;
	multiply.y = a.y * b;
	multiply.z = a.z * b;

	return multiply;
}

// scalar division
VECTOR divide(VECTOR a, float b) {

	VECTOR divide;

	divide.x = a.x / b;
	divide.y = a.y / b;
	divide.z = a.z / b;

	return divide;
}

// dot product
float dot(VECTOR a, VECTOR b) {

	VECTOR dot;

	dot.x = a.x * b.x;
	dot.y = a.y * b.y;
	dot.z = a.z * b.z;

	return dot.x + dot.y + dot.z;
}

// cross product
VECTOR cross(VECTOR a, VECTOR b) {

	VECTOR cross;

	cross.x = (a.y * b.z) - (a.z * b.y);
	cross.y = (a.z * b.x) - (a.x * b.z);
	cross.z = (a.x * b.y) - (a.y * b.x);

	return cross;
}

// vector normalization
VECTOR normalize(VECTOR a) {

	magnitude(a);

	return divide(a, magnitude(a));
}

// vector magnitude
float magnitude(VECTOR a) {

	return sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
}
