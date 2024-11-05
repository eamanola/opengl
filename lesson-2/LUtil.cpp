#include "LUtil.h"

int gColorMode = COLOR_MODE_CYAN;

GLfloat gProjectionScale = 1.0f;

bool initGL()
{
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

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

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glTranslatef(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f );

  if ( gColorMode == COLOR_MODE_CYAN )
  {
    glBegin( GL_QUADS );
      glColor3f( 0.0f, 1.0f, 1.0f );

      glVertex2f( -50.0f, -50.0f );
      glVertex2f(  50.0f, -50.0f );
      glVertex2f(  50.0f,  50.0f );
      glVertex2f( -50.0f,  50.0f );
    glEnd();
  } else {
    glBegin( GL_QUADS );
      glColor3f( 1.0f, 0.0f, 0.0f ); glVertex2f( -50.0f, -50.0f );
      glColor3f( 1.0f, 1.0f, 0.0f ); glVertex2f(  50.0f, -50.0f );
      glColor3f( 0.0f, 1.0f, 0.0f ); glVertex2f(  50.0f,  50.0f );
      glColor3f( 0.0f, 0.0f, 1.0f ); glVertex2f( -50.0f,  50.0f );
    glEnd();
  }

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{
  if( key == 'q' )
  {
    if ( gColorMode == COLOR_MODE_CYAN)
    {
      gColorMode = COLOR_MODE_MULTI;
    }
    else
    {
      gColorMode = COLOR_MODE_CYAN;
    }
  }
  else if( key == 'e')
  {
    if( gProjectionScale == 1.0f)
    {
      gProjectionScale = 2.0f;
    }
    else if( gProjectionScale == 2.0f)
    {
      gProjectionScale = 0.5f;
    }
    else if( gProjectionScale == 0.5f)
    {
      gProjectionScale = 1.0f;
    }

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, SCREEN_WIDTH * gProjectionScale, SCREEN_HEIGHT * gProjectionScale, 0.0, 1.0, -1.0 );
  }
}
