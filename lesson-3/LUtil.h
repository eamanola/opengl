#ifndef LUTIL_H
#define LUTIL_H

#include "../LOpenGL.h"

enum ViewPortMode
{
  VIEWPORT_MODE_FULL,
  VIEWPORT_MODE_HALF_CENTER,
  VIEWPORT_MODE_HALF_TOP,
  VIEWPORT_MODE_QUAD,
  VIEWPORT_MODE_RADAR
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;

bool initGL();
void render();
void update();
void handleKeys( unsigned char key, int x, int y );

#endif
