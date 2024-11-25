#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

LTexture gLoadedTexture;

bool initGL()
{
  glViewport( 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

  glEnable( GL_TEXTURE_2D );

  GLenum error = glGetError();
  if( error != GL_NO_ERROR )
  {
    printf( "Error initilizing OpenGL! %s\n", gluErrorString( error ) );
    return false;
  }

  ilInit();
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
  if ( !gLoadedTexture.loadTexttureFromFile( "../assets/texture.png" ) )
  {
    printf( "Unable to load file texture!\n" );
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

  GLfloat x = ( SCREEN_WIDTH - gLoadedTexture.textureWidth() ) / 2.0f;
  GLfloat y = ( SCREEN_HEIGHT - gLoadedTexture.textureHeight() ) / 2.0f;

  gLoadedTexture.render( x, y );

  glutSwapBuffers();
}
