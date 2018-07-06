#ifndef MODELS_KITCHEN
#define MODELS_KITCHEN

#include "../lib/setlightintensities.cpp"

unsigned int DL_KITCHEN, GRANITE_TEXTURE, MARBLE_TEXTURE, CABINETS_TEXTURE;

void compile_kitchen();

void init_kitchen() {
  GRANITE_TEXTURE = LoadTexBMP("textures/granite.bmp");
  MARBLE_TEXTURE = LoadTexBMP("textures/granite_tile.bmp");
  CABINETS_TEXTURE = LoadTexBMP("textures/cabinets.bmp");
  
  DL_KITCHEN = glGenLists(1);
  glNewList(DL_KITCHEN, GL_COMPILE);
  compile_kitchen();
  glEndList();
}

void compile_kitchen() {
  glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);
  // Enable lighting
  glEnable(GL_LIGHTING);
  setLightIntensities(0.3, 0.3, 1, 120);
  
  // Enable textures
  glEnable(GL_TEXTURE_2D);
  glColor3f(1,1,1); // white
  
  double step = 0.5; // grid side length
  double scale = 0.2; // scale factor for texture
  double countertop_length = 4.5;
  for(double i = -10; i < 10; i += step) {
    // -- BACK WALL --
    glBindTexture(GL_TEXTURE_2D, MARBLE_TEXTURE);
    glBegin(GL_QUAD_STRIP);
    glNormal3d(0,0,1);
    for(double j = 7; j > -1; j -= step) {
      glTexCoord2d((i+step)*scale, j*scale); glVertex3f(i+step, j, 0);
      glTexCoord2d((i     )*scale, j*scale); glVertex3f(i     , j, 0);
    }
    glEnd();
    
    // -- MOULDING --
    glDisable(GL_TEXTURE_2D); // the moulding should just be white
    glBegin(GL_QUAD_STRIP);
    glNormal3d(0,1,0);
    glVertex3f(i+step, 0.6, 0);
    glVertex3f(i     , 0.6, 0);
    glNormal3d(0,1,1);
    glVertex3f(i+step, 0.6, 0.1);
    glVertex3f(i     , 0.6, 0.1);
    glNormal3d(0,0,1);
    glVertex3f(i+step, 0.55, 0.1);
    glVertex3f(i     , 0.55, 0.1);
    glNormal3d(0,1,1);
    glVertex3f(i+step, 0.5, 0.1);
    glVertex3f(i     , 0.5, 0.1);
    glNormal3d(0,0,1);
    glVertex3f(i+step, 0.45, 0.15);
    glVertex3f(i     , 0.45, 0.15);
    glNormal3d(0,-1,1);
    glVertex3f(i+step, 0.4, 0.1);
    glVertex3f(i     , 0.4, 0.1);
    glNormal3d(0,0,1);
    glVertex3f(i+step, 0.1, 0.1);
    glVertex3f(i     , 0.1, 0.1);
    glNormal3d(0,1,1);
    glVertex3f(i+step, 0.0, 0.15);
    glVertex3f(i     , 0.0, 0.15);
    
    glEnd();
    
    // -- COUNTERTOP --
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, GRANITE_TEXTURE);
    glBegin(GL_QUAD_STRIP);
    // top flat bit
    glNormal3d(0,1,0);
    for(double j = 0; j < countertop_length; j += step) {
      // texture repeats 8 times, squares are 2x2
      glTexCoord2d((i+step)*scale, j*scale); glVertex3f(i+step, 0, j);
      glTexCoord2d((i     )*scale, j*scale); glVertex3f(i     , 0, j);
    }
    // bevel
    for(double j = 0; j < 1; j += 0.1) {
      
      double y = -.25 + 0.25 * Cos(j * 180);
      double z = countertop_length + 0.25 * Sin(j * 180);
      double tex_y = (countertop_length + j) * scale;
      glNormal3d(0,y,z);
      // texture repeats 8 times, squares are 2x2
      glTexCoord2d((i+step)*scale, tex_y); glVertex3f(i+step, y, z);
      glTexCoord2d((i     )*scale, tex_y); glVertex3f(i     , y, z);
    }
    glEnd();
  }
  // -- CABINETS --
  setLightIntensities(0.6, 0.3, 0, 0);
  glNormal3d(0,0,1);
  glBindTexture(GL_TEXTURE_2D, CABINETS_TEXTURE);
  glBegin(GL_QUADS);
  glTexCoord2d(1, 1); glVertex3f(10, -.5, countertop_length);
  glTexCoord2d(0, 1); glVertex3f(-10, -.5, countertop_length);
  glTexCoord2d(0, 0); glVertex3f(-10, -5, countertop_length);
  glTexCoord2d(1, 0); glVertex3f(10, -5, countertop_length);
  glEnd();
  
  
  glPopAttrib();
}

#endif
