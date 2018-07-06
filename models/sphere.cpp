#ifndef MODELS_SPHERE
#define MODELS_SPHERE

#include "../lib/trig.cpp"

/*
 * Draw vertex in polar coordinates
 * From ex18.c by Willem A. (Vlakkies) Schreuder
 * I added texture scaling so the pineapple texture could repeat
 */
void PolarVertex(int th, int ph, double tex_scale) {
  double x = -Sin(th) * Cos(ph);
  double y =            Sin(ph);
  double z =  Cos(th) * Cos(ph);
  glNormal3d(x,y,z); // To future self: try flipping y and z
  glTexCoord2d(th/360.0*tex_scale,(ph/180.0+0.5)*tex_scale);
  glVertex3d(x,y,z);
}
/*
 * Draw sphere
 * From ex18.c by Willem A. (Vlakkies) Schreuder
 * I added texture scaling so the pineapple texture could repeat
 */
void sphere(double x, double y, double z, double radius, double tex_scale) {
  glPushMatrix();
  glTranslated(x,y,z);
  glScaled(radius,radius,radius);
  for(int sph = -90; sph < 90; sph += 5) {
    glBegin(GL_QUAD_STRIP);
    for (int sth = 0; sth <= 360; sth += 5) {
      PolarVertex(sth, sph, tex_scale);
      PolarVertex(sth, sph+5, tex_scale);
    }
    glEnd();
  }
  glPopMatrix();
}
#endif
