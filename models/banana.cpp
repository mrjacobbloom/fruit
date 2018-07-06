#ifndef MODELS_BANANA
#define MODELS_BANANA

unsigned int DL_BANANA;

void compile_banana(unsigned int banana_obj);

void init_banana() {
  DL_BANANA = glGenLists(1);
  //LoadMaterial("models/banana2.mtl");
  unsigned int banana_obj = LoadOBJ("models/banana2.obj");
  glNewList(DL_BANANA, GL_COMPILE);
  compile_banana(banana_obj);
  glEndList();
}

/*
 * Compile the banana display list (called once on startup)
 */
void compile_banana(unsigned int banana_obj) {
  glPushMatrix();
  glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);
  
  // Enable lighting
  glEnable(GL_LIGHTING);
  //setLightIntensities(0.3, 0.5, 0.1, 70);
  
  glColor3f(1,1,1); // white
  
  glTranslated(0,1,0);
  
  glCallList(banana_obj);
  
  glPopAttrib();
  glPopMatrix();
}

#endif
