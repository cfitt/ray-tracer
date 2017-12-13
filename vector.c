/**  
	vector.c   

Colin Fitt -- cfitt -- C6312039
Course CpSci 1020, Prof. Catherine Hochrine
Major Programming Assignment #3: Due 11:59pm Sunday, April 20th

Old vector functions below. Produce vector addition, 
subrtaction and many more used throughout the project.
vec_t structure type is declared in vector.h, along the 
mtx_t (matrix) type used in various functions.

**/

#include "vector.h"

/*  This file contains function definitions for    */
/*  for 3D vector operations                       */

/* Compute the sum of two vectors */
/* v3 = v2 + v1; */
void vec_sum(
vec_t *v1,   /* addend */
vec_t *v2,   /* addend */
vec_t *v3)   /* result */
{
	v3->x = (v1->x + v2->x);
	v3->y = (v1->y + v2->y);
	v3->z = (v1->z + v2->z);
}

/* Compute the difference of two vectors */
/* v3 = v2 - v1 */
void vec_diff(
vec_t *v1,   /* subtrahend */
vec_t *v2,   /* minuend    */
vec_t *v3)   /* result     */
{
	v3->x = (v2->x - v1->x);
	v3->y = (v2->y - v1->y);
	v3->z = (v2->z - v1->z);
}

/* Compute the componentwise product of two vectors */
/* v3 = v2 * v1; */
void vec_mult( 
vec_t *v1,   /* Input vector 1 */
vec_t *v2,   /* Input vector 2 */
vec_t *v3)   /* result         */
{
	v3->x = (v1->x * v2->x);
	v3->y = (v1->y * v2->y);
	v3->z = (v1->z * v2->z);
}

/* Return the (dot) inner product of two input vectors     */
double vec_dot(
vec_t *v1,   /* Input vector 1 */
vec_t *v2)   /* Input vector 2 */
{
	return ((v1->x)*(v2->x) + (v1->y)*(v2->y) + (v1->z)*(v2->z));
}

/* Return length of a 3d vector */
double vec_len(
vec_t *v1)  /* Vector whose length is desired */
{
	return sqrt (pow(v1->x, 2) + pow(v1->y, 2) + pow(v1->z, 2));
}

/* Scale a 3d vector by multiplying each element by scale factor */
void vec_scale(
double fact,   /* Scale factor */
vec_t *v1,      /* Input vector */
vec_t *v2)      /* Output vector */
{
	v2->x = v1->x * fact;
	v2->y = v1->y * fact;
	v2->z = v1->z * fact;
}

/* Construct a unit vector v2 in direction of input vector v1 */
void vec_unit(
vec_t *v1,    /* Input vector      */
vec_t *v2)    /* output unit vec   */
{
	double scale = 1.0 / vec_len(v1);
	vec_scale(scale, v1, v2);
}

/* Copy vector v1 to v2 another */
void vec_copy(
vec_t *v1,   /* input vector */
vec_t *v2)   /* output vector */
{
	v2->x = v1->x;
	v2->y = v1->y;
	v2->z = v1->z;
}

/* Read in values of vector from file */
/* Return the # of items read	*/
int vec_read(
FILE *in,     /* input file  */
vec_t *v1)    /* vector from input file */
{
	return fscanf(in, "%lf %lf %lf", &v1->x, &v1->y, &v1->z);
}

/* Print values of vector to file */
void vec_print(
FILE *out,     /* output file */
char *label,   /* label string */
vec_t *v1)      /* vector to print */
{
	fprintf(out,"%s %8.3lf %8.3lf %8.3lf\n", label, v1->x, v1->y, v1->z);
}

void vec_cross(
  vec_t *v1,    /* Left input vector  */
  vec_t *v2,    /* Right input vector */
  vec_t *v3)
{
	vec_t a;
        a.x = v1->y * v2->z - v1->z * v2->y;
        a.y = v1->z * v2->x - v1->x * v2->z;
        a.z = v1->x * v2->y - v1->y * v2->x;
        vec_copy(&a, v3);
}

void vec_project(
  vec_t *n,       /* plane normal     */
  vec_t *v,       /* input vector     */
  vec_t *p)
{
	vec_t   t;
	double  ndotv;
	ndotv = vec_dot(n, v);
	vec_scale(ndotv, n, &t);
	vec_diff(&t, v, p);
}

void vec_reflect(
  vec_t *n,       /* surface normal      */
  vec_t *w,       /* incoming ray vector */
  vec_t *v)
{
	double udotn;
	vec_t u, s, f;//

	vec_scale(-1,w,&u);
	vec_scale(2,n,&s);
	udotn = vec_dot(&u, n);
	vec_scale(udotn, &s, &f);
	vec_diff(&u, &f, v);
}

void vec_xform(
  mtx_t *m,
  vec_t *v1,
  vec_t *v2)
{
        vec_t a;

        a.x = vec_dot(&m->row[0], v1);
        a.y = vec_dot(&m->row[1], v1);
        a.z = vec_dot(&m->row[2], v1);
        vec_copy(&a, v2);
}

void mtx_transpose(
  mtx_t *m1,
  mtx_t *m2)
{
	mtx_t tp;

        tp.row[0].x = m1->row[0].x;
        tp.row[0].y = m1->row[1].x;
        tp.row[0].z = m1->row[2].x;

        tp.row[1].x = m1->row[0].y;
        tp.row[1].y = m1->row[1].y;
        tp.row[1].z = m1->row[2].y;

        tp.row[2].x = m1->row[0].z;
        tp.row[2].y = m1->row[1].z;
        tp.row[2].z = m1->row[2].z;
        
        vec_copy(&tp.row[0], &m2->row[0]);
        vec_copy(&tp.row[1], &m2->row[1]);
        vec_copy(&tp.row[2], &m2->row[2]);
}
