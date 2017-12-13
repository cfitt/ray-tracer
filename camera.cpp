/* 
	camera.cpp   

Colin Fitt -- cfitt -- C6312039
Course CpSci 1020, Prof. Catherine Hochrine
Major Programming Assignment #3: Due 11:59pm Sunday, April 20th

Declares an instance of camera_t class, invokes its constructor, 
and uses the parse to pass through the model.cpp.
other functions (most do what the name says):
	camera_t::getdir
	camera_t::store_pixel
	camera_t::scale_and_clamp
	camera_t::getxdim
	camera_t::getydim
	camera_t::getviewpt
	camera_t::camera_print
	camera_t::camera_write_image
	camera_t::randomize
		used to prodduce AA images
*/


#include"ray.h"



pparm_t cam_parse[] = {
	{"pixeldim", 2, sizeof(int), "%d", 0},
	{"worlddim", 2, sizeof(double), "%lf", 0},
	{"viewpoint", 3, sizeof(double), "%lf", 0},

};

#define NUM_ATTRS (sizeof(cam_parse) / sizeof(pparm_t))

//load in camera data
camera_t::camera_t(FILE *in)
{
	cookie = CAM_COOKIE;

	char buf[256];
	int mask;

	assert (fscanf(in, "%s", name) == 1);

	fscanf(in, "%s", buf);
	assert(buf[0] == '{');

	cam_parse[0].loc = &pixel_dim;
	cam_parse[1].loc = &world_dim;
	cam_parse[2].loc = &view_point;

	mask = parser(in, cam_parse, NUM_ATTRS, 0);
	assert(mask == 7);

	 /* Allocate a pixmap to hold the ppm image data */
	pixmap = (irgb_t *)malloc(sizeof(irgb_t) * pixel_dim[0] *pixel_dim[1]);	
}


void camera_t::getdir(int x, int y, vec_t *dir)
{
	assert(cookie == CAM_COOKIE);

	vec_t world;
	double dx = (double)x;
	double dy = (double)y;

	if (AA_SAMPLES > 1)
	{
		dx = randomize(dx);
		dy = randomize(dy);
	}

	world.x = (double)(dx * world_dim[0] / (pixel_dim[0]-1));
	world.y = (double)(dy * world_dim[1] / (pixel_dim[1]-1));
	world.z = 0.0;

	vec_diff(&view_point, &world, dir);
	vec_unit(dir, dir);
}

void camera_t::store_pixel(int x,int y, drgb_t *pix)
{
	int maprow;
	irgb_t *maploc;

	maprow = pixel_dim[1] - y - 1;
	maploc = pixmap + maprow *pixel_dim[0] + x;

	scale_and_clamp(pix);

	maploc->r = (unsigned char)pix->r;
	maploc->g = (unsigned char)pix->g;
	maploc->b = (unsigned char)pix->b;
}

void camera_t::scale_and_clamp(drgb_t *pix)
{
	pix->r = (pix->r * 255) + 0.5;
	pix->g = (pix->g * 255) + 0.5;
	pix->b = (pix->b * 255) + 0.5;

	if(pix->r > 255){
		pix->r = 255;
	}
	else if (pix->r < 0){
		pix->r = 0;
	}
	if(pix->g > 255){
		pix->g = 255;
	}
	else if (pix->g < 0){
		pix->g = 0;	
	}
	if(pix->b > 255){
		pix->b = 255;
	}
	else if (pix->b < 0){
		pix->b = 0;
	}
}

int  camera_t::getxdim(void)
{
	return(pixel_dim[0]);
}

int  camera_t::getydim(void)
{
	return(pixel_dim[1]);
}

void camera_t::getviewpt(vec_t *view)
{
	vec_copy(&view_point, view);
}

void camera_t::camera_print(FILE *out)
{
	assert(cookie == CAM_COOKIE);
	fprintf(out, "camera %-12s\n", name);
	fprintf(out, "pixeldim %d %d\n", pixel_dim[0], pixel_dim[1]);
	fprintf(out, "world_dim %0.3lf %0.1lf\n", world_dim[0], world_dim[1]);
	fprintf(out, "viewpoint %0.3lf %0.1lf %0.1lf\n",
		view_point.x, view_point.y, view_point.z);
}
void camera_t::camera_write_image(FILE *out)
{
	fprintf(out, "P6\n");
	fprintf(out, "%d %d 255\n", pixel_dim[0], pixel_dim[1]);
	fwrite(pixmap, sizeof(drgb_t), pixel_dim[0] *pixel_dim[1], out);
}

double camera_t::randomize(double z)
{
	int i;
	double id;

	i = rand();
	id = (double)i;

	id = id / 0x7fffffff;
	id = id - 0.5;
	id = id + z;

	return(id);
}
/*void camera_t::getpixsize(double *x, double *y)
{

}*/

