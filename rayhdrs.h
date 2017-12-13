

/* = = = = = = = = = = = = = = camera.c = = = = = = = = = = = = = = = =  */

void camera_getdir(camera_t  *cam, int  x, int  y, vec_t  *dir);
void camera_store_pixel(camera_t  *cam, int  x, int  y, drgb_t  *pix);

/* Initialize camera data */
void camera_init(FILE  *in, model_t  *model, int  attrmax);
int camera_load_attributes(FILE  *in, camera_t  *cam);

void camera_print(camera_t  *cam, FILE  *out);
void camera_write_image(camera_t  *cam, FILE  *out);
void scale_and_clamp(drgb_t *);


/* = = = = = = = = = = = = = = list.c = = = = = = = = = = = = = = = = = */

/* malloc a new list header and initialize it */
list_t *list_init(void);

/* Add an element to the end of a list */
void list_add(list_t  *list, void  *entity);

void list_reset(list_t  *list);
int list_not_end(list_t  *list);
void list_next_link(list_t  *list);
void *list_get_entity(list_t  *list);


/* = = = = = = = = = = = = = model.c = = = = = = = = = = = = = = = = = */

/* Init model data */
model_t *model_init(FILE  *in);
void model_load_entities(FILE *in, model_t *model);
void model_print(model_t  *model, FILE  *out);


/* = = = = = = = = = = = = material.c = = = = = = = = = = = = = = = = */

/* Create a new material description */
void material_init(FILE  *in, model_t  *model, int  attrmax);
void material_load_attributes(FILE  *in, material_t  *mat);

/* Produce a formatted dump of the material list */
void material_list_print(model_t  *model, FILE  *out);
void material_print(material_t  *mat, FILE  *out);

/* Try to locate a material by name */
material_t *material_getbyname(model_t  *model, char  *name);

/* material getter functions */
void material_getambient(object_t  *obj, material_t  *mat, drgb_t  *dest);
void material_getdiffuse(object_t  *obj, material_t  *mat, drgb_t  *dest);
double material_getspecular(object_t  *obj, material_t  *mat);


/* = = = = = = = = = = = = = raytrace.c = = = = = = = = = = = = = = = = = */

/* This function traces a single ray and returns the composite intensity  */
/* of the light it encounters.     													  */
/*		base is the location of viewer or previous hit                      */
/* 	dir is the unit vector in the direction of the object               */
/*		pix is the pixel return location                                    */
/*		total_dist is the distance the ray has traveled so far              */
void ray_trace(model_t  *model, vec_t  *base, vec_t  *dir, drgb_t  *pix,        
					double  total_dist, object_t  *last_hit);
object_t *find_closest_object(model_t  *model, vec_t  *base, vec_t  *dir, 
				object_t  *last_hit, double  *retdist);
void add_illuminate(model_t *model, vec_t *base, object_t *hitobj, drgb_t *pixel);




/* = = = = = = = = = = = = = = = object.c = = = = = = = = = = = = = = =  */

/* Create a new object description */
void object_init(FILE  *in, model_t  *model);

void object_list_print(model_t  *model, FILE  *out);
void object_print(object_t  *obj, FILE  *out);

/* Find the closest object intersection on the ray based at "base" and  */
/* having direction "dir"                                               */
/* 	list is the object list										               */
/*		base is the base of the ray								               */
/* 	dir is the direction of the ray							               */
/* 	last_hit is the object's last hit point				               */
object_t *find_closest_object(model_t  *model, vec_t  *base, vec_t  *dir,         
				object_t  *last_hit, double  *retdist);

/* Returns -1.0, which means a miss                                     */
/* 	obj is the candidate object                                       */
/* 	base if the starting point of the ray                             */
/*		dir is the direction of the ray - MUST be a unit vector           */
double object_no_hit(object_t  *obj, vec_t  *base, vec_t  *dir);


/* = = = = = = = = = = = = = = = plane.c = = = = = = = = = = = = = = = = */
/* = = = = = = = = = =  a derived type of object = = = = = = = = = = = = */

/* Create a new plane object and initialize it */
void plane_init(FILE  *in, model_t  *model, int  attrmax);
int plane_load_attributes(FILE  *in, plane_t  *pln, int  attrmax);

void plane_print(object_t  *obj, FILE  *out);

/* Determine if a vector projected from location *base in direction *dir  */
/* hits the plane.  If so, the return value is the distance from the base */
/* to the point of contact.															  */
/* 	base is the ray base										                    */
/*		dir is the unit direction vector  	*/		

