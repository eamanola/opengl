#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LFont.h"

LFont gFont;
LFontTextAlignment gAlignH = LFONT_TEXT_ALIGN_LEFT;
LFontTextAlignment gAlignV = LFONT_TEXT_ALIGN_TOP;
int gAlign = gAlignH | gAlignV;
LFRect gScreenArea = { 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT };

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

  if( !LFont::initFreeType() )
  {
    printf( "Unable to initialize FreeType!\n" );
  }

  return true;
}

bool loadMedia()
{
  if( !gFont.loadFreeType( "../assets/lazy.ttf", 60 ) )
  {
    printf( "Unable to load ttf font!\n" );
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

  glColor3f( 0.f, 0.f, 1.f );
  gFont.renderText( 0.f, SCREEN_HEIGHT / 2.f, "Now rendering \nwith TTF!", &gScreenArea, gAlign );

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{
  if( key == 'a' )
  {
    switch( gAlignH )
    {
      case LFONT_TEXT_ALIGN_LEFT:
        gAlignH = LFONT_TEXT_ALIGN_RIGHT;
        break;
      case LFONT_TEXT_ALIGN_CENTERED_H:
        gAlignH = LFONT_TEXT_ALIGN_LEFT;
        break;
      case LFONT_TEXT_ALIGN_RIGHT:
        gAlignH = LFONT_TEXT_ALIGN_CENTERED_H;
        break;
      default:
        break;
    }
  }
  else if( key == 'd' )
  {
    switch( gAlignH )
    {
      case LFONT_TEXT_ALIGN_LEFT:
        gAlignH = LFONT_TEXT_ALIGN_CENTERED_H;
        break;
      case LFONT_TEXT_ALIGN_CENTERED_H:
        gAlignH = LFONT_TEXT_ALIGN_RIGHT;
        break;
      case LFONT_TEXT_ALIGN_RIGHT:
        gAlignH = LFONT_TEXT_ALIGN_LEFT;
        break;
      default:
        break;
    }
  }
  else if( key == 'w' )
  {
      switch( gAlignV )
      {
        case LFONT_TEXT_ALIGN_TOP:
          gAlignV = LFONT_TEXT_ALIGN_BOTTOM;
          break;
        case LFONT_TEXT_ALIGN_CENTERED_V:
          gAlignV = LFONT_TEXT_ALIGN_TOP;
          break;
        case LFONT_TEXT_ALIGN_BOTTOM:
          gAlignV = LFONT_TEXT_ALIGN_CENTERED_V;
          break;
        default:
          break;
      }
  }
  else if( key == 's' )
  {
      switch( gAlignV )
      {
        case LFONT_TEXT_ALIGN_TOP:
          gAlignV = LFONT_TEXT_ALIGN_CENTERED_V;
          break;
        case LFONT_TEXT_ALIGN_CENTERED_V:
          gAlignV = LFONT_TEXT_ALIGN_BOTTOM;
          break;
        case LFONT_TEXT_ALIGN_BOTTOM:
          gAlignV = LFONT_TEXT_ALIGN_TOP;
          break;
        default:
          break;
      }
  }

  //Set alignment
  gAlign = gAlignH | gAlignV;
}
