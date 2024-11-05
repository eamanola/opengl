#include "LUtil.h"

bool initGL()
{
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

  GLenum error = glGetError();
  if( error != GL_NO_ERROR )
  {
    printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
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

  glBegin( GL_QUADS );
    glVertex2f( -0.5f, -0.5f );
    glVertex2f(  0.5f, -0.5f );
    glVertex2f(  0.5f,  0.5f );
    glVertex2f( -0.5f,  0.5f );
  glEnd();

  glutSwapBuffers();
}
