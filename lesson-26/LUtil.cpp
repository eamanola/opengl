#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

LTexture gTexture;

GLfloat gPolygonAngle = 0.f;
GLfloat gPolygonX = SCREEN_WIDTH / 2.f, gPolygonY = SCREEN_HEIGHT / 2.f;

GLuint gStencilRenderOp = GL_NOTEQUAL;

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

  glClearStencil( 0 );

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
  if( !gTexture.loadTextureFromFile32( "../assets/opengl.png" ) )
  {
    printf( "Unable to load ttf font!\n" );
    return false;
  }

  return true;
}

void update()
{
  gPolygonAngle += 6.f;
}

void render()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
  glLoadIdentity();

  glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );

  glEnable( GL_STENCIL_TEST );

  glStencilFunc( GL_ALWAYS, 1, 1 );

  glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );

  glTranslatef( gPolygonX, gPolygonY, 0.f );
  glRotatef( gPolygonAngle, 0.f, 0.f, 1.f );
  glBegin( GL_TRIANGLES );
    glVertex2f(          -0.f / 4.f, -SCREEN_HEIGHT / 4.f );
    glVertex2f(  SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
    glVertex2f( -SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
  glEnd();

  glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );

  glStencilFunc( gStencilRenderOp, 1, 1 );

  glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

  glLoadIdentity();
  gTexture.render(
    ( SCREEN_WIDTH - gTexture.imageWidth() ) / 2.f,
    ( SCREEN_HEIGHT - gTexture.imageHeight() ) / 2.f
  );

  glDisable( GL_STENCIL_TEST );

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{
  if( key == 'q' )
  {
    if(gStencilRenderOp == GL_NOTEQUAL)
    {
      gStencilRenderOp = GL_EQUAL;
    }
    else if (gStencilRenderOp == GL_EQUAL)
    {
      gStencilRenderOp = GL_ALWAYS;
    }
    else if (gStencilRenderOp == GL_ALWAYS)
    {
      gStencilRenderOp = GL_NOTEQUAL;
    }
  }
}

void handleMouseMotion( int x, int y )
{
  gPolygonX = x;
  gPolygonY = y;
}
