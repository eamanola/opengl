#include "LUtil.h"
#include "LTexture.h"

LTexture gCheckerBoardTexture;

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

  return true;
}

bool loadMedia()
{
  const int CHECKERBOARD_WIDTH = 128;
  const int CHECKERBOARD_HEIGHT = 128;
  const int CHECKERBOARD_PIXEL_COUNT = CHECKERBOARD_WIDTH * CHECKERBOARD_HEIGHT;
  GLuint checkerBoard[ CHECKERBOARD_PIXEL_COUNT ];

  for( int i = 0; i < CHECKERBOARD_PIXEL_COUNT; ++i )
  {
    GLubyte* colors = (GLubyte*)&checkerBoard[ i ];

    if( i / 128 & 16 ^ i % 128 & 16 )
    {
      colors[ 0 ] = 0xFF;
      colors[ 1 ] = 0xFF;
      colors[ 2 ] = 0xFF;
      colors[ 3 ] = 0xFF;
    }
    else
    {
      colors[ 0 ] = 0xFF;
      colors[ 1 ] = 0x00;
      colors[ 2 ] = 0x00;
      colors[ 3 ] = 0xFF;
    }
  }

  if( !gCheckerBoardTexture.loadTextureFromPixels32( checkerBoard, CHECKERBOARD_WIDTH, CHECKERBOARD_HEIGHT ) )
  {
    printf( "Unable to load checkerboard texture!\n" );
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

  GLfloat x = ( SCREEN_WIDTH - gCheckerBoardTexture.textureWidth() ) / 2.0f;
  GLfloat y = ( SCREEN_HEIGHT - gCheckerBoardTexture.textureHeight() ) / 2.0f;

  gCheckerBoardTexture.render( x, y );

  glutSwapBuffers();
}
