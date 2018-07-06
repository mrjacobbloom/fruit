/*
 * Final Project
 * 
 * A game in which the user creates a smoothie by dragging fruits into a blender.
 * 
 * Keybindings:
 *   ESC            exit.
 *   m              toggle between static and mobile camera mode
 *   arrow keys     move camera if in mobile camera mode
 * 
 * To run:
 *   make clean; make; ./final
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <cstring>
#include <iostream>
#include <math.h>
#include <vector>
// OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
// Ok now our stuff
#include "lib/loadtexbmp.cpp" // From Vlakkies examples
#include "lib/object.cpp" // From Vlakkies examples
#include "lib/errcheck.cpp" // From Vlakkies examples

#include "lib/project.cpp"
#include "lib/print.cpp" // Extended to do multiline strings and centering
#include "lib/trig.cpp" // Just the Sin() and Cos() macros
#include "lib/setlightintensities.cpp"

double OBJECTS_Z = 2; // The depth at which objects are centered on the shelf.
int fruits_rotation = 0; // applies to all pineapples

// 0 = normal, 1 = "god" -- v to toggle
int view_mode = 0;
int th = 0;      //  Azimuth of view angle -- arrow keys
int ph = 0;      //  Elevation of view angle

// track the win state
enum t_win_state {ws_null, ws_win, ws_lose};
t_win_state win_state = ws_null;

// for dealing with mouse input
int mousedown_x, mousedown_y;
double selected_fruit_old_x, selected_fruit_old_y;
int already_ran_mousemove_this_frame = 0;
// Represents what (if any) object is hovered over. One of the following:
// 0 = no hits
// the fruit_id of a fruit in visible_fruits
// BLENDER_NAME
unsigned int cursor_hit_name = 0;

// tracks how many fruits are in the smoothie, etc.
#include "Smoothie.cpp"
Smoothie smoothie;

// Models and stuff
#include "models/kitchen.cpp"
#include "models/blender.cpp"

#include "Fruit.cpp"
// visible fruits, fruits dropped into blender are tracked in smoothie
std::vector <Fruit> visible_fruits;
Fruit* selected_fruit = NULL;

/*
 * The actual content of the scene
 */
void scene() {
  ErrCheck("scene (start)");
  
  glCallList(DL_KITCHEN);
  
  for(unsigned int i = 0; i < visible_fruits.size(); i++) {
    visible_fruits[i].rot = fruits_rotation;
    if(cursor_hit_name == visible_fruits[i].fruit_id) {
      visible_fruits[i].hovered = 1;
    } else {
      visible_fruits[i].hovered = 0;
    }
    visible_fruits[i].render();
  }
  
  // Render blender last because parts of it are translucent
  render_blender();
  
  ErrCheck("scene (end)");
}

/*
 * Place the camera in predetermined location
 */
void placeCamera() {
  if(view_mode == 0) { // normal mode
    gluLookAt(
      0,5,10,
      0,0,0,
      0,1,0);
  } else { // god mode
    //  Eye position
    double dim = 4;
    double Ex = -2*dim*Sin(th)*Cos(ph);
    double Ey = +2*dim        *Sin(ph);
    double Ez = +2*dim*Cos(th)*Cos(ph);
    gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
  }
  
  ErrCheck("placeCamera");
}

/*
 * Place lights above the scene, one left of center and one irght of center
 */
void placeLight() {
  
  // default light intensities
  setLightIntensities(0.3, 0.5, 0, 0);
  
  float white[] = {1,1,1,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, white);
  
  // OpenGL should normalize normal vectors
  glEnable(GL_NORMALIZE);
  // glColor sets ambient and diffuse color materials
  glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  // Enable light 0 and set up position
  glEnable(GL_LIGHT0);
  float Position0[]  = {-3,7,2,1};
  glLightfv(GL_LIGHT0, GL_POSITION, Position0);
  
  // Enable light 0 and set up position
  glEnable(GL_LIGHT1);
  float Position1[]  = {3,7,2,1};
  glLightfv(GL_LIGHT1, GL_POSITION, Position1);
  
  
  ErrCheck("placeLight");
}

