#include "sce.h"
#include <string.h>


obj_info::obj_info(char* objname)
{
	obj = new mesh(objname);
}

void obj_info::f_reset()
{
	angle = 0.0;
	for (int i = 0; i < 3; i++)
	{
		trans[i] = 0.0;
		rotate[i] = 0.0;
		scale[i] = 0.0;
	}
}

sce::sce(char* filename)
{
	obj_list.clear();
	select_flg = -1;
	printf("loading %s\n", filename);
	FILE* fp;
	fp = fopen(filename, "r");
	if (fp == 0) printf("error open file %s\n", filename);
	else
	{
		char input[1001];
		fscanf(fp, "%s", input);
		while (!feof(fp))
		{
			
			if (!strcmp(input, "model"))
			{
				char objname[1001];
				fscanf(fp, "%s", objname);
				//printf("%s\n", objname);
				obj_info obji(objname);

				fscanf(fp, "%f", &obji.scale[0]);
				fscanf(fp, "%f", &obji.scale[1]);
				fscanf(fp, "%f", &obji.scale[2]);
				fscanf(fp, "%f", &obji.angle);
				fscanf(fp, "%f", &obji.rotate[0]);
				fscanf(fp, "%f", &obji.rotate[1]);
				fscanf(fp, "%f", &obji.rotate[2]);
				fscanf(fp, "%f", &obji.trans[0]);
				fscanf(fp, "%f", &obji.trans[1]);
				fscanf(fp, "%f", &obji.trans[2]);
				obj_list.push_back(obji);

			}
			fscanf(fp, "%s", input);
		}
	}
	fclose(fp);
}

void sce::apply()
{
	int n = obj_list.size();
	for (int k = 0; k < n; k++)
	{
		mesh* object;
		object = obj_list[k].obj;
		int lastMaterial = -1;
		for (size_t i = 0; i < object->fTotal; ++i)
		{
			// set material property if this face used different material
			if (lastMaterial != object->faceList[i].m)
			{
				lastMaterial = (int)object->faceList[i].m;
				glMaterialfv(GL_FRONT, GL_AMBIENT, object->mList[lastMaterial].Ka);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, object->mList[lastMaterial].Kd);
				glMaterialfv(GL_FRONT, GL_SPECULAR, object->mList[lastMaterial].Ks);
				glMaterialfv(GL_FRONT, GL_SHININESS, &object->mList[lastMaterial].Ns);

				//you can obtain the texture name by object->mList[lastMaterial].map_Kd
				//load them once in the main function before mainloop
				//bind them in display function here
			}
			//scene
			glPushMatrix();

			glTranslatef(obj_list[k].trans[0], obj_list[k].trans[1], obj_list[k].trans[2]);
			glRotatef(obj_list[k].angle, obj_list[k].rotate[0], obj_list[k].rotate[1], obj_list[k].rotate[2]);
			glScalef(obj_list[k].scale[0], obj_list[k].scale[1], obj_list[k].scale[2]);

			glBegin(GL_TRIANGLES);
			for (size_t j = 0; j<3; ++j)
			{
				//textex corrd. object->tList[object->faceList[i][j].t].ptr
				glNormal3fv(object->nList[object->faceList[i][j].n].ptr);
				glVertex3fv(object->vList[object->faceList[i][j].v].ptr);
			}
			glEnd();

			glPopMatrix();
		}
	}
}

void sce::move(int x, int y)
{
	if (select_flg >= 0 && select_flg < obj_list.size())
	{
		obj_list[select_flg].trans[0] += x;
		obj_list[select_flg].trans[1] += y;
	}
}
