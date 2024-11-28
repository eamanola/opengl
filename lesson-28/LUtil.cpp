#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

AliasMode gMode = ALIAS_MODE_ALIASED;

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

  glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
  glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
  glDisable( GL_LINE_SMOOTH );
  glDisable( GL_POLYGON_SMOOTH );
  glDisable( GL_MULTISAMPLE );

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

  // if( !LFont::initFreeType() )
  // {
  //   printf( "Unable to initialize FreeType!\n" );
  // }

  return true;
}

bool loadMedia()
{
  return true;
}

void update()
{

}

void render()
{
  glClear( GL_COLOR_BUFFER_BIT );

  switch (gMode)
  {
    case ALIAS_MODE_ALIASED:
      glDisable( GL_LINE_SMOOTH );
      glDisable( GL_POLYGON_SMOOTH );
      glDisable( GL_MULTISAMPLE );
      break;

    case ALIAS_MODE_ANTIALIASED:
      glEnable( GL_LINE_SMOOTH );
      glEnable( GL_POLYGON_SMOOTH );
      glDisable( GL_MULTISAMPLE );
      break;

    case ALIAS_MODE_MULTISAMPLE:
      glDisable( GL_LINE_SMOOTH );
      glDisable( GL_POLYGON_SMOOTH );
      glEnable( GL_MULTISAMPLE );
      break;

    default:
      break;
  }

  glColor3f( 1.f, 1.f, 1.f );
  glBegin( GL_TRIANGLES );
    glVertex2f( SCREEN_WIDTH, 0.f );
    glVertex2f( SCREEN_WIDTH, SCREEN_HEIGHT );
    glVertex2f( 0.f, SCREEN_HEIGHT );
  glEnd();

  switch (gMode)
  {
    case ALIAS_MODE_ANTIALIASED:
      glDisable( GL_LINE_SMOOTH );
      glDisable( GL_POLYGON_SMOOTH );
      break;

    case ALIAS_MODE_MULTISAMPLE:
      glDisable( GL_MULTISAMPLE );
      break;

    default:
      break;
  }

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{
  if( key == 'q' )
  {
    switch (gMode)
    {
      case ALIAS_MODE_ALIASED:
        printf("Antialiased\n");
        gMode = ALIAS_MODE_ANTIALIASED;
        break;

      case ALIAS_MODE_ANTIALIASED:
        printf("Multisample\n");
        gMode = ALIAS_MODE_MULTISAMPLE;
        break;

      case ALIAS_MODE_MULTISAMPLE:
        printf("Aaliased\n");
        gMode = ALIAS_MODE_ALIASED;
        break;

      default:
        break;
    }

  }
}

void handleMouseMotion( int x, int y )
{
}
