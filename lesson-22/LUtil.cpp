#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

LTexture gLeft;
LTexture gRight;
LTexture gCombined;

bool initGL()
{
  GLenum glewError = glewInit();
  if( glewError != GLEW_OK )
  {
    printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
    return false;
  }

  if( !GLEW_VERSION_2_1 )
  {
    printf( "OpenGL 2.1 not supported!\n" );
    return false;
  }

  glViewport( 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glClearColor( 0.f, 0.f, 0.f, 1.f );

  glEnable( GL_TEXTURE_2D );

  glEnable( GL_BLEND );
  glDisable( GL_DEPTH_TEST );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  GLenum error = glGetError();
  if( error != GL_NO_ERROR )
  {
    printf( "Error initilizing OpenGL! %s\n", gluErrorString( error ) );
    return false;
  }

  ilInit();
  iluInit();
  ilClearColour( 255, 255, 255, 000 );

  ILenum ilError = ilGetError();
  if( ilError != IL_NO_ERROR )
  {
    printf( "Error initilizaing DevIL! %s\n", iluErrorString( ilError ) );
    return false;
  }

  return true;
}

bool loadMedia()
{
  if( !gLeft.loadPixelsFromFile32( "../assets/left.png" ) )
  {
    printf( "Unable to load left texture!\n" );
    return false;
  }

  if( !gRight.loadPixelsFromFile32( "../assets/right.png" ) )
  {
    printf( "Unable to load right texture!\n" );
    return false;
  }

  gCombined.createPixels32( gLeft.imageWidth() + gRight.imageWidth(), gLeft.imageHeight() );

  gLeft.blitPixels32( 0, 0, gCombined );
  gRight.blitPixels32( gLeft.imageWidth(), 0, gCombined );

  gCombined.padPixels32();
  gCombined.loadTextureFromPixels32();

  gLeft.freeTexture();
  gRight.freeTexture();

  return true;
}

void update()
{

}

void render()
{
  glClear( GL_COLOR_BUFFER_BIT );
  glLoadIdentity();

  gCombined.render(
    ( SCREEN_WIDTH - gCombined.imageWidth() ) / 2.f,
    ( SCREEN_HEIGHT - gCombined.imageHeight() ) / 2.f
  );

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{

}
