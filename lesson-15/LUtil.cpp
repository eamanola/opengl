#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

LTexture gRepeatingTexture;

GLfloat gTexX = 0.f, gTexY = 0.f;

int gTextureWrapType = 0;

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
  if ( !gRepeatingTexture.loadTextureFromFile( "../assets/gradient.png" ) )
  {
    printf( "Unable to load OpenGL texture!\n" );
    return false;
  }

  return true;
}

void update()
{
  gTexX ++;
  gTexY ++;

  if( gTexX >= gRepeatingTexture.textureWidth() )
  {
    gTexX = 0.f;
  }

  if( gTexY >= gRepeatingTexture.textureHeight() )
  {
    gTexY = 0.f;
  }
}

void render()
{
  glClear( GL_COLOR_BUFFER_BIT );

  GLfloat textureRight = (GLfloat)SCREEN_WIDTH / (GLfloat)gRepeatingTexture.textureWidth();
  GLfloat textureBottom = (GLfloat)SCREEN_HEIGHT /  (GLfloat)gRepeatingTexture.textureHeight();

  glBindTexture( GL_TEXTURE_2D, gRepeatingTexture.getTextureID() );

  glMatrixMode( GL_TEXTURE );

  glLoadIdentity();

  glTranslatef(
    gTexX / gRepeatingTexture.textureWidth(), gTexY / gRepeatingTexture.textureHeight(), 0.f
  );

  //Render
  glBegin( GL_QUADS );
      glTexCoord2f( 0.f,          0.f );           glVertex2f( 0.f,          0.f );
      glTexCoord2f( textureRight, 0.f );           glVertex2f( SCREEN_WIDTH, 0.f );
      glTexCoord2f( textureRight, textureBottom ); glVertex2f( SCREEN_WIDTH, SCREEN_HEIGHT );
      glTexCoord2f( 0.f,          textureBottom ); glVertex2f( 0.f,          SCREEN_HEIGHT );
  glEnd();

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{
  if( key == 'q' )
  {
    gTextureWrapType ++;

    if( gTextureWrapType >= 5 )
    {
      gTextureWrapType = 0;
    }

    glBindTexture( GL_TEXTURE_2D, gRepeatingTexture.getTextureID() );
    switch ( gTextureWrapType )
    {
      case 0:
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        printf( "%d: GL_REPEAT\n", gTextureWrapType );
      break;

      case 1:
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
        printf( "%d: GL_CLAMP\n", gTextureWrapType );
      break;

      case 2:
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
        printf( "%d: GL_CLAMP_TO_BORDER\n", gTextureWrapType );
      break;

      case 3:
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        printf( "%d: GL_CLAMP_TO_EDGE\n", gTextureWrapType );
      break;

      case 4:
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );
        printf( "%d: GL_MIRRORED_REPEAT\n", gTextureWrapType );
      break;

      default:
        break;
    }
  }
}
