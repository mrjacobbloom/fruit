// globals for dealing with mouse input
int mousedown_x, mousedown_y;
double selected_fruit_old_x, selected_fruit_old_y;
int already_ran_mousemove_this_frame = 0;
// Represents what (if any) object is hovered over
// (I used 0 as a sentinel value for no hits, but 0 can b e a valid object name)
unsigned int cursor_hit_name = 0;


void Fruit::render() {
  glPushName(fruit_id); // a positive integer of your choice uniquely identifying the object

  if(hovered || selected) { // If the fruit is hovered, highlight it with emission light
    float emission = 0.1;
    float Emission[] = {emission, emission, emission, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, Emission);
  }
  
  // ... render fruit ...
  
  glPopName(); // object end
}

/*
 * Detect what fruits are under the mouse
 * Called by GLUT when the mouse moves and the mouse is not down
 */
void mousemove(int _x, int _y) {
  // an attempt to keep it from locking up
  if(already_ran_mousemove_this_frame) return;
  
  double x = (double) _x;
  double y = (double) _y;
  
  // http://www.unknownroad.com/rtfm/graphics/glselection.html
  // https://www.opengl.org/discussion_boards/showthread.php/167829-Mouse-picking-%28GL_SELECT%29-not-registering
  
  int * viewport = new int[4];
  glGetIntegerv(GL_VIEWPORT, viewport); // gets data about window size
  
  // basically Project() with extra bits so we're only looking near the cursor
  // see https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPickMatrix.xml
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPickMatrix(x,(viewport[3] - y), 3.0,3.0, viewport); // <- this glu function generates a camera projection that is 3px by 3px around the mouse
  gluPerspective(fov,aspect_ratio,zNear,zFar);
  glMatrixMode(GL_MODELVIEW);
  
  unsigned int select_buffer[60]; // this is where hover data is returned into
  glSelectBuffer(60, select_buffer);
  glRenderMode(GL_SELECT);
  glInitNames();
  display();
  unsigned int hit_count = glRenderMode(GL_RENDER); // returns the number of hits
  Project(); // fix perspective cuz we broke the perspective
  
  // values are returned in a stupid format, unpack them into a sane format
  // see here for more explanation: http://www.unknownroad.com/rtfm/graphics/glselection.html
  cursor_hit_name = 0;
  unsigned int * select_buffer_ptr = select_buffer;
  for(unsigned int j = 0; j < hit_count; j++) {
    unsigned int name_count = *select_buffer_ptr;
    select_buffer_ptr += 3;
    while(name_count--) cursor_hit_name = *select_buffer_ptr++;
  }
  ErrCheck("mousemove");
  already_ran_mousemove_this_frame = 1;
  glutPostRedisplay();
}

/*
 * Move selected fruit
 * Called by GLUT if mouse is moved while the mouse button is down
 */
void drag(int mouse_x, int mouse_y) {
  // Here's a tutorial with the correct way to generate a pick vector
  // https://www.opengl.org/archives/resources/faq/technical/selection.htm
  if(!selected_fruit) return;
  // x position's scaling varies slightly based on (previous) y position
  double x_coef = 0.013 - 0.000008*(mouse_y - mousedown_y);
  selected_fruit->x = selected_fruit_old_x + x_coef*(mouse_x - mousedown_x);
  selected_fruit->y = selected_fruit_old_y - 0.013*(mouse_y - mousedown_y) + 1*(1-selected_fruit->scale);
  
  // fence the fruit's position within a reasonable range
  if(selected_fruit->y < 0) selected_fruit->y = 0;
  if(selected_fruit->y > 4) selected_fruit->y = 4;
  if(selected_fruit->x < -6) selected_fruit->x = -6;
  if(selected_fruit->x > 6) selected_fruit->x = 6;
  
  ErrCheck("drag");
}


/*
 * Called by GLUT when a mouse button is clicked or released
 */
void mousebutton(int button, int state, int mouse_x, int mouse_y) {
  if(button == GLUT_LEFT_BUTTON) {
    if(state == GLUT_DOWN && cursor_hit_name != 0) {
      mousedown_x = mouse_x;
      mousedown_y = mouse_y;
      // Locate the fruit with this name
      selected_fruit = fruits[cursor_hit_name]
      
      selected_fruit_old_x = selected_fruit->x;
      selected_fruit_old_y = selected_fruit->y;
      selected_fruit->selected = 1;
    } else if(state == GLUT_UP && selected_fruit) { // Mouse was released
      selected_fruit->selected = 0;
      selected_fruit = NULL;
      
      // If the fruit is fenced in and not touching the mouse, the fruit should
      // become unhighlighted. Run mousemove again to see if this is the case.
      mousemove(mouse_x, mouse_y);
    }
  }
  ErrCheck("mousebutton");
}

int main() {
  // Initialize GLUT
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(900, 600);
  glutCreateWindow("My awesome window");
  
  // set up callbacks
  glutPassiveMotionFunc(mousemove); // Mouse moved while mouse button up
  glutMotionFunc(drag); // Mouse moved while mouse button down
  glutMouseFunc(mousebutton); // Mouse button pressed or released
  
  glutMainLoop();
}