double  plane_hits(object_t  *obj, vec_t  *base, vec_t  *dir);     

/* = = = = = = = = = = = = = = = tplane.c = = = = = = = = = = = = = = = */
/* = = = = = = = = = =  a derived type of plane = = = = = = = = = = = = */
/* = = = = = = = which is a derived type of object  = = = = = = = = = = */

/* The tiled plane is derived from the generic plane.  Additional input */
/* includes the material name of the alternate or background tile, the  */
/* x-y dimension of the tiling, and a vector that, when projected into  */
/* the plane, provides the x-direction of the tiling.                   */
void tplane_init(FILE  *in, model_t  *model);
int tplane_load_attributes(FILE  *in, tplane_t  *tpln, int  attrmax);

void tplane_print(object_t  *obj, FILE  *out);


void tplane_ambient(object_t  *obj, material_t *mat, drgb_t  *value);
int tplane_foreground(object_t  *obj);


/* = = = = = = = = = = = = = = = pplane.c = = = = = = = = = = = = = = = */
/* = = = = = = = = = =  a derived type of plane = = = = = = = = = = = = */

void pplane_init(FILE  *in, model_t  *model, int  attrmax);
int pplane_load_attributes(FILE  *in, pplane_t  *ppln, int  attrmax);

void pplane_print(object_t  *obj, FILE  *out);

void pplane0_ambient(object_t  *obj, material_t  *mat, drgb_t  *value);
void pplane1_ambient(object_t  *obj, material_t  *mat, drgb_t  *value);


/* = = = = = = = = = = = = = = triangle.c = = = = = = = = = = = = = = =  */
/* = = = = = = = = = =  a derived type of object = = = = = = = = = = = = */

object_t *triangle_init(FILE  *in, model_t  *model, int  attrmax);

/* = = = = = = = = = = = = = = = sphere.c = = = = = = = = = = = = = = =  */
/* = = = = = = = = = =  a derived type of object = = = = = = = = = = = = */

/* Create a new sphere object and initialize it */
void sphere_init(FILE  *in, model_t  *model, int  attrmax);
int sphere_load_attributes(FILE  *in, sphere_t  *sph, int  attrmax);

void sphere_print(object_t  *obj, FILE  *out);

/* Determine if a vector projected from location *base in direction *dir  */
/* hits the sphere. If so, the return value is the distance from the base */
/* to the point of contact. 															  */
/* 	NOTE: dir MUST be a unit vector					     */
double  sphere_hits(object_t  *obj, vec_t  *base,  vec_t  *dir);   


/* = = = = = = = = = = = = = = ellipse.c = = = = = = = = = = = = = = = = */
/* = = = = = = = = = =  a derived type of sphere = = = = = = = = = = = = */
/* = = = = = = = = which is a derived type of object = = = = = = = = = = */

/* Create a new ellipse object and initialize it */
object_t *ellipse_init(FILE  *in, model_t  *model, int  attrmax);

void ellipse_print(object_t  *obj, FILE  *out);



/* = = = = = = = = = = = = = = = = light.c = = = = = = = = = = = = = =  */

/* Create a new light description */
light_t *light_init(FILE  *in, model_t  *model, int  attrmax);

void light_list_print(model_t  *model, FILE  *out);

void light_getloc(light_t  *lt, vec_t  loc);
void light_getemiss(light_t  *lt, drgb_t  pix);
int  light_getvis(light_t  *lt, vec_t  ir);


/* = = = = = = = = = = = = = = = = illum.c = = = = = = = = = = = = = = */

void rand_vec(vec_t  in, vec_t  out);

/* hitobj is the object that was hit by the ray  */
void illuminate(model_t  *model, object_t  *hitobj, drgb_t  pixel);



/* = = = = = = = = = = = = = = image.c = = = = = = = = = = = = = = = = */

/* This function is the driver for the raytracing procedure */
void image_create(model_t  *model, FILE  *);



/* = = = = = = = = = = = = = = parser.c = = = = = = = = = = = = = = =  */

/* Generalized attribute parser.                                         */
/* It returns a bit mask in which each possible attribute is represented */
/* by a bit on exit. The attributes that have been found will have their */
/* bit = 1.                                                              */
/* 	pct is the pcarser control table							                */
/* 	numattrs is the number of legal attributes			                */
int parser(FILE  *in, pparm_t  *pct, int  numattrs, int  attrmax);





