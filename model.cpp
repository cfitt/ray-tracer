/* 
	model.cpp
Colin Fitt -- cfitt -- C6312039
Course CpSci 1020, Prof. Catherine Hochrine
Major Programming Assignment #3: Due 11:59pm Sunday, April 20th

Although this file was given, it needed quite a few changes.
Each new addition to the program needed a similar addition 
to model_t::model_load_entity, which checks what is
in the .txt file used.

When testing, be sure to use the correct names of objects and
types, or this with fail and return a bad attribute. Names used
will be over the new instance call for each type.

*/

#include "ray.h"

extern int oparse;


#define NUM_ITEMS (sizeof(items) / sizeof(char *))


/* Load model data */
void model_t::model_load(FILE  *in) {
   char entity[16];
   int  count;

   memset(entity, 0, sizeof(entity));

	/* Here entity should be one of "material",    */
	/* "light", "plane"                            */
   count = fscanf(in, "%s", entity);
   while (count == 1)
   {
      model_load_entity(in, entity);
      count = fscanf(in, "%s", entity);
   }
}


void model_t::model_load_entity(FILE  *in, char  *entity) {

   if (strcmp(entity, "camera") == 0)
       cam = new camera_t(in);

   else if (strcmp(entity, "material") == 0)
       new material_t(in, this, 0);

    else if (strcmp(entity, "light") == 0)
       new light_t(in, this, 0);

    else if (strcmp(entity, "spotlight") == 0)
       new spotlight_t(in, this, 3);

   else if (strcmp(entity, "plane") == 0)
      new plane_t(in, this, 0);

    else if (strcmp(entity, "triangle") == 0)
       new triangle_t(in, this, 4);

   else if (strcmp(entity, "tiled_plane") == 0)
      new tplane_t(in, this, 4);

   else if (strcmp(entity, "fplane") == 0)
      new fplane_t(in, this, 0);

    else if (strcmp(entity, "sphere") == 0)
       new sphere_t(in, this, 0);

    else {
       fprintf(stderr, "bad entity %s \n", entity);
       exit(1);
    }

   return;
}


/* Init model data */
model_t::model_t(FILE  *in) {

   mats = new list_t;
   assert(mats != NULL);

   lgts = new list_t;
   assert(lgts != NULL);

   objs = new list_t;
   assert(objs != NULL);

   model_load(in);
}


/* print model data */
void model_t::print(FILE  *out) {
   cam->camera_print(out);
   material_list_print(this, out);
   object_list_print(this, out);
   // light_list_print(this, out);
}

