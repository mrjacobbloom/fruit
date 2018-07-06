#ifndef FRUIT
#define FRUIT

/*
 * Fruit is a class that tracks the physical properties of a fruit that is
 * visible on the screen. Once it's in the blender, it's tracked in Smoothie.
 * By default, it's "not draggable" and it's cloned on drag. This clone
 * is draggable. This way, the original fruits never move.
 */

unsigned int LEAF_TEXTURE;

#include "models/pineapple.cpp"
#include "models/grape.cpp"
#include "models/orange.cpp"
#include "models/banana.cpp"

enum fruit_type {t_pineapple, t_grape, t_orange, t_banana};
unsigned int max_fruit_id = 1, BANANA_OBJ;

/*
 * Compiles display lists for each fruit type
 * Called on startup in main
 */
void init_fruit() {
  LEAF_TEXTURE = LoadTexBMP("textures/leaf.bmp");
  // each fruit type's file provides an init function which compiles its
  // display lists
  init_pineapple();
  init_grape();
  init_orange();
  init_banana();
}

class Fruit {
  public:
    Fruit();
    void render();
    void idle();
    Fruit clone();
    fruit_type type;
    unsigned int fruit_id;
    double x;
    double y;
    double y_velocity;
    double z;
    double rot;
    double scale;
    int draggable; // there are permament fruits that clone into draggable fruits
    int hovered; // because C didn't need booleans?
    int selected;
    int visible;
};

Fruit::Fruit() {
  type = t_pineapple;
  fruit_id = max_fruit_id++; // for tracking which fruit is under the mouse;
  x = 0;
  y = 0;
  y_velocity = 0;
  z = OBJECTS_Z;
  rot = 0;
  scale = 1;
  draggable = 0;
  hovered = 0;
  visible = 1; // if visible is 0, it'll be deleted the next time idle is called
  
}

/*
 * Create a clone of a fruit, but make it draggable. That way the original
 * non-draggable fruits never move.
 */
Fruit Fruit::clone() {
  Fruit f = Fruit();
  f.type = type;
  f.x = x;
  f.y = y;
  f.x = x;
  f.rot = rot;
  f.draggable = 1;
  f.selected = 1;
  f.hovered = hovered;
  return f;
}

void Fruit::render() {
  if(!visible) return;
  glPushName(fruit_id);
  glPushMatrix();
  glPushAttrib(GL_LIGHTING_BIT);
  if(hovered || selected) { // If the fruit is hovered, highlight it with emission light
    float emission = 0.1;
    float Emission[] = {emission, emission, emission, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, Emission);
  }
  glTranslated(x, y, z);
  glScaled(scale, scale, scale);
  glRotated(rot,0,1,0);
  switch(type) {
    case t_pineapple:
      glCallList(DL_PINEAPPLE);
      break;
    case t_grape:
      glCallList(DL_GRAPE);
      break;
    case t_orange:
      glCallList(DL_ORANGE);
      break;
    case t_banana:
      glCallList(DL_BANANA);
      break;
  }
  glPopAttrib();
  glPopMatrix();
  glPopName();
}

void Fruit::idle() {
  if(!visible) return;
  if(!selected && y_velocity) {
    y_velocity *= 1.15;
    y += y_velocity;
    if(y < 0) {
      y = 0; y_velocity = 0;
    }
  }
  
  scale = 1;
  double dx_blender = x - BLENDER_X;
  double dy_blender = y - 1;
  double sqr_d_blender = dx_blender*dx_blender + dy_blender*dy_blender;
  if(sqr_d_blender < 4) {
    scale = sqrt(sqr_d_blender)/2;
    if(!selected) { // only do motion and hiding if not dragging
      x += dx_blender * -0.5;
      if(y < 1 && fabs(x - BLENDER_X) < 0.1) {
        visible = 0; // tell main idle function to delete this
        // add to smoothie
        switch(type) {
          case t_pineapple:
            smoothie.increment("pineapples");
            break;
          case t_grape:
            smoothie.increment("grapes");
            break;
          case t_orange:
            smoothie.increment("oranges");
            break;
          case t_banana:
            smoothie.increment("bananas");
            break;
        }
      }
    }
  }
}

#endif