/*
 * OpenGL (GLUT) calls this routine to display the scene
 */
void display() {
  ErrCheck("display (start)");
  // Erase the window and the depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Undo previous transformations
  glLoadIdentity();
  
  // place light and camera
  placeCamera();
  placeLight();
  
  // turn on face culling
  glEnable(GL_CULL_FACE);
  
  ErrCheck("display (before drawing scene)");
  
  // Draw the scene itself
  scene();
  
  ErrCheck("display (after drawing scene)");
  
  smoothie.print();
  
  ErrCheck("display (after drawing smoothie)");
  
  if(win_state == ws_win) {
    PrintCenteredish("You win!");
  } else if(win_state == ws_lose) {
    PrintCenteredish("You lose!");
  }
  
  // Render the scene and make it visible
  glFlush();
  // only swap buffers if render mode is GL_RENDER
  int renderMode = 0;
  glGetIntegerv(GL_RENDER_MODE, &renderMode);
  if(renderMode == GL_RENDER) glutSwapBuffers();
  already_ran_mousemove_this_frame = 0;
  ErrCheck("display (end)");
}

/*
 * Things to do constantly
 */
void idle(int value) {
  fruits_rotation += 1;
  fruits_rotation %= 360;;
  
  for(unsigned int i = 0; i < visible_fruits.size(); i++) {
    visible_fruits[i].idle(); // do things like gravity and scaling
    // if the fruit's invisible, delete it
    // unless there's a selected fruit since that's a pointer to
    // one of the items in visible_fruits
    if(!selected_fruit && !visible_fruits[i].visible) {
      visible_fruits.erase(visible_fruits.begin() + i);
      // decrement i snce we're deleting from the array we're iterating over
      i--;
    }
  }
  
  if(is_blending) {
    blend_step += 0.01;
    if(blend_step > 1) {
      is_blending = 0;
      if(smoothie.goals_achieved) {
        win_state = ws_win;
      } else {
        win_state = ws_lose;
      }
    }
  }
  
  ErrCheck("idle");
  
  glutTimerFunc(50, idle, value);
  glutPostRedisplay(); // request redisplay
}

/*
 * GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y) {
  switch(key) {
    case GLUT_KEY_RIGHT:
      th += 5; break;
    case GLUT_KEY_LEFT:
      th -= 5; break;
    case GLUT_KEY_UP:
      ph += 5; break;
    case GLUT_KEY_DOWN:
      ph -= 5; break;
  }
  
  //  Keep angles to +/-360 degrees
  th %= 360;
  ph %= 360;
  
  //  Tell GLUT it is necessary to redisplay the scene
  glutPostRedisplay();
}


/*
 * GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y) {
  switch(ch) {
    case 27: // Exit on ESC
      exit(0);
      break;
    case 'v':
      view_mode = 1 - view_mode;
  }
}

/*
 * GLUT calls this routine when the window is resized
 */
void reshape(int width,int height) {
  // Ratio of the width to the height of the window
  aspect_ratio = (height>0) ? (double)width/height : 1;
  // Set the viewport to the entire window
  glViewport(0,0, width,height);
  // Set projection
  Project();
}

/*
 * Detect what objects (fruits or blender) are under the mouse
 * Called by GLUT when the mouse moves and the mouse is not down
 */
