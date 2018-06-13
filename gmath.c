#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;

  normalize(normal);
  normalize(view);
  normalize(light[LOCATION]);

  color a = calculate_ambient( alight, areflect );
  color d = calculate_diffuse( light, dreflect, normal );
  color s = calculate_specular( light, sreflect, view, normal );

  i.red = a.red + d.red + s.red;
  i.green = a.green + d.green + s.green;
  i.blue = a.blue + d.blue + s.blue;

  limit_color( &i );
  
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;

  a.red = alight.red * areflect[0];
  a.green = alight.green * areflect[1];
  a.blue = alight.blue * areflect[2];

  limit_color( &a );
  
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  double dot;

  dot = dot_product(normal, light[LOCATION]);
  
  d.red = light[COLOR][RED] * dreflect[0] * dot;
  d.green = light[COLOR][GREEN] * dreflect[1] * dot;
  d.blue = light[COLOR][BLUE] * dreflect[2] * dot;

  limit_color( &d );
  
  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;
  double * r = normal;
  double td = 2 * dot_product(normal, light[LOCATION]);

  r[0] = r[0] * td - light[LOCATION][0];
  r[1] = r[1] * td - light[LOCATION][1];
  r[2] = r[2] * td - light[LOCATION][2];

  double dp = dot_product( r, view );

  dp = dp > 0 ? 0 : dp;
  dp = pow(dp, SPECULAR_EXP);

  s.red = light[COLOR][RED] * sreflect[0] * dp;
  s.green = light[COLOR][GREEN] * sreflect[1] * dp;
  s.blue = light[COLOR][BLUE] * sreflect[2] * dp;

  limit_color( &s );

  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {

  c->red = (c->red > 255) ? 255 : c->red;
  c->green = (c->green > 255) ? 255 : c->green;
  c->blue = (c->blue > 255) ? 255 : c->blue;

  c->red = (c->red < 0) ? 0 : c->red;
  c->green = (c->green < 0) ? 0 : c->green;
  c->blue = (c->blue < 0) ? 0 : c->blue;

  /*
  c->green = min(255, c->green);
  c->blue = min(255, c->blue);  

  c->red = max(0, c->red);
  c->green = max(0, c->green);
  c->blue = max(0, c->blue);  
  */
  
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double norm = sqrt(dot_product(vector, vector));
  vector[0] = vector[0] / norm;
  vector[1] = vector[1] / norm;
  vector[2] = vector[2] / norm;
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] + b[2]);
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
