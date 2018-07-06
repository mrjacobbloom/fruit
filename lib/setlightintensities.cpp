#ifndef SET_LIGHT_INTENSITIES
#define SET_LIGHT_INTENSITIES
void setLightIntensities(float ambient, float diffuse, float specular, int shiny) {
  // Translate intensity to color vectors
  
  // cut them all in half now that we've got two light sources
  ambient /= 2;
  diffuse /= 2;
  specular /= 2;
  
  float Ambient[]   = {ambient, ambient, ambient, 1.0};
  float Diffuse[]   = {diffuse, diffuse, diffuse, 1.0};
  float Specular[]  = {specular, specular, specular, 1.0};

  // Set ambient, diffuse, specular components of light 0
  glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
  
  // Set ambient, diffuse, specular components of light 1
  glLightfv(GL_LIGHT1, GL_AMBIENT, Ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, Diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, Specular);
  
  // Set shininess of current material
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
}
#endif