void mousemove(int _x, int _y) {
  // an attempt to keep it from locking up
  if(!already_ran_mousemove_this_frame) {
    double x = (double) _x;
    double y = (double) _y;
    
    // https://www.opengl.org/discussion_boards/showthread.php/167829-Mouse-picking-%28GL_SELECT%29-not-registering
    // http://www.unknownroad.com/rtfm/graphics/glselection.html
    int * viewport = new int[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    // basically Project() with extra bits so we're only looking near the cursor
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPickMatrix(x,(viewport[3] - y), 3.0,3.0, viewport);
    gluPerspective(fov,aspect_ratio,zNear,zFar);
    glMatrixMode(GL_MODELVIEW);
    
    unsigned int select_buffer[60];
    glSelectBuffer(60, select_buffer);
    glRenderMode(GL_SELECT);
    glInitNames();
    display();
    unsigned int hit_count = glRenderMode(GL_RENDER);
    Project(); // fix perspective cuz we broke the perspective
    
    // values are returned in a stupid format, unpack them into a sane format
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
}

/*
 * Move selected fruit while the mouse is down
 */
void drag(int mouse_x, int mouse_y) {
  // I was about to follow this tutorial to generate a pick vector
  // https://www.opengl.org/archives/resources/faq/technical/selection.htm
  // then I was going to find the intersection of that vector and a z-plane...
  // but then this approximation worked so well I decided not to
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
      if(cursor_hit_name == BLENDER_NAME) { // the clicked item was blender
        if(smoothie.contains_anything) {
          is_blending = 1;
          blend_step = 0;
        }
      } else { // the clicked item was a fruit
        mousedown_x = mouse_x;
        mousedown_y = mouse_y;
        // Locate the fruit with this name
        for(unsigned int i = 0; i < visible_fruits.size(); i++) {
          if(visible_fruits[i].fruit_id == cursor_hit_name) {
            selected_fruit = &visible_fruits[i];
            break;
          }
        }
        // if the fruit is one of the original non-draggables, clone it
        if(!selected_fruit->draggable) {
          selected_fruit->hovered = 0;
          visible_fruits.push_back(selected_fruit->clone());
          selected_fruit = &visible_fruits.back();
        }
        selected_fruit_old_x = selected_fruit->x;
        selected_fruit_old_y = selected_fruit->y;
        selected_fruit->y_velocity = 0;
        selected_fruit->selected = 1;
      }
    } else if(state == GLUT_UP && selected_fruit) { // Mouse was released
      selected_fruit->y_velocity = -0.07; // Start gravity
      selected_fruit->selected = 0;
      selected_fruit = NULL;
      // If the fruit is fenced in and not touching the mouse, the fruit should
      // become unhighlighted. Run mousemove again to see if this is the case.
      mousemove(mouse_x, mouse_y);
    }
  }
  ErrCheck("mousebutton");
}

/*
 * Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[]) {
  // Initialize GLUT
  glutInit(&argc,argv);
  // Request double buffered, true color window with Z buffering at 900x600
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(900, 600);
  glutCreateWindow("Final Project: Jacob Bloom");
  
  // Enable Z-buffering in OpenGL
  glEnable(GL_DEPTH_TEST);
  
  // Load textures and compile display lists
  init_kitchen();
  init_blender();
  init_fruit(); // compiles display lists for various fruit types
  
  // Generate a smoothie (includes random goal numbers for each fruit)
  smoothie = Smoothie();
  
  // Generate cloneable/onn-draggable fruits
  visible_fruits.push_back(Fruit());
  visible_fruits[0].x = -4;
  visible_fruits[0].type = t_pineapple;
  visible_fruits.push_back(Fruit());
  visible_fruits[1].x = -2;
  visible_fruits[1].type = t_grape;
  visible_fruits.push_back(Fruit());
  visible_fruits[2].x = -0;
  visible_fruits[2].type = t_orange;
  visible_fruits.push_back(Fruit());
  visible_fruits[3].x = 2;
  visible_fruits[3].type = t_banana;
  
  // Set callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key); // letter keys and esc
  glutSpecialFunc(special); // arrow keys
  glutPassiveMotionFunc(mousemove); // Mouse moved and mouse button up
  glutMotionFunc(drag); // Mouse moved and mouse button down
  glutMouseFunc(mousebutton); // Mouse button pressed or released
  
  // Start our timed loops
  idle(0);
  
  ErrCheck("init");
  
  // Pass control to GLUT so it can interact with the user
  glutMainLoop();
  return 0;
}
