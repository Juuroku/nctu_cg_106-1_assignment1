#include <stdio.h>
#include "light.h"

ambient::ambient()
{
	data[3] = 1.0;
}

lighting::lighting(GLfloat* p, GLfloat* a, GLfloat* d, GLfloat* s)
{
	position[0] = p[0];
	position[1] = p[1];
	position[2] = p[2];
	position[3] = 1.0;
	ia[0] = a[0];
	ia[1] = a[1];
	ia[2] = a[2];
	ia[3] = 1.0;
	id[0] = d[0];
	id[1] = d[1];
	id[2] = d[2];
	id[3] = 1.0;
	is[0] = s[0];
	is[1] = s[1];
	is[2] = s[2];
	is[3] = 1.0;
}

light_info::light_info()
{
	light_list.clear();
}

void light_info::add_light(char* filename)
{
	FILE *fp;
	char pr[50];
	printf("open %s ......\n", filename);
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
			if (!strcmp(pr, "light"))
			{
				printf("loading light\n");
				GLfloat p[4], a[4], d[4], s[4];
				fscanf(fp, "%f", &p[0]);
				fscanf(fp, "%f", &p[1]);
				fscanf(fp, "%f", &p[2]);
				fscanf(fp, "%f", &a[0]);
				fscanf(fp, "%f", &a[1]);
				fscanf(fp, "%f", &a[2]);
				fscanf(fp, "%f", &d[0]);
				fscanf(fp, "%f", &d[1]);
				fscanf(fp, "%f", &d[2]);
				fscanf(fp, "%f", &s[0]);
				fscanf(fp, "%f", &s[1]);
				fscanf(fp, "%f", &s[2]);
				lighting lg(p,a,d,s);
				light_list.push_back(lg);
				/*for (int i = 0; i < 3; i++)
				{
					printf("%f %f %f %f\n", p[i], a[i], d[i], s[i]);
				}*/
			}
			else if (!strcmp(pr, "ambient"))
			{
				printf("loading ambient\n");
				GLfloat R, G, B;
				fscanf(fp, "%f", &R);
				fscanf(fp, "%f", &G);
				fscanf(fp, "%f", &B);
				amb.data[0] = R;
				amb.data[1] = G;
				amb.data[2] = B;
				/*for (int i = 0; i < 4; i++)
				{
					printf("%f ", amb.data[i]);
				}
				printf("\n");*/
			}
			fscanf(fp, "%s", pr);
		}
	}
	fclose(fp);
}