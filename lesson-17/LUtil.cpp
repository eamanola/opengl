#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "./LVertexPos2D.h"

LVertexPos2D gQuadVertices[ 4 ];

GLuint gIndices[ 4 ];

GLuint gVertexBuffer = 0;

GLuint gIndexBuffer = 0;

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

  gIndices[ 0 ] = 0;
  gIndices[ 1 ] = 1;
  gIndices[ 2 ] = 2;
  gIndices[ 3 ] = 3;

  // VBO
  glGenBuffers( 1, &gVertexBuffer );
  glBindBuffer( GL_ARRAY_BUFFER, gVertexBuffer );
  // glBufferData( GL_ARRAY_BUFFER, sizeof(gQuadVertices), gQuadVertices, GL_STATIC_DRAW );
  glBufferData( GL_ARRAY_BUFFER, 4 * sizeof(LVertexPos2D), gQuadVertices, GL_STATIC_DRAW );

  // IBO
  glGenBuffers( 1, &gIndexBuffer );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), gIndices, GL_STATIC_DRAW );

  return true;
}

void update()
{

}

void render()
{
  glClear( GL_COLOR_BUFFER_BIT );

  glEnableClientState( GL_VERTEX_ARRAY );

    glBindBuffer( GL_ARRAY_BUFFER, gVertexBuffer );
    glVertexPointer( 2, GL_FLOAT, 0, NULL );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer );
    glDrawElements( GL_QUADS, 4, GL_UNSIGNED_INT, NULL );

  glDisableClientState( GL_VERTEX_ARRAY );

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{

}
