#include "LTexture.h"
#include <IL/il.h>
#include <string>

LTexture::LTexture()
{
  mTextureID = 0;
  mTextureWidth = 0;
  mTextureHeight = 0;
}

LTexture::~LTexture()
{
  freeTexture();
}

bool LTexture::loadTextureFromPixels32( GLuint* pixels, GLuint width, GLuint height )
{
  freeTexture();

  mTextureWidth = width;
  mTextureHeight = height;

  glGenTextures( 1, &mTextureID );
  glBindTexture( GL_TEXTURE_2D, mTextureID );

  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

  glBindTexture( GL_TEXTURE_2D, NULL );

  GLenum error = glGetError();
  if( error != GL_NO_ERROR )
  {
    printf( "Error loading texture from %p pixels! %s\n", pixels, gluErrorString( error ) );
    return false;
  }

  return true;
}

bool LTexture::loadTexttureFromFile( std::string path )
{
  bool textureLoaded = false;

  ILuint imgID = 0;
  ilGenImages( 1, &imgID );
  ilBindImage( imgID );

  ILboolean success = ilLoadImage( path.c_str() );

  if ( success == IL_TRUE )
  {
    success = ilConvertImage( IL_RGBA, IL_UNSIGNED_BYTE );

    if ( success == IL_TRUE )
    {
      textureLoaded = loadTextureFromPixels32(
        (GLuint*)ilGetData(),
        (GLuint)ilGetInteger( IL_IMAGE_WIDTH ),
        (GLuint)ilGetInteger( IL_IMAGE_HEIGHT )
      );
    }

    ilDeleteImages( 1, &imgID );
  }

  return textureLoaded;
}

void LTexture::freeTexture()
{
  if( mTextureID != 0 )
  {
    glDeleteTextures( 1, &mTextureID );
    mTextureID = 0;
  }

  mTextureWidth = 0;
  mTextureHeight = 0;
}

void LTexture::render( GLfloat x, GLfloat y )
{
  if(mTextureID != 0)
  {
    glLoadIdentity();

    glTranslatef( x, y, 0.0f );

    glBindTexture( GL_TEXTURE_2D, mTextureID );

    glBegin( GL_QUADS );
      glTexCoord2f( 0.0f, 0.0f ); glVertex2f( 0.0f,           0.0f );
      glTexCoord2f( 1.0f, 0.0f ); glVertex2f( mTextureWidth,  0.0f );
      glTexCoord2f( 1.0f, 1.0f ); glVertex2f( mTextureWidth,  mTextureHeight );
      glTexCoord2f( 0.0f, 1.0f ); glVertex2f( 0.0f,           mTextureHeight );
    glEnd();
  }
}

GLuint LTexture::getTextureID()
{
  return mTextureID;
}

GLuint LTexture::textureWidth()
{
  return mTextureWidth;
}

GLuint LTexture::textureHeight()
{
  return mTextureHeight;
}
