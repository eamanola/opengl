#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexturedPolygonProgram2D.h"
#include "LMultiColorVertex2D.h"
#include "LTexture.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

LTexturedPolygonProgram2D gProgram;
LTexture gTexture;
LColorRGBA gColor = { 1.f, 1.f, 1.f, 0.75f };

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
    printf( "Unable to load textured polygon program!\n" );
    return false;
  }

  gProgram.bind();

  gProgram.setProjection( glm::ortho<GLfloat>( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 ) );
  gProgram.updateProjection();

  gProgram.setModelView( glm::mat4() );
  gProgram.updateModelView();

  gProgram.setTextureUnit( 0 );

  LTexture::setTexturedPolygonProgram2D( &gProgram );

  return true;
}

bool loadMedia()
{
  if( !gTexture.loadTextureFromFile32( "../assets/opengl.png" ) )
  {
      printf( "Unable to load texture!\n" );
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

  gProgram.setModelView( glm::mat4() );

  // bugs
  gProgram.setTextureColor( gColor );

  gTexture.render(
    ( SCREEN_WIDTH - gTexture.imageWidth() ) / 2.f,
    ( SCREEN_HEIGHT - gTexture.imageHeight() ) / 2.f
  );

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{
}

void handleMouseMotion( int x, int y )
{
}
