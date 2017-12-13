/*
	fplane.cpp
Colin Fitt -- cfitt -- C6312039
Course CpSci 1020, Prof. Catherine Hochrine
Major Programming Assignment #3: Due 11:59pm Sunday, April 20th

this creates a finite plane, which is a rectangular region of 
finite area within a general plane. It is a derived class
of the general plane, but loads in 2 new constraints:
	xdir
		a vector, when projected specifies the x direction
`		y direction is implicitly given by the cross product 
		of the unit plane normal and the unitized, projected xdir
	dimensions
		first dimension is the size in the x direction
		the second the size in the y direction

*/
#include "ray.h"

pparm_t fplane_parse[] =
{
	{"xdir", 3, sizeof(double), "%lf", 0}, 
	{"dimensions", 2, sizeof(double), "%lf", 0},
};

#define NUM_ATTRS (sizeof(fplane_parse) / sizeof(pparm_t))

fplane_t::fplane_t(FILE *in, model_t *model, int attrmax) : plane_t(in, model, 2)
{
	int mask;
	strcpy(obj_type, "fplane");
	fplane_parse[0].loc = &xdir;
	fplane_parse[1].loc = &dims;
	mask = parser(in, fplane_parse, NUM_ATTRS, attrmax);
	assert(mask == 3);
	
	//project xdir onto the plane creating projxdir
	vec_unit(&normal, &normal);
	vec_project(&normal,&xdir,&projxdir);

	fprintf(stderr,"xdir:%lf %lf %lf\n", xdir.x, xdir.y, xdir.z);
	fprintf(stderr,"projxdir: %lf %lf %lf\n", projxdir.x, projxdir.y, projxdir.z);
	
	// check
	if((projxdir.x == projxdir.y) && (projxdir.x == projxdir.z))
		assert(projxdir.x != 0.0);
	
	
	vec_unit(&projxdir, &projxdir);

	vec_copy(&projxdir, &rot.row[0]);
	vec_copy(&normal, &rot.row[2]);

	vec_unit(&rot.row[2], &rot.row[2]);
	vec_cross(&rot.row[2],&rot.row[0],&rot.row[1]);
	//ndotq = vec_dot(&point, &normal);
}

void fplane_t::printer(FILE *out) 
{
	plane_t::printer(out);
	fprintf(out,"\n%-12s %5.1lf %5.1lf %5.1lf","xdir",xdir.x,xdir.y,xdir.z);	
	fprintf(out,"\n%-12s %5.1lf %5.1lf %5.1lf","projxdir",projxdir.x,projxdir.y,projxdir.z);
	fprintf(out,"\n%-12s %5.1lf %5.1lf","dimensions",dims[0],dims[1]);
	fprintf(out,"\n%-12s %5.1lf %5.1lf %5.1lf","rot",rot.row[0].x,rot.row[0].y,rot.row[0].z);
	fprintf(out,"\n%-12s %5.1lf %5.1lf %5.1lf","rot",rot.row[1].x,rot.row[1].y,rot.row[1].z);
	fprintf(out,"\n%-12s %5.1lf %5.1lf %5.1lf","rot",rot.row[2].x,rot.row[2].y,rot.row[2].z);
}

double fplane_t::hits(
	vec_t	*base,	/* ray base			*/
	vec_t	*dir)		/* unit direction */
{
	vec_t newloc;
	double t;
	

	t = plane_t::hits(base, dir);
	
	if(t == -1)
	{
		return(-1);
	}
	
	vec_diff(&point, &last_hitpt, &newloc);
	vec_xform(&rot, &newloc, &newloc);
	
	if((0 <= newloc.x) && (newloc.x <= dims[0]))
	{
		if (0<=newloc.y && newloc.y <= dims[1])
		{
			vec_copy(&newloc,&rothit);
			return(t);
		}
	}
	return(-1);
}

