/*
	tplane.cpp
Colin Fitt -- cfitt -- C6312039
Course CpSci 1020, Prof. Catherine Hochrine
Major Programming Assignment #3: Due 11:59pm Sunday, April 20th

To be honest I got this completely on accident. I was getting crazy
ppms and it was not working at all, and then one day I returned to it
and it started to work. I'm not gonna mess with it cause you-know
"If it ain't broke don't fix it", but here what I think is happening.

tplane_t creates an instance of a plane_t, uses the parse, and uses
vec_cross to build a rotation matrix that rotates the plane normal 
into the z-axis, and the projected xdir into the x-axis. The
tplane_t::select() function:	

apply the rot matrix to last_hit to rotate it into the plane 
having normal (0, 0, 1) with the projected xdir of the tiling parallel
to the x-axis (creating new vector newloc; z-component = 0). 
Add 100000 to newloc.x and newloc.y (avoid doublewide stripe at origin).
Divide newloc.x / dims[0], newloc.y / dims[1], use thesum of these values
to determine if the tile is foreground or background. Then some magic
happened because this was so frustrating 3 days ago.
*/

#include "ray.h"

static pparm_t tplane_parse[] =
{
    {"xdir", 3, sizeof(double),"%lf",0},
    {"dimensions", 2, sizeof(double), "%lf", 0},
    {"altmaterial", 1, sizeof(char), "%s", 0},
};

#define NUM_ATTRS (sizeof(tplane_parse) / sizeof(pparm_t))

tplane_t::tplane_t(FILE *in, model_t *model, int attrmax):plane_t(in,model,2)
{
	char altname [NAME_LEN];
	cookie = OBJ_COOKIE;
        
	strcpy(obj_type, "tiled_plane"); //Copies over the string plane to obj_type

	tplane_parse[0].loc = &xdir; //Sends in the normal to the parser
	tplane_parse[1].loc = &dims; //Sends in the point to the parser
	tplane_parse[2].loc = &altname;

//This calls the parser which acts like the load attributes function
	parser(in, tplane_parse, NUM_ATTRS, attrmax);

	//fscanf(in, "%s", altname);
   	
	altmat = material_getbyname(model, altname);
    
	vec_project(&normal, &xdir, &projxdir);
	vec_unit(&xdir,&xdir);
    
	vec_copy(&projxdir, &rot.row[0]);
	vec_copy(&normal, &rot.row[2]);
	vec_cross(&rot.row[2], &rot.row[0], &rot.row[1]);
}

void tplane_t::printer(FILE *out)
{
	object_t::printer(out);
 
	fprintf(out,"%-12s %5.1lf %5.1lf %5.1lf\n",
			"normal",normal.x,normal.y,normal.z);    
	fprintf(out,"%-12s %5.1lf %5.1lf %5.1lf\n",
			"xdir",xdir.x,xdir.y,xdir.z);
	fprintf(out,"%-12s %5.1lf %5.1lf\n",
			"dimensions", dims[0],dims[1]);
	fprintf(out,"%-12s %-5s\n",
			"altmaterial", altmat->material_getname());
}

void tplane_t::getambient(drgb_t  *value)
{
	if(select() == 0)
	{
		object_t::getambient(value);
	}
	else
	{
		altmat->material_getdiffuse(value);
	}
}

void tplane_t::getspecular(double *value)
{
	if(select() == 0)
	{
		object_t::getspecular(value);
	}
	else
	{
		altmat->material_getspecular(value);
	}  
}

void tplane_t::getdiffuse(drgb_t *value)
{
	if(select() == 0)
	{
		object_t::getdiffuse(value);
	}
	else
	{
		altmat->material_getdiffuse(value);
	}
}

int tplane_t::select(void)
{
	vec_t newloc = {0.0, 0.0, 1};
   
	vec_xform(&rot, &last_hitpt, &newloc);
  
	newloc.x = (newloc.x + 100000) / dims[0];
	newloc.y = (newloc.y + 100000) / dims[1];
 
	if((((int)newloc.x +(int)newloc.y)%2)==0)
		return 0;
	else
		return 1;
}
