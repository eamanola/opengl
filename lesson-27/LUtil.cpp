#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

LTexture gOpenGLTexture;
GLfloat gAngle = 0.f;

GLuint gFBO = 0;
LTexture gFBOTexture;

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

  glGenFramebuffers( 1, &gFBO );

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
  if( !gOpenGLTexture.loadTextureFromFile32( "../assets/opengl.png" ) )
  {
    printf( "Unable to load opengl texture!\n" );
    return false;
  }

  return true;
}

void update()
{
  gAngle += 6.f;
}

void renderScene()
{
  glLoadIdentity();
  glTranslatef( SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, 0.f );
  glRotatef( gAngle, 0.f, 0.f, 1.f );
  glTranslatef( gOpenGLTexture.imageWidth() / -2.f, gOpenGLTexture.imageHeight() / -2.f, 0.f );
  glColor3f( 1.f, 1.f, 1.f );
  gOpenGLTexture.render( 0.f, 0.f );

  glBindTexture( GL_TEXTURE_2D, 0 );

  //Red quad
  glLoadIdentity();
  glTranslatef( SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f, 0.f );
  glRotatef( gAngle, 0.f, 0.f, 1.f );
  glBegin( GL_QUADS );
    glColor3f( 1.f, 0.f, 0.f );
    glVertex2f( -SCREEN_WIDTH / 16.f, -SCREEN_HEIGHT / 16.f );
    glVertex2f(  SCREEN_WIDTH / 16.f, -SCREEN_HEIGHT / 16.f );
    glVertex2f(  SCREEN_WIDTH / 16.f,  SCREEN_HEIGHT / 16.f );
    glVertex2f( -SCREEN_WIDTH / 16.f,  SCREEN_HEIGHT / 16.f );
  glEnd();

  //Green quad
  glLoadIdentity();
  glTranslatef( SCREEN_WIDTH * 3.f / 4.f, SCREEN_HEIGHT * 1.f / 4.f, 0.f );
  glRotatef( gAngle, 0.f, 0.f, 1.f );
  glBegin( GL_QUADS );
    glColor3f( 0.f, 1.f, 0.f );
    glVertex2f( -SCREEN_WIDTH / 16.f, -SCREEN_HEIGHT / 16.f );
    glVertex2f(  SCREEN_WIDTH / 16.f, -SCREEN_HEIGHT / 16.f );
    glVertex2f(  SCREEN_WIDTH / 16.f,  SCREEN_HEIGHT / 16.f );
    glVertex2f( -SCREEN_WIDTH / 16.f,  SCREEN_HEIGHT / 16.f );
  glEnd();

  //Blue quad
  glLoadIdentity();
  glTranslatef( SCREEN_WIDTH * 1.f / 4.f, SCREEN_HEIGHT * 3.f / 4.f, 0.f );
  glRotatef( gAngle, 0.f, 0.f, 1.f );
  glBegin( GL_QUADS );
    glColor3f( 0.f, 0.f, 1.f );
    glVertex2f( -SCREEN_WIDTH / 16.f, -SCREEN_HEIGHT / 16.f );
    glVertex2f(  SCREEN_WIDTH / 16.f, -SCREEN_HEIGHT / 16.f );
    glVertex2f(  SCREEN_WIDTH / 16.f,  SCREEN_HEIGHT / 16.f );
    glVertex2f( -SCREEN_WIDTH / 16.f,  SCREEN_HEIGHT / 16.f );
  glEnd();

  //Yellow quad
  glLoadIdentity();
  glTranslatef( SCREEN_WIDTH * 3.f / 4.f, SCREEN_HEIGHT * 3.f / 4.f, 0.f );
  glRotatef( gAngle, 0.f, 0.f, 1.f );
  glBegin( GL_QUADS );
    glColor3f( 1.f, 1.f, 0.f );
    glVertex2f( -SCREEN_WIDTH / 16.f, -SCREEN_HEIGHT / 16.f );
    glVertex2f(  SCREEN_WIDTH / 16.f, -SCREEN_HEIGHT / 16.f );
    glVertex2f(  SCREEN_WIDTH / 16.f,  SCREEN_HEIGHT / 16.f );
    glVertex2f( -SCREEN_WIDTH / 16.f,  SCREEN_HEIGHT / 16.f );
  glEnd();
}

void render()
{
  glClear( GL_COLOR_BUFFER_BIT );

  if( gFBOTexture.getTextureID() != 0 )
  {
    glLoadIdentity();
    glTranslatef( SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, 0.f );
    glRotatef( -gAngle, 0.f, 0.f, 1.f );
    glTranslatef( gFBOTexture.imageWidth() / -2.f, gFBOTexture.imageHeight() / -2.f, 0.f );
    glColor3f( 1.f, 1.f, 1.f );
    gFBOTexture.render( 0.f, 0.f );
  }

  renderScene();

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{
  if( key == 'q' )
  {
    glBindFramebuffer( GL_FRAMEBUFFER, gFBO );

    if( gFBOTexture.getTextureID() == 0)
    {
      gFBOTexture.createPixels32( SCREEN_WIDTH, SCREEN_HEIGHT );
      gFBOTexture.padPixels32();
      gFBOTexture.loadTextureFromPixels32();
    }

    glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gFBOTexture.getTextureID(), 0
    );

    glClear( GL_COLOR_BUFFER_BIT );

    renderScene();

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
  }
}

void handleMouseMotion( int x, int y )
{
}
