#include "LUtil.h"

GLfloat gCameraX = 0.0f;
GLfloat gCameraY = 0.0f;

bool initGL()
{
  glViewport( 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glPushMatrix();

  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

  GLenum error = glGetError();
  if( error != GL_NO_ERROR )
  {
    printf( "Error initilizing OpenGL! %s\n", gluErrorString( error ) );
    return false;
  }

  return true;
}

void update()
{

}

void drawQuad(GLfloat red, GLfloat green, GLfloat blue)
{
  glBegin( GL_QUADS );
    glColor3f(red, green, blue);

    glVertex2f( -SCREEN_WIDTH / 4.0f, -SCREEN_HEIGHT / 4.0f );
    glVertex2f(  SCREEN_WIDTH / 4.0f, -SCREEN_HEIGHT / 4.0f );
    glVertex2f(  SCREEN_WIDTH / 4.0f,  SCREEN_HEIGHT / 4.0f );
    glVertex2f( -SCREEN_WIDTH / 4.0f,  SCREEN_HEIGHT / 4.0f );
  glEnd();
}

void render()
{
  glClear( GL_COLOR_BUFFER_BIT );

  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();

  glPushMatrix();

  glTranslatef( SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f );
  drawQuad( 1.0f, 0.0f, 0.0f );

  glTranslatef( SCREEN_WIDTH, 0.0f, 0.0f );
  drawQuad( 0.0f, 1.0f, 0.0f );

  glTranslatef( 0.0f, SCREEN_HEIGHT, 0.0f );
  drawQuad( 0.0f, 0.0f, 1.0f );

  glTranslatef( -SCREEN_WIDTH, 0.0f, 0.0f );
  drawQuad( 1.0f, 1.0f, 0.0f );

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{
  switch (key)
  {
    case 'w':
      gCameraY -= 16.0f;
      break;

    case 's':
      gCameraY += 16.0f;
      break;

    case 'a':
      gCameraX -= 16.0f;
      break;

    case 'd':
      gCameraX += 16.0f;
      break;

    default:
      break;
  }

  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();
  glLoadIdentity();

  glTranslatef( -gCameraX, -gCameraY, 0.0f );

  glPushMatrix();
}
