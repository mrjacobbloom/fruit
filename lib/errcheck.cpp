/*
 *  Function to print any errors encountered
 */
#ifndef ERR_CHECK
#define ERR_CHECK
void ErrCheck(const char* where) {
  GLenum err;
  while((err = glGetError()) != GL_NO_ERROR) {
    fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err), where);
  }
}
#endif
