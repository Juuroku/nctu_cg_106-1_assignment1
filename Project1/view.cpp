#include <stdio.h>
#include <string.h>
#include <math.h>
#include "glut.h"
#include "view.h"

#define ZOOM_DISTANCE 0.1
#define ROTATE_SPEED 0.25

view::view(char* filename)
{
	FILE *fp;
	char pr[50];
	fp = fopen(filename, "r");
	if (!fp)
	{
		printf("open \"%s\" error", filename);
	}
	else
	{
		fscanf(fp, "%s", pr);
		while (!feof(fp))
		{
			if (!strcmp(pr, "eye"))
			{
				fscanf(fp, "%lf", &camera_position.x);
				fscanf(fp, "%lf", &camera_position.y);
				fscanf(fp, "%lf", &camera_position.z);
				printf("eye %lf %lf %lf\n", camera_position.x, camera_position.y, camera_position.z);
			}
			else if (!strcmp(pr, "vat"))
			{
				fscanf(fp, "%lf", &look_direction.x);
				fscanf(fp, "%lf", &look_direction.y);
				fscanf(fp, "%lf", &look_direction.z);
				printf("vat %lf %lf %lf\n", look_direction.x, look_direction.y, look_direction.z);
			}
			else if (!strcmp(pr, "vup"))
			{
				fscanf(fp, "%lf", &upvector.x);
				fscanf(fp, "%lf", &upvector.y);
				fscanf(fp, "%lf", &upvector.z);
				printf("vup %lf %lf %lf\n", upvector.x, upvector.y, upvector.z);
			}
			else if (!strcmp(pr, "fovy"))
			{
				fscanf(fp, "%lf", &fovy);
			}
			else if (!strcmp(pr, "dnear"))
			{
				fscanf(fp, "%lf", &dnear);
			}
			else if (!strcmp(pr, "dfar"))
			{
				fscanf(fp, "%lf", &dfar);
			}
			else if (!strcmp(pr, "viewport"))
			{
				fscanf(fp, "%d", &viewport.x);
				fscanf(fp, "%d", &viewport.y);
				fscanf(fp, "%d", &viewport.w);
				fscanf(fp, "%d", &viewport.h);
			}
			fscanf(fp, "%s", pr);
		}
	}
	fclose(fp);
	if ((camera_position.x - look_direction.x)*upvector.x + (camera_position.y - look_direction.y)*upvector.y + (camera_position.z - look_direction.z)*upvector.z)
	{
		printf("change upvector\n");
		GLdouble cross[3];
		cross[2] = (camera_position.x - look_direction.x) * upvector.y - (camera_position.y - look_direction.y) * upvector.x;
		cross[0] = (camera_position.y - look_direction.y) * upvector.z - (camera_position.z - look_direction.z) * upvector.y;
		cross[1] = (camera_position.z - look_direction.z) * upvector.x - (camera_position.x - look_direction.x) * upvector.z;
		printf("%lf %lf %lf\n", cross[0], cross[1], cross[2]);
		//GLdouble cross[3];
		orthovector.z = -(camera_position.x - look_direction.x) * cross[1] + (camera_position.y - look_direction.y) * cross[0];
		orthovector.x = -(camera_position.y - look_direction.y) * cross[2] + (camera_position.z - look_direction.z) * cross[1];
		orthovector.y = -(camera_position.z - look_direction.z) * cross[0] + (camera_position.x - look_direction.x) * cross[2];
	}
	else
	{
		orthovector.z = upvector.z;
		orthovector.x = upvector.x;
		orthovector.y = upvector.y;
	}
	GLdouble abs;
	abs = sqrt(orthovector.x*orthovector.x + orthovector.y*orthovector.y + orthovector.z*orthovector.z);
	orthovector.x = orthovector.x / abs;
	orthovector.y = orthovector.y / abs;
	orthovector.z = orthovector.z / abs;
	printf("%lf %lf %lf\n", orthovector.x, orthovector.y, orthovector.z);
}

