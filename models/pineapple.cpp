#ifndef MODELS_PINEAPPLE
#define MODELS_PINEAPPLE

#include "sphere.cpp"

unsigned int DL_PINEAPPLE;

void compile_pineapple(unsigned int PINEAPPLE_TEXTURE);

void init_pineapple() {
  unsigned int PINEAPPLE_TEXTURE = LoadTexBMP("textures/pineapple.bmp");
  
  DL_PINEAPPLE = glGenLists(1);
  glNewList(DL_PINEAPPLE, GL_COMPILE);
  compile_pineapple(PINEAPPLE_TEXTURE);
  glEndList();
}
/*
 * Compile the pineapple display list (called once on startup)
 */
void compile_pineapple(unsigned int PINEAPPLE_TEXTURE) {
  glPushMatrix();
  glTranslated(0,1.3,0); // centered on x & z but y is bottom
  glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);
  
  // Enable lighting
  glEnable(GL_LIGHTING);
  
  // texture settings
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
  // Draw the whole pineapple on a unit sphere and stretch it after
  glScaled(1,1.5,1);
  
  glColor3f(1,1,1); // white
  glBindTexture(GL_TEXTURE_2D, PINEAPPLE_TEXTURE);
  glEnable(GL_TEXTURE_2D);
  sphere(0,0,0,1, 8); // draw unit sphere
  
  // draw the leaves
  glBindTexture(GL_TEXTURE_2D, LEAF_TEXTURE);
  glDisable(GL_CULL_FACE); // they're 2d so don't cull the back face
  // set up 2d lighting
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  double phi = 137.5;
  for(int i = 0; i < 25; i++) {
    glPushMatrix();
    glTranslated(0, i * 0.05 - 0.2, 0);
    glRotated(i * phi, 0, 1, 0);
    
    glColor3f(1,1,1); // white
    glBegin(GL_QUAD_STRIP);
    glNormal3d(0,0,1);
    glTexCoord2d(0, 0.0); glVertex3d(-0.1, 0.2, 0.2);
    glTexCoord2d(1, 0.0); glVertex3d(+0.1, 0.2, 0.2);
    glNormal3d(0,-.2,1);
    glTexCoord2d(0, 0.2); glVertex3d(-0.1, 1.1, 0.25);
    glTexCoord2d(1, 0.2); glVertex3d(+0.1, 1.1, 0.25);
    glNormal3d(0,-.4,1);
    glTexCoord2d(0, 0.4); glVertex3d(-0.1, 1.3, 0.3);
    glTexCoord2d(1, 0.4); glVertex3d(+0.1, 1.3, 0.3);
    glNormal3d(0,-.6,1);
    glTexCoord2d(0, 0.6); glVertex3d(-0.07, 1.5, 0.5);
    glTexCoord2d(1, 0.6); glVertex3d(+0.07, 1.5, 0.5);
    glNormal3d(0,-.8,1);
    glTexCoord2d(0, 0.8); glVertex3d(-0.05, 1.7, 0.7);
    glTexCoord2d(1, 0.8); glVertex3d(+0.05, 1.7, 0.7);
    glEnd();
    glBegin(GL_TRIANGLES); // the tip bit
    glTexCoord2d(0, 0.8); glVertex3d(-0.05, 1.7, 0.7);
    glTexCoord2d(1, 0.8); glVertex3d(+0.05, 1.7, 0.7);
    glNormal3d(0,-1,1);
    glTexCoord2d(0.5, 1); glVertex3d(0, 1.8, 1);
    glEnd();
    glPopMatrix();
  }
  glPopAttrib();
  glPopMatrix();
}

#endif
