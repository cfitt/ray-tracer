/*
	light.cpp
Colin Fitt -- cfitt -- C6312039
Course CpSci 1020, Prof. Catherine Hochrine
Major Programming Assignment #3: Due 11:59pm Sunday, April 20th

Creates lights; Declares an instance of light_t class, 
invokes its constructor, and uses the parse to pass through 
the model.cpp. Diffused Lighting is only associated with 
specific light sources because it is reflected uniformly in 
all directions, just like lights in the motal world. In order 
to use lights, 2 parameters must be established:
	location
		postion and direction of light
	emissivity
		qualities of light

The code for glints are added to the end of illuminate(), and the
algorithm seems to be solid, though I still cannot produce glints in my
image. I have a theroy that this has something to do with what I added to
raytrace() in image.c for specular lighting that is causeing this issue.
Otherwise, I have no clue why glints would not appear.
*/

#include "ray.h"

pparm_t light_parse[] = {
	{"location", 3, sizeof(double), "%lf", 0},
	{"emissivity", 3, sizeof(double), "%lf", 0},
};

#define NUM_ATTRS (sizeof(light_parse) / sizeof(pparm_t))

light_t::light_t(FILE *in, model_t *model, int attrmax)
{
	char buf[256];
	int mask;
	list_t list;

	cookie = LGT_COOKIE;

	assert (fscanf(in, "%s", name) == 1);

	fscanf(in, "%s", buf);
	assert(buf[0] == '{');

	light_parse[0].loc = &location;
	light_parse[1].loc = &emissivity;

	mask = parser(in, light_parse, NUM_ATTRS, attrmax);
	assert(mask == 3);

	model->lgts->add((void *)this);

}

void light_t::printer(FILE *out)
{
	assert(cookie == LGT_COOKIE);
	fprintf(out, "%-12s %s \n", "light", name);
	fprintf(out, "%-12s %5.1lf %5.1lf %5.1lf \n", "location", 
				location.x, location.y, location.z);
	fprintf(out, "%-12s %5.1lf %5.1lf %5.1lf \n", "emissivity", 
				emissivity.r, emissivity.g, emissivity.b);
}

/* Produce a formatted print of the light list */
void light_list_print(model_t *model, FILE *out)
{
	list_t *list;
	light_t *light;
	

	list = model->lgts;
	list->reset();

	

	while(list->not_end())
	{
		light = (light_t *)list->get_entity();

		light->printer(out);
		list->next_link();
		
	}
}


void  light_t::getemiss(drgb_t *emiss)
{
	emiss = &emissivity;	
}

void  light_t::getloc(vec_t *loc)
{
	loc = &location;
}

void light_t::illuminate(
model_t *model,
vec_t *base,
object_t *hitobj, /* The object hit by the ray */
drgb_t *pixel) /* add illumination here */
{
	vec_t dir; // unit direction to light from hitpt
	object_t *obj; // closest object in dir to light
	double close; // dist to closest object in dir to light
	double cos; // of angle between normal and dir to light
	double dist; // distance to the light from hitpoint
	vec_t last_hitpt; // from hitobj argument
	drgb_t diffuse = {0.0, 0.0, 0.0};

	hitobj->getlast_hitpt(&last_hitpt);

	vec_diff(&last_hitpt, &location, &dir);
	dist = sqrt(vec_dot(&dir, &dir));

	vec_unit(&dir, &dir);	


	vec_t hitnorm;
	hitobj->getlast_normal(&hitnorm);

	cos = vec_dot(&hitnorm, &dir);
	if(cos < 0)
	{
		return;
	}


	if(vischeck(&last_hitpt) == 0)
		return;

	obj = find_closest_object(model, &last_hitpt, 
			&dir, hitobj, &close);

	if(obj != NULL && close < dist)
	{
		return;
	}

	hitobj->getdiffuse(&diffuse);

	pix_prod(&emissivity, &diffuse, &diffuse);

	pix_scale((cos/dist), &diffuse, &diffuse);

	pix_sum(pixel, &diffuse, pixel);
/*
//Glimer! SPARKLE! MUCH REFLECTIVE! So Not... working... :(

	double shine_value, dot, spec;;
	drgb_t emiss = {0.0, 0.0, 0.0};
	vec_t viewpt, sum;
	camera_t *cam = model->cam;
	hitobj->getshine(&shine_value);

//a) you’ll need a variable declared to hold the shininess value from the hit object ; if it’s zero,return

	if(shine_value==0)
		return;

	cam->getviewpt(&viewpt);
	vec_diff(&last_hitpt, &viewpt, &viewpt);
	vec_unit(&viewpt, &viewpt);
	vec_sum(&dir, &viewpt, &sum);
	vec_unit(&sum, &sum);

	vec_unit(&hitnorm, &hitnorm);
	dot = vec_dot(&hitnorm, &sum);
	dot = pow(dot, shine_value);

	hitobj->getspecular(&spec);
	getemiss(&emiss);
	pix_scale((spec*dot), &emiss, &emiss);

	pix_sum(&emiss, pixel, pixel);
*/
}

int light_t::vischeck(vec_t *hitloc)
{
	return 1;
}

