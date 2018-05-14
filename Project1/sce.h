#ifndef SCE_H
#define SCE_H

#include <vector>

#include "mesh.h"
#include "glut.h"

class obj_info
{
public:
	GLfloat trans[3];
	GLfloat angle;
	GLfloat rotate[3];
	GLfloat scale[3];
	mesh* obj;
	obj_info(char*);
	void f_reset();
};

class sce
{
public:
	int select_flg;
	std::vector<obj_info> obj_list;
	sce(char*);
	void apply();
	void reset();
	void move(int, int);
};


#endif