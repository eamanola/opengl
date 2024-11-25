#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

LTexture gRotatingTexture;
GLfloat gAngle = 0.f;

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
  if ( !gRotatingTexture.loadTextureFromFile( "../assets/arrow.png" ) )
  {
    printf( "Unable to load arrow texture!\n" );
    return false;
  }

  return true;
}

void update()
{
  gAngle += 360.f / SCREEN_FPS;

  if( gAngle > 360.f )
  {
    gAngle -= 360.f;
  }
}

void render()
{
  glClear( GL_COLOR_BUFFER_BIT );

  gRotatingTexture.render(
    ( ( SCREEN_WIDTH - gRotatingTexture.imageWidth() ) / 2.f ),
    ( SCREEN_HEIGHT - gRotatingTexture.imageHeight() ) / 2.f,
    NULL, NULL, gAngle
  );

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{
}
