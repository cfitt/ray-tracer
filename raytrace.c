/*  
	raytrace.c
Colin Fitt -- cfitt -- C6312039
Course CpSci 1020, Prof. Catherine Hochrine
Major Programming Assignment #3: Due 11:59pm Sunday, April 20th

Find the closest object intersection on the ray based
at "base" and having direction "dir".
If no object is hit, then retdist will have the value -1, 
and NULL will be returned. If an object is hit, retdist 
will end up with the mindist and minobj will be a non-NULL 
object that is returned.

add_illuminate() funtion was added to this file, which is
simply a call to light's illumiate function.

*/

#include "ray.h"

object_t *find_closest_object(
        model_t  *model,
        vec_t  *base,
        vec_t  *dir,
        object_t  *last_hit,
        double  *retdist)
{
        object_t *minobj = NULL;
        double mindist = -1.0;

        object_t *obj;
        double dist = -1;

	list_t *list = model->objs;
        //list_reset(model->objs);

	list->reset();

	//New
	while(list->not_end()){
		obj = (object_t *)list->get_entity();
		if(obj != last_hit)
		{
			dist = obj->hits(base, dir);
			
			if(dist > 0)
                        {
                                if(mindist == -1.0 || dist < mindist)
                                {
                                        mindist = dist;
                                        minobj = obj;
                                }
                        }
		}
	list->next_link();
	}

        // save the mindist to retdist
        *retdist = mindist;
        // and return the minobj
        return(minobj);
}

void add_illuminate(
   model_t *model, /* object list */
   vec_t *base, /* base of ray.. not used yet */
   object_t *hitobj, /* object that was hit by the ray */
   drgb_t *pixel) /* where to add intensity */
{
	light_t *light;
	list_t *list;
	list = model->lgts;

	list->reset();

	while(list->not_end())
	{
		light = (light_t *)list->get_entity();
		
		// get light from list
		light->illuminate(model, base, hitobj, pixel);
		// illuminate( ) resides in light.cpp
		// go to next link in list
		list->next_link();
	
		
	}
}
