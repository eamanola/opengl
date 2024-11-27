#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LFont.h"

LFont gFont;

LFRect gScaledArea = { 0.f, 0.f, 0.f, 0.f };
LFRect gPivotArea = { 0.f, 0.f, 0.f, 0.f };
LFRect gCirclingArea = { 0.f, 0.f, 0.f, 0.f };

GLfloat gBigTextScale = 3.f;
GLfloat gPivotAngle = 0.f;
GLfloat gCirclingAngle = 0.f;

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

  gScaledArea = gFont.getStringArea("Big Text!");
  gPivotArea = gFont.getStringArea("Pivot");
  gCirclingArea = gFont.getStringArea("Wheee");

  return true;
}

void update()
{
  gPivotAngle += -1.f;
  gCirclingAngle += 2.f;

  gBigTextScale += 0.03f;
  if(gBigTextScale >= 3.f) {
    gBigTextScale = 0.f;
  }
}

void render()
{
  glClear( GL_COLOR_BUFFER_BIT );

  glLoadIdentity();
  glColor3f( 1.f, 0.f, 0.f );

  glTranslatef(
    (SCREEN_WIDTH - gScaledArea.w * gBigTextScale) / 2.f,
    (SCREEN_HEIGHT - gScaledArea.h * gBigTextScale) / 4.f,
    0.f
  );

  glScalef(gBigTextScale, gBigTextScale, gBigTextScale);
  gFont.renderText( 0.f, 0.f, "Big Text!", &gScaledArea, LFONT_TEXT_ALIGN_CENTERED_H );


  glLoadIdentity();
  glColor3f(0.f, 1.f, 0.f);

  glTranslatef(
    (SCREEN_WIDTH - gPivotArea.w * 1.5f) / 2.f,
    (SCREEN_HEIGHT - gPivotArea.h * 1.5f) * 3.f / 4.f,
    0.f
  );

  glScalef(1.5f, 1.5f, 1.5f);
  glTranslatef(gPivotArea.w / 2.f, gPivotArea.h / 2.f, 0.f);

  glRotatef(gPivotAngle, 0.f, 0.f, 1.f);
  glTranslatef(-gPivotArea.w / 2.f, -gPivotArea.h / 2.f, 0.f);
  gFont.renderText(0.f, 0.f, "Pivot", &gPivotArea, LFONT_TEXT_ALIGN_CENTERED_H);

  glLoadIdentity();
  glColor3f(0.f, 1.f, 0.f);

  glTranslatef((SCREEN_WIDTH) / 2.f, (SCREEN_HEIGHT) / 2.f, 0.f);

  glRotatef(gCirclingAngle, 0.f, 0.f, 1.f);

  glTranslatef(0.f, -SCREEN_HEIGHT / 2.f, 0.f);
  glTranslatef(-gCirclingArea.w / 2.f, 0.f, 0.f);

  gFont.renderText(0.f, 0.f, "Wheee", &gPivotArea, LFONT_TEXT_ALIGN_CENTERED_H);

  glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{

}
