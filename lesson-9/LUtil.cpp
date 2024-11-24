#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

LTexture gCircleTexture;

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
  if ( !gCircleTexture.loadTexttureFromFile( "./circle.png" ) )
  {
    printf( "Unable to load circle texture!\n" );
    return false;
  }

  gCircleTexture.lock();

  GLuint targetColor;
  GLubyte* colors = (GLubyte*)&targetColor;
  colors[ 0 ] = 000;
  colors[ 1 ] = 255;
  colors[ 2 ] = 255;
  colors[ 3 ] = 255;

  const int width = (int)gCircleTexture.textureWidth();
  const int height = (int)gCircleTexture.textureHeight();

  // Replace target color with transparent black
  GLuint* pixels = gCircleTexture.getPixelData32();
  int pixelCount =  width * height;
  for( int i = 0; i < pixelCount; ++i )
  {
    if( pixels[ i ] == targetColor )
    {
      pixels[ i ] = 0;
    }
  }

  // Diagonal Lines
  for( int y = 0; y < height; ++y )
  {
    for( int x = 0; x < width; ++x )
    {
      if( y % 10 != x % 10)
      {
        gCircleTexture.setPixel32( x, y, 0 );
      }
    }
  }

  gCircleTexture.unlock();

  return true;
}

void update()
{

}

void render()
{
  glClear( GL_COLOR_BUFFER_BIT );

  gCircleTexture.render(
    ( SCREEN_WIDTH - gCircleTexture.imageWidth() ) / 2.f,
    ( SCREEN_HEIGHT - gCircleTexture.imageHeight() ) / 2.f
  );

  glutSwapBuffers();
}
