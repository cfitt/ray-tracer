/*
	sphere.cpp
Colin Fitt -- cfitt -- C6312039
Course CpSci 1020, Prof. Catherine Hochrine
Major Programming Assignment #3: Due 11:59pm Sunday, April 20th

This program parses and loads the data for the sphere structure.
Also a printer funtion.
*/


#include "ray.h"

pparm_t sphere_parse[] = {
	{"center", 3, sizeof(double), "%lf", 0},
	{"radius", 1, sizeof(double), "%lf", 0},
};

#define NUM_ATTRS (sizeof(sphere_parse) / sizeof(pparm_t))

sphere_t::sphere_t( FILE *in, model_t *model, int attrmax) : object_t(in, model)
{
	int mask;
	strcpy(obj_type, "sphere");
/* The parser is fairly generic but the address of where to */
/* put the data must be updated for each new object         */
	sphere_parse[0].loc = &center;
	sphere_parse[1].loc = &radius;
	mask = parser(in, sphere_parse, NUM_ATTRS, attrmax);
	assert(mask == 3);
//	vec_unit(radius, radius);
//	vec_copy(radius, hitnorm);
}

double sphere_t::hits(vec_t *base, vec_t *dir)
{
	vec_t hit, sdir, hitdir;
//sphere_t *sph;
//sph = (sphere_t *)obj->priv; 
	double a, b, c, t, d;
	vec_t vloc;
	vec_diff(&center, base, &vloc);
	a = vec_dot(dir, dir);
	b = 2.0 * vec_dot(&vloc, dir);
	c = vec_dot(&vloc, &vloc) - (radius * radius);

	d = b * b - 4 * a * c;

	if(d > 0.0)
	{
		t = (b *(-1.0) - sqrt(d)) / (2 * a);
	}
	else
	{
		return (-1);
	}

	vec_scale(t, dir, &sdir);
	vec_sum(base, &sdir, &hit);

	if(hit.z < 0.0)
	{
		vec_copy(&hit, &last_hitpt);
		vec_diff(&center, &hit, &hitdir);
		vec_unit(&hitdir, &last_normal);
		return(t);
	}
	if(d > 0.0)
	{
		t = (b * (-1.0) - sqrt(d)) / (2 * a);
		return(t);
	}
	else
	{
	return(-1);
	}
}

void sphere_t::printer(FILE *out)
{
	
	fprintf(out, "%-12s %5.1lf %5.1lf %5.1lf \n", "center", center.x, center.y, center.z);
	fprintf(out, "%-12s %5.1lf \n", "radius", radius);
}
