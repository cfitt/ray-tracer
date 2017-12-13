/*
	triangle.cpp
Colin Fitt -- cfitt -- C6312039
Course CpSci 1020, Prof. Catherine Hochrine
Major Programming Assignment #3: Due 11:59pm Sunday, April 20th

triangle_t is a derived class of plane_t. I originally had it set as it's own
class seperate from plane (hacked into object_t), but I ran into some issues 
making tiled triangles,which called for the change to the plane_t class type. 
3 vertex's are read in, which the normal and hitpoint are derived with math, 
and help with the algorithm used in plane hits.

*/
#include "ray.h"

#define NUM_ATTRS (sizeof(tri_parse) / sizeof(pparm_t))

pparm_t tri_parse[] = {
        {"vertex1", 3, sizeof(double), "%lf", 0},
        {"vertex2", 3, sizeof(double), "%lf", 0},
	{"vertex3", 3, sizeof(double), "%lf", 0},
};

triangle_t::triangle_t(FILE *in, model_t *model, int attrmax):plane_t(in,model,2)
{
	int  mask;
	vec_t tv1, tv2;

	strcpy(obj_type, "triangle");

	tri_parse[0].loc = &vertex1;
	tri_parse[1].loc = &vertex2;
	tri_parse[2].loc = &vertex3;
	mask = parser(in, tri_parse, NUM_ATTRS, attrmax);
	assert(mask == 7);

//find normal 
	vec_diff(&vertex2, &vertex1, &tv1);
	vec_diff(&vertex3, &vertex1, &tv2);
	vec_unit(&tv1, &tv1);
	vec_unit(&tv2, &tv2);
	vec_cross(&tv1, &tv2, &normal);
	vec_copy(&normal, &last_normal);

	ndotq = vec_dot(&normal, &vertex1);

}

void triangle_t::printer(FILE *out)
{
        object_t::printer(out);

        fprintf(out, "%-12s %5.1lf %5.1lf %5.1lf \n",
                "normal", normal.x, normal.y, normal.z);
	fprintf(out, "%-12s %5.1lf %5.1lf %5.1lf \n",
		"point1", vertex1.x, vertex1.y, vertex1.z);
	fprintf(out, "%-12s %5.1lf %5.1lf %5.1lf \n",
		"point2", vertex2.x, vertex2.y, vertex2.z);
	fprintf(out, "%-12s %5.1lf %5.1lf %5.1lf \n",
		"point3", vertex3.x, vertex3.y, vertex3.z);
//
}


double triangle_t::hits(vec_t *base, vec_t *dir)
{
	double a, b, c, t, ndotd, ndotb, total;
	vec_t  e_1,  e_2,  e_3, hitpt;

	ndotd = vec_dot(dir, &normal);

	if (ndotd == 0)
		return -1;
	
	ndotb = vec_dot(&normal, base);
	t = (ndotq - ndotb) / ndotd;

	if (t <= 0)
		return(-1);
	
	vec_scale(t, dir, &hitpt);
	vec_sum(&hitpt, base, &hitpt);

	if (hitpt.z > 0)
		return(-1);

//	t = plane_t::hits(base, dir);
	
	vec_diff(&hitpt, &vertex1, &e_1);
	vec_diff(&hitpt, &vertex2, &e_2);
	vec_diff(&hitpt, &vertex3, &e_3);

	vec_unit(&e_1 ,&e_1);
	vec_unit(&e_2 ,&e_2);
	vec_unit(&e_3 ,&e_3);

	a = vec_dot(&e_1, &e_2);
	b = vec_dot(&e_2, &e_3);
	c = vec_dot(&e_3, &e_1);

	total = a + b + c;

	if (total > -1)
	{	
		return -1;
	} else {
		vec_copy(&hitpt, &last_hitpt);
		return(t);	
	}
}


