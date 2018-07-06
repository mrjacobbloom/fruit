/*
 * Convenience routine to output raster text
 * Use VARARGS to make this more flexible
 */
#ifndef PRINT
#define PRINT
#define LEN 8192  // Maximum length of text string
void Print(const char* format , ...) {
  char    buf[LEN];
  char*  ch=buf;
  va_list args;
  // Turn the parameters into a character string
  va_start(args,format);
  vsnprintf(buf,LEN,format,args);
  va_end(args);
  // Display the characters one at a time at the current raster position
  while (*ch) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
  }
}

/*
 * Print a multiline string, restarting at left after every newline
 */
void PrintMultiline(const char* format, ...) {
  // get window height
  int * viewport = new int[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  int window_height = viewport[3];
  
  int line_height = (5 + 18);
  
  char buf[LEN];
  va_list args;
  // Turn the parameters into a character string
  va_start(args,format);
  vsnprintf(buf,LEN,format,args);
  va_end(args);
  
  std::string formatted = buf;
  
  // split -- https://stackoverflow.com/a/14266139/1784306
  int pos = 0;
  std::string token;
  int done = 0;
  int line = 1;
  while (!done) {
    if((pos = formatted.find('\n')) != -1) {
      token = formatted.substr(0, pos);
    } else {
      token = formatted;
      done = 1;
    }
    glWindowPos2i(5,window_height - line*line_height);
    Print(token.c_str());
    formatted.erase(0, pos + 1);
    line++;
  }
}

/*
 * Print text centered(ish) on the back wall
 */
void PrintCenteredish(const char* text) {
  int * viewport = new int[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  glWindowPos2i(viewport[2]/2 - 40,viewport[3] * 0.7);
  Print(text);
}

#endif
