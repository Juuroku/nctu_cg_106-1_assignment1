#ifndef VIEW_H
#define VIEW_H

#include "vec.h"

class view
{
public:
	v3 camera_position;
	v3 look_direction;
	v3 upvector;
	v3 orthovector;
	GLdouble fovy;
	GLdouble dnear;
	GLdouble dfar;
	vp viewport;

	view(char*);
	void apply();
	void zoom_in();
	void zoom_out();
	void camera_rl();
	void camera_rr();
};


#endif