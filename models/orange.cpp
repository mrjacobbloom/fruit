#ifndef MODELS_ORANGE
#define MODELS_ORANGE

#include "sphere.cpp"

unsigned int DL_ORANGE;

void compile_orange(unsigned int ORANGE_TEXTURE);

void init_orange() {
  unsigned int ORANGE_TEXTURE = LoadTexBMP("textures/orange.bmp");
  
  DL_ORANGE = glGenLists(1);
  glNewList(DL_ORANGE, GL_COMPILE);
  compile_orange(ORANGE_TEXTURE);
  glEndList();
}

/*
 * Compile the orange display list (called once on startup)
 */
void compile_orange(unsigned int ORANGE_TEXTURE) {
  glPushMatrix();
  glTranslated(0,0.5,0); // centered on x & z but y is bottom
  glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);
  
  // Enable lighting
  glEnable(GL_LIGHTING);
  setLightIntensities(0.4, 0.4, 0.3, 40);
  
  // texture settings
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, ORANGE_TEXTURE);
  
  glColor3f(1,1,1); // white
  sphere(0,0,0, 0.6, 1);
  
  // leaves
  glTranslated(0, 0.7, 0);
  glScaled(0.3, 0.3, 0.3);
  glRotated(90, 1, 0, 0);
  
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
    glNormal3f(0,-0.3,-1);
    glTexCoord2d(1, 0.0); glVertex3f(+0.1, 0, 0.3);
    glTexCoord2d(0, 0.0); glVertex3f(-0.1, 0, 0.3);
    glNormal3f(0,0,-1);
    glTexCoord2d(1, 0.2); glVertex3f(+0.2, 0.1, 0);
    glTexCoord2d(0, 0.2); glVertex3f(-0.2, 0.1, 0);
    glNormal3f(0,0,-1);
    glTexCoord2d(1, 0.4); glVertex3f(+0.3, 0.4, 0);
    glTexCoord2d(0, 0.4); glVertex3f(-0.3, 0.4, 0);
    glNormal3f(0,.15,-1);
    glTexCoord2d(1, 0.6); glVertex3f(+0.2, 0.6, .15);
    glTexCoord2d(0, 0.6); glVertex3f(-0.2, 0.6, .15);
    glEnd();
    
    glBegin(GL_TRIANGLES);
    glTexCoord2d(1, 0.6); glVertex3f(+0.2, 0.6, .15);
    glTexCoord2d(0, 0.6); glVertex3f(-0.2, 0.6, .15);
    
    glNormal3f(0,.4,-1);
    glTexCoord2d(0.5, 1); glVertex3f(0, 0.8, .4);
    glEnd();
    glPopMatrix();
  }
  
  glPopAttrib();
  glPopMatrix();
}

#endif
