/*
 * Set projection
 */
#ifndef PROJECT
#define PROJECT

double zNear = 3.5 / 4;
double zFar = 4 * 3.5;
int fov = 55;            // Field of view (for perspective)
double aspect_ratio = 1; // Aspect ratio

static void Project() {
  // Tell OpenGL we want to manipulate the projection matrix
  glMatrixMode(GL_PROJECTION);
  // Undo previous transformations
  glLoadIdentity();
  // Perspective transformation
  gluPerspective(fov,aspect_ratio,zNear,zFar);
    
  // Switch to manipulating the model matrix
  glMatrixMode(GL_MODELVIEW);
  // Undo previous transformations
  glLoadIdentity();
}
#endif
