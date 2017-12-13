/*  
	image.c
Colin Fitt -- cfitt -- C6312039
Course CpSci 1020, Prof. Catherine Hochrine
Major Programming Assignment #3: Due 11:59pm Sunday, April 20th

Each function relies on the one above it. Determines whether the
image produces a "hit" or "miss". Code was altered to produce AA.
functions:
    	ray_trace()
	make_pixel()
	make_row()
	image_create()
*/

#include "ray.h"


//=============================================================================
// ******* ray_trace( ) *******
// 
// 
//=============================================================================
void ray_trace (
   model_t *model,
   vec_t *base,                // view point
   vec_t *dir,                 // unit direction vector
   drgb_t *pix,                // pixel return location
   double total_dist,          // distance ray has travelled so far
   object_t *last_hit)         // most recently hit object
{
   double dist;
   vec_t normal, last_hitpt;
   drgb_t thispix = {0.0, 0.0, 0.0};
   object_t *obj;
   double specref = 0.0;

	if (total_dist > MAX_DIST) 
		return;
   /* Get the object (plane or sphere).        */
	/* Later we will use find_closest_object()  */
   /* to get the closest object                */
   	// obj = find_closest_object(model, base, dir, last_hit, &dist);
   	obj = find_closest_object(model, base, dir, last_hit, &dist);

	/* get the distance and the color */
	if(obj != NULL){
   		dist = obj->hits(base, dir);  
   		obj->getambient(&thispix);
		add_illuminate(model, base, obj, &thispix);
	}
   /**  check to see if object was hit.  if so
        1)  add this distance to the total distance travelled so far
        2)  scale thispix by 1/total_distance
        3)  add thispix to pix
   **/   
	//  CODE HERE
	if(dist > 0){
		total_dist = dist + total_dist;
		pix_scale((1/total_dist), &thispix, &thispix);
		obj->getspecular(&specref);
		if(specref != 0)
		{
			drgb_t specint = {0.0, 0.0, 0.0};
			vec_t ref_dir;
			obj->getlast_normal(&normal);
			obj->getlast_hitpt(&last_hitpt);

			vec_reflect(&normal, dir, &ref_dir);
			ray_trace(model, &last_hitpt, &ref_dir, &specint, total_dist, obj);
			pix_scale(specref,&specint, &specint);
			pix_sum(&specint, &thispix, &thispix);
		}
	}
	pix_sum(&thispix, pix, pix);
}


//=============================================================================
// ******* make_pixel( ) *******
// 
// 
//=============================================================================
void make_pixel(model_t  *model, int  x, int  y) {
   vec_t raydir, view;
   drgb_t pix = {0.0, 0.0, 0.0};
   camera_t *cam = model->cam;
   int i;

   cam->getviewpt(&view);
   for(i = 0; i < AA_SAMPLES; i++)
   {
	cam->getdir(x, y, &raydir);
	ray_trace(model, &view, &raydir, &pix, 0.0, NULL);
   }


   /*  The ray_trace function determines the pixel color in */
   /*  d_rgb units.   The last two parameters are used ONLY */
   /*  in the case of specular (bouncing) rays              */
    pix_scale(1.0 / AA_SAMPLES, &pix, &pix);
    cam->store_pixel(x, y, &pix);

   return;
}


//=============================================================================
// ******* make_row( ) *******
// 
// 
//=============================================================================
void make_row(model_t  *model, int  y) 
{
   int x;
   camera_t *cam = model->cam;
   int dim_x = cam->getxdim();

   /*  for each pixel in the row, invoke make_pixel() */
   /*  to paint the pixel.                            */
	//  CODE HERE   
	for( x= 0; x < dim_x; x++){
		make_pixel(model, x, y);
	}


}


//=============================================================================
// ******* image_create( ) *******
// 
// 
//=============================================================================
void image_create(model_t  *model, FILE  *out) 
{
   int y;
   camera_t *cam = model->cam;
   int dim_y = cam->getydim();

   /*  fire rays through each pixel in the window; */
   /*  for each row, invoke make_row() to paint    */
   /*  the row of pixels.                          */
   //  CODE HERE 
	for(y = 0; y < dim_y; y++){
		make_row(model, y);
	}


   /*  write the image                           */
   cam->camera_write_image(out);
}


