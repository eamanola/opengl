#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LFont.h"

LFont gTTF;

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

  if( !LFont::initFreeType() )
  {
    printf( "Unable to initialize FreeType!\n" );
  }

  return true;
}

bool loadMedia()
{
  if( !gTTF.loadFreeType( "../assets/lazy.ttf", 60 ) )
  {
    printf( "Unable to load ttf font!\n" );
    return false;
  }

  return true;
}

void update()
{

}

void render()
{
  glClear( GL_COLOR_BUFFER_BIT );
  glLoadIdentity();

  glColor3f( 0.f, 0.f, 1.f );
  gTTF.renderText( 0.f, 0.f, "Now rendering with TTF!" );

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{

}
