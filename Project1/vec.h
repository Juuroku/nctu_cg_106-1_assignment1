#ifndef VEC_H
#define VEC_H

#include "glut.h"

class v3
{
public:
	GLdouble x;
	GLdouble y;
	GLdouble z;
};

class v4
{
public:
	GLdouble x;
	GLdouble y;
	GLdouble z;
	GLdouble w;
};

class vp
{
public:
	GLint x;
	GLint y;
	GLsizei w;
	GLsizei h;
};

#endif