#ifndef MODELS_GRAPE
#define MODELS_GRAPE

#include "sphere.cpp"

unsigned int DL_GRAPE;

void compile_grape();

void init_grape() {
  DL_GRAPE = glGenLists(1);
  glNewList(DL_GRAPE, GL_COMPILE);
  compile_grape();
  glEndList();
}

/*
 * Compile the grape display list (called once on startup)
 */
void compile_grape() {
  glPushMatrix();
  glTranslated(0,0.5,0); // centered on x & z but y is bottom
  glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);
  
  // Enable lighting
  glEnable(GL_LIGHTING);
  setLightIntensities(0.3, 0.5, 0.1, 70);
  
  glColor3f(0.6,0.1,0.8); // purple
  // draw spheres in a kind of spiral shape
  double grape_radius = .15;
  for(int layer = 0; layer < 9; layer++) {
    double layer_radius = layer * grape_radius * 0.7;
    if(layer > 4) layer_radius -= (layer - 5)*grape_radius;
    int grapes_this_layer = (int) (layer_radius * 3.14 / grape_radius);
    double angle = 360.0 / grapes_this_layer;
    for(int grape = 0; grape < grapes_this_layer; grape++) {
      double x = layer_radius * Cos((layer*15) + (angle * grape));
      double y = layer_radius * Sin((layer*15) + (angle * grape));
      double z = 0.7 - (grape_radius * layer);
      sphere(x,y,z, grape_radius, 1);
    }
  }
  // put a giant grape monster inside it to fill holes between grapes
  sphere(0,0,-.2, 0.4, 1);
  
  
  // leaves
  glTranslated(0, 0, -.8);
  glEnable(GL_TEXTURE_2D);
  glColor3d(1,1,1);
  setLightIntensities(0.3, 0.5, 0, 0);
  glBindTexture(GL_TEXTURE_2D, LEAF_TEXTURE);
  glDisable(GL_CULL_FACE); // they're 2d so don't cull the back face
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // set up 2d lighting
  // if these normals all seem counterintuitive it's because
  // I modeled this the other way but
  // GL_LIGHT_MODEL_TWO_SIDE + emission lighting is broken
  for(int i = 0; i < 3; i++) {
    glPushMatrix();
    glRotated(360.0 / 3.0 * i, 0, 0, 1);
    glBegin(GL_QUAD_STRIP);
    glNormal3d(0,-0.3,-1);
    glTexCoord2d(1, 0.0); glVertex3d(+0.1, 0, 0.3);
    glTexCoord2d(0, 0.0); glVertex3d(-0.1, 0, 0.3);
    glNormal3d(0,0,-1);
    glTexCoord2d(1, 0.2); glVertex3d(+0.2, 0.1, 0);
    glTexCoord2d(0, 0.2); glVertex3d(-0.2, 0.1, 0);
    glNormal3d(0,0,-1);
    glTexCoord2d(1, 0.4); glVertex3d(+0.3, 0.4, 0);
    glTexCoord2d(0, 0.4); glVertex3d(-0.3, 0.4, 0);
    glNormal3d(0,.15,-1);
    glTexCoord2d(1, 0.6); glVertex3d(+0.2, 0.6, .15);
    glTexCoord2d(0, 0.6); glVertex3d(-0.2, 0.6, .15);
    glEnd();
    
    glBegin(GL_TRIANGLES);
    glTexCoord2d(1, 0.6); glVertex3d(+0.2, 0.6, .15);
    glTexCoord2d(0, 0.6); glVertex3d(-0.2, 0.6, .15);
    
    glNormal3d(0,.4,-1);
    glTexCoord2d(0.5, 1); glVertex3d(0, 0.8, .4);
    glEnd();
    glPopMatrix();
  }
  
  glPopAttrib();
  glPopMatrix();
}

#endif
