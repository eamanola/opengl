#ifndef LUTIL_H
#define LUTIL_H

#include "../LOpenGL.h"

enum AliasMode
{
  ALIAS_MODE_ALIASED,
  ALIAS_MODE_ANTIALIASED,
  ALIAS_MODE_MULTISAMPLE
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;

bool initGL();
bool loadMedia();
void update();
void render();
void handleKeys( unsigned char key, int x, int y );
void handleMouseMotion( int x, int y );

#endif
