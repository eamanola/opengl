#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LPlainPolygonProgram2D.h"

LPlainPolygonProgram2D gProgram;

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

bool loadGP()
{
  if( !gProgram.loadProgram() )
  {
    printf( "Unable to load basic shader!\n" );
    return false;
  }

  gProgram.bind();

  return true;
}

bool loadMedia()
{
  return true;
}

void update()
{

}

void render()
{
  glClear( GL_COLOR_BUFFER_BIT );

  glLoadIdentity();

  glTranslatef( SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, 0.f );

  glBegin( GL_QUADS );
    glColor3f( 0.f, 1.f, 1.f );
    glVertex2f( -50.f, -50.f );
    glVertex2f(  50.f, -50.f );
    glVertex2f(  50.f,  50.f );
    glVertex2f( -50.f,  50.f );
  glEnd();

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{
}

void handleMouseMotion( int x, int y )
{
}