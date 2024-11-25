#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

LTexture gStretchedTexture;

LFRect gStrechRect = { 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT };

GLenum gFiltering = GL_LINEAR;

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
  if ( !gStretchedTexture.loadTextureFromFile( "./mini_opengl.png" ) )
  {
    printf( "Unable to load mini texture!\n" );
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

  gStretchedTexture.render( 0.f, 0.f, NULL, &gStrechRect );

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{
  if( key == 'q' )
  {
    glBindTexture( GL_TEXTURE_2D, gStretchedTexture.getTextureID() );

    if( gFiltering != GL_LINEAR )
    {
      gFiltering = GL_LINEAR;
    }
    else
    {
      gFiltering = GL_NEAREST;
    }

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gFiltering );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gFiltering );

    glBindTexture( GL_TEXTURE_2D, 0 );
  }
}
