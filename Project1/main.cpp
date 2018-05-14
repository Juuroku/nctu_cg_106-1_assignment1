//CG assignment1 by Juuroku
#include "mesh.h"
#include "glut.h"
#include "light.h"
#include "view.h"
#include "sce.h"


mesh *object;//1, *object2, *object3;

int windowSize[2];

void light();
void display();
void reshape(GLsizei , GLsizei );
void keyboard(unsigned char, int, int);
void mouse(int, int, int, int);
void drag(int, int);

view v("scene2.view");
light_info li;
sce sc("scene2.scene");

int mouse_flg;
int mouse_x, mouse_y;

int main(int argc, char** argv)
{
	//object = new mesh("box.obj");
	//object2 = new mesh("bunny.obj");
	//object3 = new mesh("venus.obj");
	mouse_flg = 0;

	li.add_light("scene2.light");
	printf("load light\n");

	glutInit(&argc, argv);
	glutInitWindowSize(v.viewport.w, v.viewport.h);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("CG Project 1-1");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}

void light()
{
	/*GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat light_position[] = {150.0, 150.0, 150.0, 1.0};*/

	glShadeModel(GL_SMOOTH);

	// z buffer enable
	glEnable(GL_DEPTH_TEST);

	// enable lighting
	glEnable(GL_LIGHTING);
	// set light property
	int n = li.light_list.size();
	//printf("n = %d\n", n);
	for (int i = 0; i < n && i < 8; i++)
	{
		//printf("%d\n", i);
		glEnable(GL_LIGHT0 + i);
		glLightfv(GL_LIGHT0 + i, GL_POSITION, li.light_list[i].position);
		/*for (int j = 0; j < 4; j++)
		{
			printf("%f ", li.light_list[i].position[j]);
		}
		printf("\n");*/
		glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, li.light_list[i].id);
		/*for (int j = 0; j < 4; j++)
		{
			printf("%f ", li.light_list[i].id[j]);
		}
		printf("\n");*/
		glLightfv(GL_LIGHT0 + i, GL_SPECULAR, li.light_list[i].is);
		/*for (int j = 0; j < 4; j++)
		{
			printf("%f ", li.light_list[i].is[j]);
		}
		printf("\n");*/
		glLightfv(GL_LIGHT0 + i, GL_AMBIENT, li.light_list[i].ia);
		/*for (int j = 0; j < 4; j++)
		{
			printf("%f ", li.light_list[i].ia[j]);
		}
		printf("\n");*/
	}
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, li.amb.data);
	
}

void display()
{
	// clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);      //清除用color
	glClearDepth(1.0f);                        // Depth Buffer (就是z buffer) Setup
	glEnable(GL_DEPTH_TEST);                   // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                    // The Type Of Depth Test To Do
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//這行把畫面清成黑色並且清除z buffer

	// viewport transformation
	//glViewport(0, 0, windowSize[0], windowSize[1]);

	// projection transformation
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(60.0, (GLfloat)windowSize[0]/(GLfloat)windowSize[1], 1.0, 1000.0);
	// viewing and modeling transformation
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	/*gluLookAt(	200.0, 300.0, 150.0,// eye
				0.0, 0.0, 0.0,     // center
				0.0, 1.0, 0.0);    // up*/

	v.apply();

	//注意light位置的設定，要在gluLookAt之後
	light();

	/*int lastMaterial = -1;
	for(size_t i=0;i < object->fTotal;++i)
	{
		// set material property if this face used different material
		if(lastMaterial != object->faceList[i].m)
		{
			lastMaterial = (int)object->faceList[i].m;
			glMaterialfv(GL_FRONT, GL_AMBIENT  , object->mList[lastMaterial].Ka);
			glMaterialfv(GL_FRONT, GL_DIFFUSE  , object->mList[lastMaterial].Kd);
			glMaterialfv(GL_FRONT, GL_SPECULAR , object->mList[lastMaterial].Ks);
			glMaterialfv(GL_FRONT, GL_SHININESS, &object->mList[lastMaterial].Ns);

			//you can obtain the texture name by object->mList[lastMaterial].map_Kd
			//load them once in the main function before mainloop
			//bind them in display function here
		}
		//scene
		glPushMatrix();

		glTranslatef(-200.0, 0.0, 0.0);

		glBegin(GL_TRIANGLES);
		for (size_t j=0;j<3;++j)
		{
			//textex corrd. object->tList[object->faceList[i][j].t].ptr
			glNormal3fv(object->nList[object->faceList[i][j].n].ptr);
			glVertex3fv(object->vList[object->faceList[i][j].v].ptr);	
		}
		glEnd();

		glPopMatrix();
	}*/
	sc.apply();

	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
	windowSize[0] = w;
	windowSize[1] = h;
}

void keyboard(unsigned char key, int x, int y)
{
	float rs = 0.1f;
	printf("%c\n", key);
	switch (key)
	{
	case 'w':
		//zoom in
		v.zoom_in();
		glutPostRedisplay();
		break;
	case 'a':
		//move left
		v.camera_rl();
		glutPostRedisplay();
		break;
	case 's':
		//zoom out
		v.zoom_out();
		glutPostRedisplay();
		break;
	case 'd':
		v.camera_rr();
		glutPostRedisplay();
		//move right
		break;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		//select object
		sc.select_flg = key - '1';
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		printf("%d %d\n", x, y);
		mouse_x = x;
		mouse_y = y;
		mouse_flg = 1;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (mouse_flg)
		{
			mouse_flg = 0;
		}
	}
}

void drag(int x, int y)
{
	if (mouse_flg)
	{
		printf("%d %d\n", x, y);
		sc.move(x - mouse_x, mouse_y - y);
		mouse_x = x;
		mouse_y = y;
		glutPostRedisplay();
	}
}