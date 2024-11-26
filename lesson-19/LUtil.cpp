#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LSpriteSheet.h"

LSpriteSheet gArrowSprites;

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
  if( !gArrowSprites.loadTextureFromFile( "../assets/arrows.png" ) )
  {
    printf( "Unable to load sprite sheet!\n" );
    return false;
  }

  LFRect clip = { 0.f, 0.f, 128.f, 128.f };

  clip.x = 0.f;
  clip.y = 0.f;
  gArrowSprites.addClipSprite( clip );

  clip.x = 128.f;
  clip.y = 0.f;
  gArrowSprites.addClipSprite( clip );

  clip.x = 0.f;
  clip.y = 128.f;
  gArrowSprites.addClipSprite( clip );

  clip.x = 128.f;
  clip.y = 128.f;
  gArrowSprites.addClipSprite( clip );

  if ( !gArrowSprites.generateDataBuffer() )
  {
    printf( "Unable to clip sprite sheet!\n" );
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
  glTranslatef( 64.f, 64.f, 0.f );
  gArrowSprites.renderSprite( 0 );

  glLoadIdentity();
  glTranslatef( SCREEN_WIDTH - 64.f, 64.f, 0.f );
  gArrowSprites.renderSprite( 1 );

  glLoadIdentity();
  glTranslatef( 64.f, SCREEN_HEIGHT - 64.f, 0.f );
  gArrowSprites.renderSprite( 2 );

  glLoadIdentity();
  glTranslatef( SCREEN_WIDTH - 64.f, SCREEN_HEIGHT - 64.f, 0.f );
  gArrowSprites.renderSprite( 3 );

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{

}
