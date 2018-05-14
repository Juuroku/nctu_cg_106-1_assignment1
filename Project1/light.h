#ifndef LIGHT_H
#define LIGHT_H

#include <vector>
#include <string.h>
#include "vec.h"
#include "glut.h"



class lighting
{
public:
	GLfloat position[4];
	GLfloat ia[4];
	GLfloat id[4];
	GLfloat is[4];

	lighting(GLfloat*, GLfloat*, GLfloat*, GLfloat*);
};

class ambient
{
public:
	GLfloat data[4];
	
	ambient();
};

class light_info
{
public:
	std::vector<lighting> light_list;
	ambient amb;
	light_info();
	void add_light(char*);
};
#endif