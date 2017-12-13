/*
	spotlight.cpp
Colin Fitt -- cfitt -- C6312039
Course CpSci 1020, Prof. Catherine Hochrine
Major Programming Assignment #3: Due 11:59pm Sunday, April 20th

The spotlight_t is a derived class of light_t. The spotlight will 
illuminate the hit location if and only if a vector from the location 
of the spotlight (light) to the hit location lies inside the spot 
cone. The vischeck() method will check for that.
2 parameters need to be read in:
	theta
		half angle in degrees	
	point
		point the centerline hits
*/


#include"ray.h"

pparm_t spotl_parse[] = {
	//{"location", 3, sizeof(double), "%lf", 0},
	//{"emissivity", 3, sizeof(double), "%lf", 0},
	{"theta", 1, sizeof(double), "%lf", 0},
	{"point", 3, sizeof(double), "%lf", 0},
};

#define NUM_ATTRS (sizeof(spotl_parse) / sizeof(pparm_t))

spotlight_t::spotlight_t(FILE *in, model_t *model, int attrmax) :light_t(in,model,2)
{
	int mask;
	double rtheta;

	//strcpy(name, "spotlight");
	//light_t(in,model,attrmax);
	/* The parser is fairly generic but the address of where */
	/* to put the data must be updated for each new object */
	spotl_parse[0].loc = &theta;
	spotl_parse[1].loc = &point;
	//spotl_parse[2].loc = &costheta;

	mask = parser(in, spotl_parse, NUM_ATTRS, attrmax);
	assert(mask >= 0);

	vec_diff(&location, &point, &direction);
	vec_unit(&direction,&direction);

	rtheta = theta * (M_PI/180);
	costheta = cos(rtheta);
}

int spotlight_t::vischeck(vec_t *hitloc)
{
	vec_t tmp;
	double v;

	vec_diff(&location, hitloc, &tmp);
	vec_unit(&tmp,&tmp);

	v = vec_dot(&tmp, &direction);

	if (v > costheta)
		return 1;
	else
		return 0;
}

void spotlight_t::printer(FILE *out)
{
	light_t::printer(out);

	fprintf(out, "%-12s %5.1lf %5.1lf %5.1lf \n", "point", 
				point.x, point.y, point.z);
	fprintf(out, "%-12s %5.1lf \n", "theta", theta);
}
