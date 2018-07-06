#ifndef MODELS_BLENDER
#define MODELS_BLENDER

#include "sphere.cpp"

unsigned int DL_BLENDER, SMOOTHIE_TEXTURE, BLENDER_NAME = 300;
double BLENDER_X = 5; // this'll change
double blend_step = 0; // blend animation 0-1
unsigned int is_blending = 0; // boolean

void compile_blender();
void blend();

void init_blender() {
  SMOOTHIE_TEXTURE = LoadTexBMP("textures/smoothie.bmp");
  
  DL_BLENDER = glGenLists(1);
  glNewList(DL_BLENDER, GL_COMPILE);
  compile_blender();
  glEndList();
}

void render_blender() {
  glPushName(BLENDER_NAME);
  glPushAttrib(GL_LIGHTING_BIT);
  if(cursor_hit_name == BLENDER_NAME) { // If hovered, highlight
    float emission = 0.1;
    float Emission[] = {emission, emission, emission, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, Emission);
  }
  if(is_blending) blend();
  glCallList(DL_BLENDER);
  glPopAttrib();
  glPopName();
}

void compile_blender() {
  glPushMatrix();
  glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);
  
  glTranslated(BLENDER_X, 0, 0);
  
  // lighting settings
  glEnable(GL_LIGHTING);
  setLightIntensities(0.3, 0.5, 0.5, 120);
  
  // texture settings -- off for now since the base is untextured
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glDisable(GL_TEXTURE_2D);
  
  // turn on polygon offset since the black hole goes above the base
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1,1);
  
  glColor3f(1,1,1); // white
  // -- base -- front
  glBegin(GL_QUAD_STRIP);
  glNormal3d(0,0,1);
  glVertex3d(-.98, 0, 3.4);
  glVertex3d(+.98, 0, 3.4);
  glVertex3d(-1, 0.1, 3.5);
  glVertex3d(+1, 0.1, 3.5);
  glVertex3d(-1, 0.4, 3.5);
  glVertex3d(+1, 0.4, 3.5);
  glNormal3d(0,0.2,1);
  glVertex3d(-1, 0.5, 3.45);
  glVertex3d(+1, 0.5, 3.45);
  glNormal3d(0,0.5,1);
  glVertex3d(-1, 0.6, 3.4);
  glVertex3d(+1, 0.6, 3.4);
  glNormal3d(0,0.7,1);
  glVertex3d(-1, 0.7, 3.35);
  glVertex3d(+1, 0.7, 3.35);
  glNormal3d(0,1,0.9);
  glVertex3d(-.98, 0.8, 3.25);
  glVertex3d(+.98, 0.8, 3.25);
  glNormal3d(0,1,0.7);
  glVertex3d(-.97, 0.9, 3.1);
  glVertex3d(+.97, 0.9, 3.1);
  glNormal3d(0,1,0.5);
  glVertex3d(-.95, 1, 2.95);
  glVertex3d(+.95, 1, 2.95);
  glNormal3d(-.1,1,0.2);
  glVertex3d(-.9, 1, 2.1);
  glNormal3d(+.1,1,0.2);
  glVertex3d(+.9, 1, 2.1);
  glNormal3d(-.1,1,0);
  glVertex3d(-.8, 1, 1.1);
  glNormal3d(+.1,1,0);
  glVertex3d(+.8, 1, 1.1);
  glNormal3d(-.1,1,0);
  glVertex3d(-.6, 1, 1.0);
  glNormal3d(+.1,1,0);
  glVertex3d(+.6, 1, 1.0);
  glEnd();
  
  
  // -- left side
  
  glBegin(GL_TRIANGLES);
  glNormal3d(-1,0,1);
  glVertex3d(-.98, 0, 3.4);
  glNormal3d(-1,0,0);
  glVertex3d(-1, 0, 3.5);
  glVertex3d(-1, 0.1, 3.5);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glVertex3d(-1, 0, 3.5);
  glVertex3d(-1, 0.1, 3.5);
  glVertex3d(-1, 0, 3.5);
  glVertex3d(-1, 0.4, 3.5);
  glVertex3d(-1, 0, 3.45);
  glVertex3d(-1, 0.5, 3.45);
  glVertex3d(-1, 0, 3.4);
  glVertex3d(-1, 0.6, 3.4);
  glVertex3d(-1, 0, 3.35);
  glVertex3d(-1, 0.7, 3.35);
  glNormal3d(-1,0,-.1);
  glVertex3d(-.98, 0, 3.25);
  glVertex3d(-.98, 0.8, 3.25);
  glNormal3d(-1,0,-.2);
  glVertex3d(-.97, 0, 3.1);
  glVertex3d(-.97, 0.9, 3.1);
  glNormal3d(-1,0,-.3);
  glVertex3d(-.95, 0, 2.95);
  glVertex3d(-.95, 1, 2.95);
  glNormal3d(-1,0,-.6);
  glVertex3d(-.9, 0, 2.1);
  glVertex3d(-.9, 1, 2.1);
  glNormal3d(-1,0,-.9);
  glVertex3d(-.8, 0, 1.1);
  glVertex3d(-.8, 1, 1.1);
  glNormal3d(-1,0,-1);
  glVertex3d(-.6, 0, 1.0);
  glVertex3d(-.6, 1, 1.0);
  glEnd();
  
  glPolygonOffset(0,0);
  glDisable(GL_POLYGON_OFFSET_FILL);
  
  // -- neck that goes around the glass --
  glDisable(GL_CULL_FACE);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glBegin(GL_QUAD_STRIP);
  for(int i = 0; i <= 50; i++) {
    double th = i * (360.0 / 50.0);
    double x =       0.5 * Sin(th);
    double z = 2.1 + 0.5 * Cos(th);
    glNormal3d(x,0,z);
    glVertex3d(x, 1.3, z);
    glVertex3d(x, 1, z);
  }
  glEnd();
  glEnable(GL_CULL_FACE);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
  
  // -- buttons --
  for(double buttonx = -.8; buttonx < 0; buttonx += 0.3) {
    glColor3f(1, 0, 0);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(0,0,1);
    glVertex3d(buttonx, 0.5, 3.7);
    for(double angle = 0; angle <= 360; angle += 36) {
      double x = buttonx + 0.05 * Cos(angle);
      double y = 0.5 + (0.08 * Sin(angle));
      glNormal3d(0.5*Cos(angle),0.5*Sin(angle),1);
      glVertex3d(x, y, 3.7);
    }
    glEnd();
  }
  
  // -- black hole ~~~~~~~
  glColor3f(0,0,0);
  setLightIntensities(0, 0, 0, 0);
  glNormal3d(0,1,0);
  glBegin(GL_TRIANGLE_FAN);
  glVertex3d(0, 1, 2.1);
  for(int i = 0; i <= 50; i++) {
    double th = i * (360.0 / 50.0);
    double x =       0.5 * Sin(th);
    double z = 2.1 + 0.5 * Cos(th);
    glVertex3d(x, 1, z);
  }
  glEnd();
  
  // -- the glass :o --
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(1,1,1, 0.4);
  setLightIntensities(0.3, 0.5, 1, 127);
  
  glDisable(GL_CULL_FACE);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  // it can just be a hemisphere
  glPushMatrix();
  glTranslated(0,3.45,2.1);
  glScaled(1,2.5,1);
  for(int sph = -90; sph < 0; sph += 5) {
    glBegin(GL_QUAD_STRIP);
    for (int sth = 0; sth <= 360; sth += 5) {
      PolarVertex(sth, sph, 1);
      PolarVertex(sth, sph+5, 1);
    }
    glEnd();
  }
  glPopMatrix();
  
  glPopAttrib();
  glPopMatrix();
}

