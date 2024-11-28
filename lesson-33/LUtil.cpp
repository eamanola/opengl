#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LMultiColorPolygonProgram2D.h"
#include "LMultiColorVertex2D.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

LMultiColorPolygonProgram2D gProgram;

GLuint gVBO = 0;
GLuint gIBO = 0;

bool initGL()
{
  GLenum glewError = glewInit();
  if( glewError != GLEW_OK )
  {
    printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
    return false;
  }

  if( !GLEW_VERSION_3_1 )
  {
    printf( "OpenGL 3.1 not supported!\n" );
    return false;
  }

  glViewport( 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT );

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

  gProgram.setProjection( glm::ortho<GLfloat>( 0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.f, 1.f, -1.f ) );
  gProgram.updateProjection();

  gProgram.setModelView( glm::mat4() );
  gProgram.updateModelView();

  return true;
}

bool loadMedia()
{
  LMultiColorVertex2D quadVertices[ 4 ];
  GLuint indices[ 4 ];

  quadVertices[ 0 ].pos.x = -50.f;
  quadVertices[ 0 ].pos.y = -50.f;
  quadVertices[ 0 ].rgba.r = 1.f;
  quadVertices[ 0 ].rgba.g = 0.f;
  quadVertices[ 0 ].rgba.b = 0.f;
  quadVertices[ 0 ].rgba.a = 1.f;

  quadVertices[ 1 ].pos.x =  50.f;
  quadVertices[ 1 ].pos.y = -50.f;
  quadVertices[ 1 ].rgba.r = 1.f;
  quadVertices[ 1 ].rgba.g = 1.f;
  quadVertices[ 1 ].rgba.b = 0.f;
  quadVertices[ 1 ].rgba.a = 1.f;

  quadVertices[ 2 ].pos.x =  50.f;
  quadVertices[ 2 ].pos.y =  50.f;
  quadVertices[ 2 ].rgba.r = 0.f;
  quadVertices[ 2 ].rgba.g = 1.f;
  quadVertices[ 2 ].rgba.b = 0.f;
  quadVertices[ 2 ].rgba.a = 1.f;

  quadVertices[ 3 ].pos.x = -50.f;
  quadVertices[ 3 ].pos.y =  50.f;
  quadVertices[ 3 ].rgba.r = 0.f;
  quadVertices[ 3 ].rgba.g = 0.f;
  quadVertices[ 3 ].rgba.b = 1.f;
  quadVertices[ 3 ].rgba.a = 1.f;

  indices[ 0 ] = 0;
  indices[ 1 ] = 1;
  indices[ 2 ] = 2;
  indices[ 3 ] = 3;

  glGenBuffers( 1, &gVBO );
  glBindBuffer( GL_ARRAY_BUFFER, gVBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW );

  glGenBuffers(1, &gIBO );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW );

  return true;
}

void update()
{

}

void render()
{
  glClear( GL_COLOR_BUFFER_BIT );

  gProgram.setModelView(
    glm::translate<GLfloat>( glm::vec3( SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, 0.f ) )
  );
  gProgram.updateModelView();

  gProgram.enableVertexPointer();
  gProgram.enableColorPointer();

    glBindBuffer( GL_ARRAY_BUFFER, gVBO );
    gProgram.setVertexPointer(
      sizeof(LMultiColorVertex2D), (GLvoid*)offsetof( LMultiColorVertex2D, pos )
    );
    gProgram.setColorPointer(
      sizeof(LMultiColorVertex2D), (GLvoid*)offsetof( LMultiColorVertex2D, rgba )
    );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
    glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );

  gProgram.disableVertexPointer();
  gProgram.disableColorPointer();

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{
}

void handleMouseMotion( int x, int y )
{
}
