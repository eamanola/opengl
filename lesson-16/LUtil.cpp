#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "./LVertexPos2D.h"

LVertexPos2D gQuadVertices[ 4 ];

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

bool loadMedia()
{
  gQuadVertices[ 0 ].x = SCREEN_WIDTH * 1.f / 4.f;
  gQuadVertices[ 0 ].y = SCREEN_HEIGHT * 1.f / 4.f;

  gQuadVertices[ 1 ].x = SCREEN_WIDTH * 3.f / 4.f;
  gQuadVertices[ 1 ].y = SCREEN_HEIGHT * 1.f / 4.f;

  gQuadVertices[ 2 ].x = SCREEN_WIDTH * 3.f / 4.f;
  gQuadVertices[ 2 ].y = SCREEN_HEIGHT * 3.f / 4.f;

  gQuadVertices[ 3 ].x = SCREEN_WIDTH * 1.f / 4.f;
  gQuadVertices[ 3 ].y = SCREEN_HEIGHT * 3.f / 4.f;

  return true;
}

void update()
{

}

void render()
{
  glClear( GL_COLOR_BUFFER_BIT );

  glEnableClientState( GL_VERTEX_ARRAY );

    glVertexPointer( 2, GL_FLOAT, 0, gQuadVertices );

    glDrawArrays( GL_QUADS, 0, 4 );

  glDisableClientState( GL_VERTEX_ARRAY );

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{

}