/*
 * Draw the smoothie blending in the blender based on blend_step
 */
void blend() {
  glPushMatrix();
  glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);
  
  
  // lighting settings
  glEnable(GL_LIGHTING);
  setLightIntensities(0.5, 0.4, 0,0);
  
  glColor3dv(smoothie.color);
  
  // make sure it renders behind the glass
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1,1);
  
  // texture settings
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, SMOOTHIE_TEXTURE);
  
  int max_ph = -20;
  
  if(blend_step < 0.2) {
    max_ph = -90 + (blend_step/0.2)*70;
  } else if(blend_step > 0.8) {
    max_ph = -90 + ((1 - blend_step)/0.2)*70;
  }
  
  glTranslated(BLENDER_X + 0,3.45,2.1);
  glRotated(2 * 360 * blend_step, 0,1,0);
  glScaled(1,2.5,1);
  for(int sph = -90; sph < max_ph; sph += 2) {
    // this calculation is wrong but looks cooler than what I meant to do
    double texture_scale = 2 * blend_step;
    glBegin(GL_QUAD_STRIP);
    for (int sth = 0; sth <= 360; sth += 5) {
      PolarVertex(sth, sph, texture_scale);
      PolarVertex(sth, sph+2, texture_scale);
    }
    glEnd();
  }
  // draw top of smoothie
  glNormal3d(0,1,0);
  glBegin(GL_TRIANGLE_FAN);
  glVertex3d(0,Sin(max_ph),0);
  for (int sth = 0; sth >= -360; sth -= 5) {
    PolarVertex(sth, max_ph, 1);
  }
  glEnd();
  
  glDisable(GL_POLYGON_OFFSET_FILL);
  glPopAttrib();
  glPopMatrix();
}

#endif
