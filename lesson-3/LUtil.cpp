#include "LUtil.h"

int gViewPortMode = VIEWPORT_MODE_FULL;

bool initGL()
{
  glViewport( 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

  GLenum error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    printf( "Error initializing OpenGL! %s", gluErrorString( error ) );
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

  glTranslatef( SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f );

  if( gViewPortMode == VIEWPORT_MODE_FULL )
  {
    glViewport( 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT );

    glBegin( GL_QUADS );
      glColor3f( 1.0f, 0.0f, 0.0f );
      glVertex2f( -SCREEN_WIDTH / 2.0f, -SCREEN_HEIGHT / 2.0f );
      glVertex2f(  SCREEN_WIDTH / 2.0f, -SCREEN_HEIGHT / 2.0f );
      glVertex2f(  SCREEN_WIDTH / 2.0f,  SCREEN_HEIGHT / 2.0f );
      glVertex2f( -SCREEN_WIDTH / 2.0f,  SCREEN_HEIGHT / 2.0f );
    glEnd();
  }
  else if( gViewPortMode == VIEWPORT_MODE_HALF_CENTER )
  {
    glViewport( SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 4.0f, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f );

    glBegin( GL_QUADS );
      glColor3f( 0.0f, 1.0f, 0.0f );
      glVertex2f( -SCREEN_WIDTH / 2.0f, -SCREEN_HEIGHT / 2.0f );
      glVertex2f(  SCREEN_WIDTH / 2.0f, -SCREEN_HEIGHT / 2.0f );
      glVertex2f(  SCREEN_WIDTH / 2.0f,  SCREEN_HEIGHT / 2.0f );
      glVertex2f( -SCREEN_WIDTH / 2.0f,  SCREEN_HEIGHT / 2.0f );
    glEnd();
  }
  else if( gViewPortMode == VIEWPORT_MODE_HALF_TOP )
  {
    glViewport( SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 2.0f, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f );

    glBegin( GL_QUADS );
      glColor3f( 0.0f, 0.0f, 1.0f );
      glVertex2f( -SCREEN_WIDTH / 2.0f, -SCREEN_HEIGHT / 2.0f );
      glVertex2f(  SCREEN_WIDTH / 2.0f, -SCREEN_HEIGHT / 2.0f );
      glVertex2f(  SCREEN_WIDTH / 2.0f,  SCREEN_HEIGHT / 2.0f );
      glVertex2f( -SCREEN_WIDTH / 2.0f,  SCREEN_HEIGHT / 2.0f );
    glEnd();
  }
  else if( gViewPortMode == VIEWPORT_MODE_QUAD )
  {
    glViewport( 0.0f, 0.0f, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f );
    glBegin( GL_QUADS );
      glColor3f( 1.0f, 0.0f, 0.0f );
      glVertex2f( -SCREEN_WIDTH / 4.0f, -SCREEN_HEIGHT / 4.0f );
      glVertex2f(  SCREEN_WIDTH / 4.0f, -SCREEN_HEIGHT / 4.0f );
      glVertex2f(  SCREEN_WIDTH / 4.0f,  SCREEN_HEIGHT / 4.0f );
      glVertex2f( -SCREEN_WIDTH / 4.0f,  SCREEN_HEIGHT / 4.0f );
    glEnd();

    glViewport( SCREEN_WIDTH / 2.0f, 0.0f, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f );
    glBegin( GL_QUADS );
      glColor3f( 0.0f, 1.0f, 0.0f );
      glVertex2f( -SCREEN_WIDTH / 4.0f, -SCREEN_HEIGHT / 4.0f );
      glVertex2f(  SCREEN_WIDTH / 4.0f, -SCREEN_HEIGHT / 4.0f );
      glVertex2f(  SCREEN_WIDTH / 4.0f,  SCREEN_HEIGHT / 4.0f );
      glVertex2f( -SCREEN_WIDTH / 4.0f,  SCREEN_HEIGHT / 4.0f );
    glEnd();

    glViewport( 0.0f, SCREEN_HEIGHT / 2.0f, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f );
    glBegin( GL_QUADS );
      glColor3f( 0.0f, 0.0f, 1.0f );
      glVertex2f( -SCREEN_WIDTH / 4.0f, -SCREEN_HEIGHT / 4.0f );
      glVertex2f(  SCREEN_WIDTH / 4.0f, -SCREEN_HEIGHT / 4.0f );
      glVertex2f(  SCREEN_WIDTH / 4.0f,  SCREEN_HEIGHT / 4.0f );
      glVertex2f( -SCREEN_WIDTH / 4.0f,  SCREEN_HEIGHT / 4.0f );
    glEnd();

    glViewport( SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f );
    glBegin( GL_QUADS );
      glColor3f( 1.0f, 1.0f, 0.0f );
      glVertex2f( -SCREEN_WIDTH / 4.0f, -SCREEN_HEIGHT / 4.0f );
      glVertex2f(  SCREEN_WIDTH / 4.0f, -SCREEN_HEIGHT / 4.0f );
      glVertex2f(  SCREEN_WIDTH / 4.0f,  SCREEN_HEIGHT / 4.0f );
      glVertex2f( -SCREEN_WIDTH / 4.0f,  SCREEN_HEIGHT / 4.0f );
    glEnd();
  }
  else if( gViewPortMode == VIEWPORT_MODE_RADAR )
  {
    glViewport( 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT );
    glBegin( GL_QUADS );
      glColor3f( 1.0f, 0.0f, 0.0f );
      glVertex2f( -SCREEN_WIDTH / 8.0f, -SCREEN_HEIGHT / 8.0f );
      glVertex2f(  SCREEN_WIDTH / 8.0f, -SCREEN_HEIGHT / 8.0f );
      glVertex2f(  SCREEN_WIDTH / 8.0f,  SCREEN_HEIGHT / 8.0f );
      glVertex2f( -SCREEN_WIDTH / 8.0f,  SCREEN_HEIGHT / 8.0f );

      glColor3f( 0.0f, 0.0f, 0.0f );
      glVertex2f( -SCREEN_WIDTH / 16.0f, -SCREEN_HEIGHT / 16.0f );
      glVertex2f(  SCREEN_WIDTH / 16.0f, -SCREEN_HEIGHT / 16.0f );
      glVertex2f(  SCREEN_WIDTH / 16.0f,  SCREEN_HEIGHT / 16.0f );
      glVertex2f( -SCREEN_WIDTH / 16.0f,  SCREEN_HEIGHT / 16.0f );
    glEnd();

    glViewport( SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f );
    glBegin( GL_QUADS );
      glColor3f( 1.0f, 1.0f, 1.0f );
      glVertex2f( -SCREEN_WIDTH / 8.0f, -SCREEN_HEIGHT / 8.0f );
      glVertex2f(  SCREEN_WIDTH / 8.0f, -SCREEN_HEIGHT / 8.0f );
      glVertex2f(  SCREEN_WIDTH / 8.0f,  SCREEN_HEIGHT / 8.0f );
      glVertex2f( -SCREEN_WIDTH / 8.0f,  SCREEN_HEIGHT / 8.0f );

      glColor3f( 0.0f, 0.0f, 0.0f );
      glVertex2f( -SCREEN_WIDTH / 16.0f, -SCREEN_HEIGHT / 16.0f );
      glVertex2f(  SCREEN_WIDTH / 16.0f, -SCREEN_HEIGHT / 16.0f );
      glVertex2f(  SCREEN_WIDTH / 16.0f,  SCREEN_HEIGHT / 16.0f );
      glVertex2f( -SCREEN_WIDTH / 16.0f,  SCREEN_HEIGHT / 16.0f );
    glEnd();
  }

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{
  if ( key == 'q' )
  {
    gViewPortMode++;
    if (gViewPortMode > VIEWPORT_MODE_RADAR)
    {
      gViewPortMode = VIEWPORT_MODE_FULL;
    }
  }
}
