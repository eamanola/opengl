#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

LTexture gArrowTexture;

LFRect gArrowClips[ 4 ];

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

  ilInit();
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
  if ( !gArrowTexture.loadTexttureFromFile( "../assets/arrows.png" ) )
  {
    printf( "Unable to load arrow texture!\n" );
    return false;
  }

  const GLfloat halfw = gArrowTexture.textureWidth() / 2.f;
  const GLfloat halfh = gArrowTexture.textureHeight() / 2.f;

  gArrowClips[ 0 ].x = 0.f;
  gArrowClips[ 0 ].y = 0.f;
  gArrowClips[ 0 ].w = halfw;
  gArrowClips[ 0 ].h = halfh;

  gArrowClips[ 1 ].x = halfw;
  gArrowClips[ 1 ].y = 0.f;
  gArrowClips[ 1 ].w = halfw;
  gArrowClips[ 1 ].h = halfh;

  gArrowClips[ 2 ].x = 0.f;
  gArrowClips[ 2 ].y = halfh;
  gArrowClips[ 2 ].w = halfw;
  gArrowClips[ 2 ].h = halfh;

  gArrowClips[ 3 ].x = halfw;
  gArrowClips[ 3 ].y = halfh;
  gArrowClips[ 3 ].w = halfw;
  gArrowClips[ 3 ].h = halfh;



  return true;
}

void update()
{

}

void render()
{
  glClear( GL_COLOR_BUFFER_BIT );

  gArrowTexture.render( 0.f, 0.f, &gArrowClips[ 0 ] );
  gArrowTexture.render( SCREEN_WIDTH - gArrowClips[ 1 ].w, 0.f, &gArrowClips[ 1 ] );
  gArrowTexture.render( 0.f, SCREEN_HEIGHT - gArrowClips[ 2 ].h, &gArrowClips[ 2 ] );
  gArrowTexture.render( SCREEN_WIDTH - gArrowClips[ 3 ].w, SCREEN_HEIGHT - gArrowClips[ 3 ].h, &gArrowClips[ 3 ] );

  glutSwapBuffers();
}