void view::apply()
{
	
	glViewport(viewport.x, viewport.y, viewport.w, viewport.h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(fovy, (GLfloat)viewport.w / (GLfloat)viewport.h, dnear, dfar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camera_position.x, camera_position.y, camera_position.z, look_direction.x, look_direction.y, look_direction.z, upvector.x, upvector.y, upvector.z);
}

void view::zoom_in()
{
	GLdouble x = camera_position.x;
	GLdouble y = camera_position.y;
	GLdouble z = camera_position.z;
	double abs = x*x + y*y + z*z;
	// = sqrt(abs);
	camera_position.x = camera_position.x + ZOOM_DISTANCE * (look_direction.x - camera_position.x); // abs;
	camera_position.y = camera_position.y + ZOOM_DISTANCE * (look_direction.y - camera_position.y);// abs;
	camera_position.z = camera_position.z + ZOOM_DISTANCE * (look_direction.z - camera_position.z);// abs;
	
	apply();
}

void view::zoom_out()
{
	GLdouble x = look_direction.x - camera_position.x;
	GLdouble y = look_direction.y - camera_position.y;
	GLdouble z = look_direction.z - camera_position.z;
	double abs = x*x + y*y + z*z;
	abs = sqrt(abs);
	camera_position.x = camera_position.x - ZOOM_DISTANCE * (look_direction.x - camera_position.x); // abs;
	camera_position.y = camera_position.y - ZOOM_DISTANCE * (look_direction.y - camera_position.y); // abs;
	camera_position.z = camera_position.z - ZOOM_DISTANCE * (look_direction.z - camera_position.z); // abs;
}

void view::camera_rl()
{
	GLdouble cross[3];
	cross[2] = (camera_position.x - look_direction.x) * orthovector.y - (camera_position.y - look_direction.y) * orthovector.x;
	cross[0] = (camera_position.y - look_direction.y) * orthovector.z - (camera_position.z - look_direction.z) * orthovector.y;
	cross[1] = (camera_position.z - look_direction.z) * orthovector.x - (camera_position.x - look_direction.x) * orthovector.z;
	printf("%lf %lf %lf\n", cross[0],cross[1],cross[2]);
	GLdouble plus[3];
	plus[0] = camera_position.x + cross[0] * ROTATE_SPEED - look_direction.x;
	plus[1] = camera_position.y + cross[1] * ROTATE_SPEED - look_direction.y;
	plus[2] = camera_position.z + cross[2] * ROTATE_SPEED - look_direction.z;
	printf("%lf %lf %lf\n", plus[0], plus[1], plus[2]);
	GLdouble abs1, abs2;
	abs1 =sqrt( plus[0] * plus[0] + plus[1] * plus[1] + plus[2] * plus[2]);
	abs2 = sqrt((camera_position.x - look_direction.x)* (camera_position.x - look_direction.x) + (camera_position.y - look_direction.y)*(camera_position.y - look_direction.y) + (camera_position.z - look_direction.z)*(camera_position.z - look_direction.z));
	camera_position.x = plus[0] * (abs2 / abs1) + look_direction.x;
	camera_position.y = plus[1] * (abs2 / abs1) + look_direction.y;
	camera_position.z = plus[2] * (abs2 / abs1) + look_direction.z;
	printf("%lf %lf %lf\n", camera_position.x, camera_position.y, camera_position.z);
}

void view::camera_rr()
{
	GLdouble cross[3];
	cross[2] = (camera_position.x - look_direction.x) * orthovector.y - (camera_position.y - look_direction.y) * orthovector.x;
	cross[0] = (camera_position.y - look_direction.y) * orthovector.z - (camera_position.z - look_direction.z) * orthovector.y;
	cross[1] = (camera_position.z - look_direction.z) * orthovector.x - (camera_position.x - look_direction.x) * orthovector.z;
	printf("%lf %lf %lf\n", cross[0], cross[1], cross[2]);
	GLdouble plus[3];
	plus[0] = camera_position.x - cross[0] * ROTATE_SPEED - look_direction.x;
	plus[1] = camera_position.y - cross[1] * ROTATE_SPEED - look_direction.y;
	plus[2] = camera_position.z - cross[2] * ROTATE_SPEED - look_direction.z;
	printf("%lf %lf %lf\n", plus[0], plus[1], plus[2]);
	GLdouble abs1, abs2;
	abs1 = sqrt(plus[0] * plus[0] + plus[1] * plus[1] + plus[2] * plus[2]);
	abs2 = sqrt((camera_position.x - look_direction.x)* (camera_position.x - look_direction.x) + (camera_position.y - look_direction.y)*(camera_position.y - look_direction.y) + (camera_position.z - look_direction.z)*(camera_position.z - look_direction.z));
	camera_position.x = plus[0] * (abs2 / abs1) + look_direction.x;
	camera_position.y = plus[1] * (abs2 / abs1) + look_direction.y;
	camera_position.z = plus[2] * (abs2 / abs1) + look_direction.z;
	printf("%lf %lf %lf\n", camera_position.x, camera_position.y, camera_position.z);
}